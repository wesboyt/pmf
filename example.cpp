//
// Created by wes on 2/23/20.
//
#include "solve.h"
int main() {
    //dist();
    std::pair<std::vector<float>, std::vector<std::vector<int>>> result = solvePmf("3s4hAdAh,4d9c9h4c");
    for(float ev : result.first) {
        std::cout << ev << std::endl;
    }
    for(float ev : solveEquity("3s4h,4d9c")) {
        std::cout << ev << std::endl;
    }
}

