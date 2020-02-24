//
// Created by wes on 2/22/20.
//

#include <vector>
#include <map>
#include <cstring>
#include <iostream>
#include <chrono>
#include <algorithm>
#include <set>
#include "SKPokerEval/src/SevenEval.h"

#ifndef PMF_SOLVE_H
#define PMF_SOLVE_H

#endif //PMF_SOLVE_H

std::vector<std::vector<uint8_t>> combinations(std::vector<uint8_t> src, int r) {
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


std::vector<int> getRanks(std::vector<std::vector<uint8_t>> cards, std::vector<uint8_t> board) {
    std::vector<int> result;
    uint size = cards.size();
    for(uint i = 0; i < size; i++) {
        result.emplace_back(SevenEval::GetRank(board[0], board[1], board[2], board[3], board[4], cards[i][0], cards[i][1]));
    }
    return result;
}


static inline std::pair<std::vector<float>, std::vector<std::vector<int>>> solve(std::string input) {
    std::map<char, u_int8_t> suitLookup = {{'s', 0}, {'h', 1}, {'d', 2}, {'c', 3}};
    std::map<char, u_int8_t> cardLookup = {{'A', 0}, {'K', 1}, {'Q', 2}, {'J', 3}, {'T', 4}, {'9',5}, {'8',6}, {'7',7}, {'6',8}, {'5',9}, {'4',10}, {'3',11}, {'2',12}};
    std::vector<std::vector<u_int8_t>> cards;
    int boardIndex = input.find_first_of('|');
    int commaIndex = input.find_first_of(',');
    int length = input.length();

    std::vector<u_int8_t> board = {};
    if(boardIndex != std::string::npos) {
        for(int i = 0; i < boardIndex; i += (commaIndex + 1)) {
            std::vector<u_int8_t> playerCards = {};
            for(int j = i; j < i + (commaIndex - 1); j+=2) {
                playerCards.emplace_back(cardLookup[input[j]] * 4 + suitLookup[suitLookup[j+1]]);
            }
            cards.emplace_back(playerCards);
        }
        for(int i = boardIndex; i < length; i+=2) {
            board.emplace_back((cardLookup[input[i]] * 4) + suitLookup[suitLookup[i+1]]);
        }
    } else {
        for(int i = 0; i < length; i += (commaIndex + 1)) {
            std::vector<u_int8_t> playerCards = {};
            for(int j = i; j < i + (commaIndex - 1); j+=2) {
                playerCards.emplace_back(cardLookup[input[j]] * 4 + suitLookup[suitLookup[j+1]]);
            }
            cards.emplace_back(playerCards);
        }
    }

    std::set<uint8_t> deck = {};

    //code
    for(int i = 0; i < 52; i++) {
        deck.insert(i);
    }
    for(std::vector<uint8_t> tempHoles : cards){
        for(int card : tempHoles){
            deck.erase(card);
        }
    }

    std::vector<std::vector<int>> result;
    //uint8_t taco[deck.size()];
    std::vector<uint8_t> taco;
    taco.insert(taco.end(), deck.begin(), deck.end());
    //std::copy(deck.begin(), deck.end(), taco);
    const int size = deck.size();
    std::vector<std::vector<uint8_t>> boards = combinations(taco, (5 - board.size()));
    //std::vector<std::vector<uint8_t>> boards = for_each_combination(taco, taco + (5 - board.size()), taco + deck.size(), boardSet()).result;

    if(board.size() > 0) {
        for(std::vector<uint8_t> tempBoard : boards) {
            tempBoard.insert(tempBoard.end(), board.begin(), board.end());
        }
    }

    for(std::vector<uint8_t> tempBoard : boards) {
        result.emplace_back(getRanks(cards, tempBoard));
    }

    int numPlayers = cards.size();
    float results[numPlayers];
    memset(results, 0.0, sizeof(float) * numPlayers);

    std::vector<std::vector<int>> massFunctions(numPlayers,std::vector<int>(7464, 0));

    int maxRank;
    std::vector<int> maxIndexes;
    int indexSize;

    for(std::vector<int> ranks : result) {
        maxIndexes.clear();
        maxRank = *std::max_element(ranks.begin(), ranks.end());
        for(int i = 0; i < numPlayers; i++) {
            massFunctions[i][ranks[i]] += 1;
            if(ranks[i] == maxRank) {
                maxIndexes.emplace_back(i);
            }
        }

        indexSize = maxIndexes.size();
        for (int i = 0; i < indexSize; i++) {
            results[maxIndexes[i]] += (1.0 / (float) indexSize);
        }
    }
    std::vector<float> equities;
    for(int i = 0; i < numPlayers; i++) {
        equities.emplace_back(results[i] / (float)boards.size());
        std::cout << equities[i] << std::endl;
    }
    return std::pair<std::vector<float>, std::vector<std::vector<int>>>(equities, massFunctions);
}