#pragma once
#include "map.h"
#include "set.h"

Map<std::string, Set<std::string>> readDocs(std::string filename);

Map<std::string, Set<std::string>> buildIndex(Map<std::string, Set<std::string>>& docs);

Set<std::string> findQueryMatches(Map<std::string, Set<std::string>>& index, std::string query);

void searchEngine(std::string dbfile);
