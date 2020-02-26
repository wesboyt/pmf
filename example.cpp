//
// Created by wes on 2/23/20.
//
#include "solve.h"
int main() {
    std::vector<char> suitLookup = {'s', 'h', 'd', 'c'};
    std::vector<char> cardLookup = {'A','K', 'Q', 'J', 'T', '9', '8', '7', '6', '5', '4', '3', '2'};
    std::pair<std::vector<float>, std::vector<std::vector<int>>> result = solve("KdQh,JsJd,TsTc");
}

