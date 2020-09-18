
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
#include "FiveEval.h"
#include "combinations/combinations.h"
#include "combination.h"

#ifndef PMF_SOLVE_H
#define PMF_SOLVE_H

#endif //PMF_SOLVE_H
FiveEval init_eval()
{
    return FiveEval();
}

extern FiveEval eval = init_eval();


std::vector<int> getRanks(std::vector<std::vector<uint8_t>> cards, std::vector<uint8_t> board) {
    std::vector<int> result;
    int numPlayers = cards.size();
    int handSize = cards[0].size();
    int tempMax, playerMax;
    if(handSize == 2) {
        for(int i = 0; i < numPlayers; i++) {
            playerMax = 0;
            for(int j = 0; j < 1; j++) {
                for(int k = j + 1; k < 2; k++) {
                    tempMax = SevenEval::GetRank(board[0], board[1], board[2], board[3], board[4], cards[i][j], cards[i][k]);
                    if(tempMax > playerMax) {
                        playerMax = tempMax;
                    }
                }
            }
            result.emplace_back(playerMax);
        }
    } else if(handSize == 4) {
        std::vector<std::vector<uint8_t>> bCombos = combination(board, 3);
        int comboLength = bCombos.size();
        for(int i = 0; i < numPlayers; i++) {
            playerMax = 0;
            for(int j = 0; j < comboLength; j++) {
                for (int k = 0; k < handSize - 1; k++) {
                    for (int l = k + 1; l < handSize; l++) {
                        tempMax = eval.GetRank(bCombos[j][0], bCombos[j][1], bCombos[j][2], cards[i][k], cards[i][l]);
                        if(tempMax > playerMax) {
                            playerMax = tempMax;
                        }
                    }
                }
            }
            result.emplace_back(playerMax);
        }
    } else {
        throw std::invalid_argument( "invalid number of cards" );
    }

    return result;
}

struct result {
    int numPlayers, boardCount;
    std::vector<float> results;
    std::vector<uint8_t> board;
    std::vector<std::vector<uint8_t>> cards;
    std::vector<std::vector<int>> massFunctions;

    result(int inNumPlayers, std::vector<std::vector<uint8_t>> inCards, std::vector<uint8_t> inBoard) {
        numPlayers = inNumPlayers;
        boardCount = 0;
        results = std::vector<float>(numPlayers);
        board = inBoard;
        cards = inCards;
        massFunctions = std::vector<std::vector<int>>(numPlayers, std::vector<int>(7463, 0));
    }

    bool operator()(uint8_t * first, uint8_t* last) {
        boardCount++;
        std::vector<uint8_t> combo = std::vector<uint8_t>(first, last);
        if (board.size() > 0) {
            combo.insert(combo.end(), board.begin(), board.end());
        }
        std::vector<int> ranks = getRanks(cards, combo);
        int maxRank = 0, maxCount;
        for (int i = 0; i < numPlayers; i++) {
            massFunctions[i][ranks[i]] += 1;
            if (ranks[i] >= maxRank) {
                if (ranks[i] == maxRank) {
                    maxCount++;
                } else {
                    maxRank = ranks[i];
                    maxCount = 1;
                }

            }
        }
        float addition = (1.0 / (float) maxCount);
        for (int i = 0; i < numPlayers; i++) {
            if (ranks[i] == maxRank) {
                results[i] += addition;
            }
        }
        return false;
    }
};


