#include "shared.h"

int hashCode(const Thing& t) {
    return t.num;
}
bool operator ==(const Thing& t1, const Thing& t2) {
    return t1.num == t2.num && t1.label == t2.label;
}
bool operator <(const Thing& t1, const Thing& t2) {
    return stanfordcpplib::collections::compareTo(t1.num, t2.num, t1.label, t2.label) < 0;
}