//
// Created by wes on 2/23/20.
//
#include "solve1.h"
int main() {
    std::pair<std::vector<float>, std::vector<std::vector<int>>> result = solve("AsQh,Ac8s");
    for(float ev : result.first) {
        std::cout << ev << std::endl;
    }
    //std::set<uint8_t> deck = {};
    //std::vector<uint8_t> test;
    //for(int i = 0; i < 50; i++) {
    //    deck.insert(i);
    //}
    //auto combos = combinations(test, 2);


    //std::cout << combos.size() << std::endl;
}