std::pair<std::vector<float>, std::vector<std::vector<int>>> buildPmf(std::vector<std::vector<uint8_t>> cards, std::vector<uint8_t> board, std::vector<uint8_t> removed) {
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

    std::set<uint8_t> deck = {};
    for(int i = 0; i < 52; i++) {
        deck.insert(i);
    }

    int numPlayers = cards.size();
    for(int i = 0; i < numPlayers; i++) {
        for(int j = 0; j < (cards[0].size()); j++) {
            deck.erase(cards[i][j]);
        }
    }

    int boardLength = board.size();
    for(int i = 0; i < boardLength; i++) {
        deck.erase(board[i]);
    }

    int removeLength = removed.size();
    for(int i = 0; i < removeLength; i++) {
        deck.erase(removed[i]);
    }

    uint8_t taco[deck.size()];
    std::copy(deck.begin(), deck.end(), taco);
    int N = 5 - boardLength;
    struct result results = for_each_combination(taco, taco + N, taco + deck.size(), result(numPlayers, cards, board));
    std::vector<float> equities;
    for(int i = 0; i < numPlayers; i++) {
        equities.emplace_back(results.results[i] / (float)results.boardCount);
    }

    //reslice array to contiguous sections 7068
    std::vector<std::vector<int>> finalPmf;
    for(int i = 0; i < numPlayers; i++) {
        finalPmf.emplace_back(std::vector<int>());
        finalPmf[i].insert(finalPmf[i].end(), &results.massFunctions[i][49], &results.massFunctions[i][1277]);
        finalPmf[i].insert(finalPmf[i].end(), &results.massFunctions[i][1296], &results.massFunctions[i][4137]);
        finalPmf[i].insert(finalPmf[i].end(), &results.massFunctions[i][4141], &results.massFunctions[i][4995]);
        finalPmf[i].insert(finalPmf[i].end(), &results.massFunctions[i][5004], &results.massFunctions[i][7140]);
        finalPmf[i].insert(finalPmf[i].end(), &results.massFunctions[i][7453], &results.massFunctions[i][7462]);
    }
    std::cout << "It took me " << (std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - t1).count()) << " seconds."  << std::endl;
    return std::pair<std::vector<float>, std::vector<std::vector<int>>>(equities, finalPmf);
}

std::tuple<std::vector<std::vector<uint8_t>>, std::vector<uint8_t>, std::vector<uint8_t>> parseCards(std::string input) {
    std::map<char, uint8_t> suitLookup = {{'s', 0}, {'S', 0}, {'h', 1}, {'H', 1}, {'d', 2}, {'D', 2}, {'c', 3}, {'C', 3}};
    std::map<char, uint8_t> cardLookup = {{'A', 0}, {'a', 0}, {'K', 1}, {'k', 1}, {'Q', 2}, {'q', 2}, {'J', 3}, {'j', 3}, {'T', 4}, {'t', 4}, {'9',5}, {'8',6}, {'7',7}, {'6',8}, {'5',9}, {'4',10}, {'3',11}, {'2',12}};
    std::vector<std::vector<uint8_t>> cards;
    int boardIndex = input.find_first_of('|');
    int handSize = input.find_first_of(',');
    int removedIndex = input.find_first_of(':');
    int length = input.length();
    std::vector<uint8_t> removed = {};
    std::vector<uint8_t> board = {};

    if(removedIndex != std::string::npos) {
        for(int i = removedIndex + 1; i < length; i+=2) {
            removed.emplace_back((cardLookup[input[i]] * 4) + suitLookup[input[i+1]]);
        }
    }
    int boardEnd = (removedIndex != std::string::npos) ? removedIndex : length;
    if(boardIndex != std::string::npos) {
        for(int i = boardIndex + 1; i < boardEnd; i+=2) {
            board.emplace_back((cardLookup[input[i]] * 4) + suitLookup[input[i+1]]);
        }
    }
    int cardEnd = (boardIndex != std::string::npos)? boardIndex : ((removedIndex != std::string::npos)? removedIndex: length);
    for(int i = 0; i < cardEnd; i += handSize + 1) {
        std::vector<uint8_t> playerCards = {};
        for(int j = i; j < i + handSize; j += 2) {
            playerCards.emplace_back(cardLookup[input[j]] * 4 + suitLookup[input[j+1]]);
        }
        cards.emplace_back(playerCards);
    }
    return std::tuple(cards, board, removed);
}

std::pair<std::vector<float>, std::vector<std::vector<int>>> solve(std::string input) {
    std::tuple<std::vector<std::vector<uint8_t>>, std::vector<uint8_t>, std::vector<uint8_t>> gameState = parseCards(input);
    return buildPmf(std::get<0>(gameState), std::get<1>(gameState), std::get<2>(gameState));
}

//for each combo of hole cards, remove them from the deck and iterate over all other combos of combos