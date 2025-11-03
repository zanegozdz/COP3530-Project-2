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

template<typename T>
void BPlusTree<T>::insertNonFull(BPlusNode<T> *node, int key, T *value) {
    if (node->isLeaf) {
        int i = node->keys.size() - 1;
        node->keys.push_back(0);
        node->values.push_back(nullptr);

        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            node->values[i + 1] = node->values[i];
            i--;
        }

        node->keys[i + 1] = key;
        node->values[i + 1] = value;
    } else {
        // Find child to insert into
        int i = node->keys.size() - 1;
        while (i >= 0 && key < node->keys[i]) {
            i--;
        }
        i++;

        // Split child if full
        if (node->children[i]->keys.size() >= order - 1) {
            splitChild(node, i);
            if (key > node->keys[i]) {
                i++;
            }
        }

        insertNonFull(node->children[i], key, value);

    }
}