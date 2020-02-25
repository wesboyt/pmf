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

std::vector<std::vector<uint16_t>> combinations(std::vector<uint16_t> src, int r) {
    std::vector<std::vector<uint16_t>> cs;
    if (r == 1) {
        for (auto i = 0; i < src.size(); i++) {
            std::vector<uint16_t> c;
            c.push_back(src[i]);
            cs.push_back(c);
        }
        return cs;
    }
    int* places = (int*)malloc(sizeof(int) * r);
    for (auto i = 0; i < r; i++) places[i] = i;
    while (true) {
        // push_back another combination
        std::vector<uint16_t> c; c.reserve(r);
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


std::vector<int> getRanks(std::vector<std::vector<uint16_t>> cards, std::vector<uint16_t> board) {
    std::vector<int> result;
    int numPlayers = cards.size();
    int handSize = cards[0].size();
    int tempMax, playerMax;
    for(int i = 0; i < numPlayers; i++) {
        playerMax = 0;
        for(int j = 0; j < handSize - 1; j++) {
            for(int k = j + 1; k < handSize; k++) {
                tempMax = SevenEval::GetRank(board[0], board[1], board[2], board[3], board[4], cards[i][j], cards[i][k]);
                if(tempMax > playerMax) {
                    playerMax = tempMax;
                }
            }

        }
        result.emplace_back(playerMax);
    }
    return result;
}

std::pair<std::vector<float>, std::vector<std::vector<int>>> solve(std::vector<std::vector<uint16_t>> cards, std::vector<uint16_t> board) {
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    std::set<uint16_t> deck = {};
    int handSize = cards[0].size();
    //code
    for(int i = 0; i < 52; i++) {
        deck.insert(i);
    }
    int numPlayers = cards.size();
    for(int i = 0; i < numPlayers; i++) {
        for(int j = 0; j < (handSize / 2); j++) {
            deck.erase(cards[i][j]);
        }
    }
    int boardLength = board.size();
    for(int i = 0; i < boardLength; i++) {
        deck.erase(board[i]);
    }

    std::vector<std::vector<int>> result;
    std::vector<uint16_t> taco;
    taco.insert(taco.end(), deck.begin(), deck.end());
    std::vector<std::vector<uint16_t>> boards = combinations(taco, (5 - boardLength));

    int boardsLength = boards.size();
    if(boardLength > 0) {
        for(int i = 0; i < boardsLength; i++) {
            boards[i].insert(boards[i].end(), board.begin(), board.end());
        }
    }

    for(std::vector<std::vector<uint16_t>>::iterator it = boards.begin(); it != boards.end(); ++it) {
        result.emplace_back(getRanks(cards, *it));
    }

    float results[numPlayers];
    memset(results, 0.0, sizeof(float) * numPlayers);
    std::vector<std::vector<int>> massFunctions(numPlayers,std::vector<int>(7463, 0));

    int maxRank;
    std::vector<int> maxIndexes;
    int indexSize;
    int rlength = result.size();

    for(int i = 0; i < rlength; i++) {
        maxIndexes.clear();
        maxRank = *std::max_element(result[i].begin(), result[i].end());
        for(int j = 0; j < numPlayers; j++) {
            massFunctions[j][result[i][j]] += 1;
            if(result[i][j] == maxRank) {
                maxIndexes.emplace_back(j);
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

    //reslice array to contiguous sections 4824
    std::vector<std::vector<int>> finalPmf;
    for(int i = 0; i < numPlayers; i++) {
        finalPmf.emplace_back(std::vector<int>());
        finalPmf[i].insert(finalPmf[i].end(), &massFunctions[i][49], &massFunctions[i][1277]);
        finalPmf[i].insert(finalPmf[i].end(), &massFunctions[i][1296], &massFunctions[i][4137]);
        finalPmf[i].insert(finalPmf[i].end(), &massFunctions[i][4141], &massFunctions[i][4995]);
        finalPmf[i].insert(finalPmf[i].end(), &massFunctions[i][5004], &massFunctions[i][7140]);
        finalPmf[i].insert(finalPmf[i].end(), &massFunctions[i][7453], &massFunctions[i][7462]);
    }

    std::cout << "It took me " << (std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - t1).count()) << " seconds."  << std::endl;
    return std::pair<std::vector<float>, std::vector<std::vector<int>>>(equities, finalPmf);
}

std::pair<std::vector<std::vector<uint16_t>>, std::vector<uint16_t>> parseCards(std::string input) {
    std::map<char, uint16_t> suitLookup = {{'s', 0}, {'h', 1}, {'d', 2}, {'c', 3}};
    std::map<char, uint16_t> cardLookup = {{'A', 0}, {'K', 1}, {'Q', 2}, {'J', 3}, {'T', 4}, {'9',5}, {'8',6}, {'7',7}, {'6',8}, {'5',9}, {'4',10}, {'3',11}, {'2',12}};
    std::vector<std::vector<uint16_t>> cards;
    int boardIndex = input.find_first_of('|');
    int handSize = input.find_first_of(',');
    int length = input.length();

    std::vector<uint16_t> board = {};
    if(boardIndex != std::string::npos) {
        for(int i = 0; i < boardIndex - 1; i += handSize + 1) {
            std::vector<uint16_t> playerCards = {};
            for(int j = i; j < i + handSize; j += 2) {
                playerCards.emplace_back(cardLookup[input[j]] * 4 + suitLookup[input[j+1]]);
            }
            cards.emplace_back(playerCards);
        }
        for(int i = boardIndex + 1; i < length; i+=2) {
            board.emplace_back((cardLookup[input[i]] * 4) + suitLookup[input[i+1]]);
        }
    } else {
        for(int i = 0; i < length; i += handSize + 1) {
            std::vector<uint16_t> playerCards = {};
            for(int j = i; j < i + handSize; j += 2) {
                playerCards.emplace_back(cardLookup[input[j]] * 4 + suitLookup[input[j+1]]);
            }
            cards.emplace_back(playerCards);
        }
    }
    return std::pair<std::vector<std::vector<uint16_t>>, std::vector<uint16_t>>(cards, board);
}