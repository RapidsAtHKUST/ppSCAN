//
// Created by yche on 9/5/17.
//

#ifndef PSCAN_DATA_STRUCTURE_UTIL_H
#define PSCAN_DATA_STRUCTURE_UTIL_H

namespace std {
    template<>
    struct hash<std::pair<int, int>> {
        inline size_t operator()(const std::pair<int, int> &v) const {
            std::hash<int> int_hasher;
            return int_hasher(v.first) ^ int_hasher(v.second);
        }
    };
}

#endif //PSCAN_DATA_STRUCTURE_UTIL_H
