//
// Created by Raymond Adams on 11/2/25.
//

#include "B+Tree.h"
#include <iostream>

using namespace std;

template<typename T>
BPlusNode<T>::BPlusNode(int ord, bool leaf)
    : isLeaf(leaf), next(nullptr), order(ord) {}

template<typename T>
BPlusNode<T>::~BPlusNode() {
    if (!isLeaf) {
        for (auto child : children) {
            delete child;
        }
    }
}

