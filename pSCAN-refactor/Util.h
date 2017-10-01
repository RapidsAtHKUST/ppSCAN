//
// Created by yche on 9/5/17.
//

#ifndef PPSCAN_DATA_STRUCTURE_UTIL_H
#define PPSCAN_DATA_STRUCTURE_UTIL_H

#include <memory>

namespace yche {
    constexpr int NOT_SIMILAR = -2;
    constexpr int SIMILAR = -1;

    constexpr char TRUE = 1;
    constexpr char FALSE = 0;

    template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args &&... args) {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
}

namespace std {
    template<>
    struct hash<std::pair<int, int>> {
        inline size_t operator()(const std::pair<int, int> &v) const {
            std::hash<int> int_hasher;
            return int_hasher(v.first) ^ int_hasher(v.second);
        }
    };
}


#endif //PPSCAN_DATA_STRUCTURE_UTIL_H
