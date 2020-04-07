//
// Created by wes on 4/6/20.
//

#include <vector>

#ifndef PMF_GRAPH_H
#define PMF_GRAPH_H

#endif //PMF_GRAPH_H

struct Node {
    int size;
    Node* source;
    std::vector<float> likelihoods;
    std::vector<Node> children;
};



std::vector<float> holeLikelihoods(std::vector<float> actionRepresentation) {
    return std::vector<float>(1326, 0.00075414781);
}

std::vector<float> raiseLikelihoods(std::vector<float> actionRepresentation) {
    return std::vector<float>(1001, 0.00099900099);
}
