//
// Created by wes on 5/2/20.
//

#ifndef PMF_COMBINATION_H
#define PMF_COMBINATION_H

#include <vector>
#include <cstdint>
#include <cstdlib>

std::vector<std::vector<uint8_t>> combination(std::vector<uint8_t> src, int r) {
    std::vector<std::vector<uint8_t>> cs;
    if (r == 1) {
        for (auto i = 0; i < src.size(); i++) {
            std::vector<uint8_t> c;
            c.push_back(src[i]);
            cs.push_back(c);
        }
        return cs;
    }
    int* places = (int*)malloc(sizeof(int) * r);
    for (auto i = 0; i < r; i++) places[i] = i;
    while (true) {
        // push_back another combination
        std::vector<uint8_t> c; c.reserve(r);
        for (auto i = 0; i < r; i++) {
            c.push_back(src[places[i]]);
        }
        cs.push_back(c);
        // update places
        for (auto i = 0; i < r-1; i++) {
            places[i]++;
            if (places[i+1] == places[i]) {
                places[i] = i;
                if (i == r-2) places[r-1]++;
                continue;
            }
            break;
        }
        if (places[r-1] == src.size()) break;
    }
    free(places);
    return cs;
}
#endif //PMF_COMBINATION_H
