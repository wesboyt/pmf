//
// Created by wes on 2/23/20.
//
#include "solve.h"
int main() {
    auto cards = parseCards("KdQh,JsJd|AsTh9c");
    std::pair<std::vector<float>, std::vector<std::vector<int>>> result = solve(cards.first, cards.second);
}

