/**
 * MemoryDiagnostics.cpp
 *
 * @author Keith Schwarz
 * @version 2020/3/5
 *    Keith final revision from end of quarter 19-2
 */

#include "MemoryDiagnostics.h"
#include "hashmap.h"
#include "error.h"
#include <sstream>
#include <unordered_map>
#include <typeindex>
#include <cxxabi.h> // Nonstandard, but supported on g++ and clang.
using namespace std;

namespace {
    /* Type --> Frequency */
    unordered_map<type_index, int>& allocationTable() {
        static unordered_map<type_index, int> instance;
        return instance;
    }

    /* Name --> Type */
    unordered_map<string, type_index>& lookupTable() {
        static unordered_map<string, type_index> instance;
        return instance;
    }
}

namespace MemoryDiagnostics {
    /* Injects the type into the appropriate table. */
    int registerSentinel(const std::type_info& type) {
        /* std::type_info does not guarantee that .name() will be at all human-readable.
         * Use this g++/clang-specific logic to "demangle" the name back into a human-readable
         * format.
         */
        int statusCode;
        auto* realName = abi::__cxa_demangle(type.name(), nullptr, nullptr, &statusCode);
        if (statusCode != 0) error("Internal error: Couldn't demangle name?");

        /* C++ type_info objects don't work well as keys in maps, but std::type_index
         * allows us to fix that.
         */
        lookupTable().insert(make_pair(string(realName), type_index(type)));

        free(realName);
        return 137; // You know, cause, why not?
    }

    void recordNew(const type_info& type) {
        allocationTable()[type_index(type)]++;
    }

    void recordDelete(const type_info& type) {
        allocationTable()[type_index(type)]--;
    }

    /* Clears the allocation table. */
    void clear() {
        allocationTable().clear();
    }

    /* Returns a list of all imbalanced types. */
    map<string, int> typesWithErrors() {
        map<string, int> result;

        /* Loop over types, looking for mismatches. */
        for (const auto& entry: lookupTable()) {
            int record = allocationTable()[entry.second];
            if (record != 0) {
                result[entry.first] = record;
            }
        }

        return result;
    }
}
