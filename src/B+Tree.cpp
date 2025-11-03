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
void BPlusTree<T>::splitChild(BPlusNode<T> *parent, int index) {
    BPlusNode<T> *node = parent->children[index];
    int mid = node->keys.size() / 2;

    BPlusNode<T> *newNode = new BPlusNode<T>(order, node->isLeaf);

    if (!node->isLeaf) {
        newNode->keys.assign(node->keys.begin() + mid, node->keys.end());
        newNode->values.assign(node->values.begin() + mid, node->values.end());

        node->keys.resize(mid);
        node->values.resize(mid);

        //link leaf nodes

        newNode->next = node->next;
        node->next = newNode;

        //copy up first key
        parent->keys.insert(parent->keys.begin() + index, newNode->keys[0]);
    }
    else {
        // split node
        newNode->keys.assign(node->keys.begin() + mid + 1, node->keys.end());
        newNode->children.assign(node->children.begin() + mid + 1, node->children.end());

        int midKey = node->keys[mid];
        node->keys.resize(mid);
        node->children.resize(mid + 1);

        //push up middle key
        parent->keys.insert(parent->keys.begin() + index, midKey);
    }
    parent->children.insert(parent->children.begin() + index + 1, newNode);
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

template<typename T>
void BPlusTree<T>::searchNode(BPlusNode<T> *node, int key, vector<T *> &results) {
    if (node->isLeaf) {
        for (size_t i = 0; i < node->keys.size(); i++) {
            if (node->keys[i] == key) {
                results.push_back(node->values[i]);
            }
        }
    }
    else {
        int i = 0;
        while (i < node->keys.size() && key >= node->keys[i]) {
            i++;
        }
        searchNode(node->children[i], key, results);
    }
}

template<typename T>
BPlusNode<T> * BPlusTree<T>::findLeaf(BPlusNode<T> *node, int key) {
    if (node->isLeaf) {
        return node;
    }
    else {
        int i = 0;
        while (i < node->keys.size() && key >= node->keys[i]) {
            i++;
        }
        return findLeaf(node->children[i], key);
    }
}

template<typename T>
void BPlusTree<T>::printTree(BPlusNode<T> *node, int level) {
    if (node == nullptr) {
        return;
    }
    for (int i = 0; i < level; i++) {
        cout << " ";
    }
    if (node->isLeaf) {
        cout << "Leaf: ";
    }
    else {
        cout << "Internal: ";
    }
    for (int key : node->keys) {
        cout << key << " ";
    }
    cout << endl;

    if (!node->isLeaf) {
        for (auto child : node->children) {
            printTree(child, level + 1);
        }
    }
}

template<typename T>
void BPlusTree<T>::insert(int key, T *value) {
    BPlusNode<T> *r = root;

    //split root if full
    if (r->keys.size() >= order - 1) {
        BPlusNode<T> *newRoot = new BPlusNode<T>(order);
        newRoot->children.push_back(root);
        splitChild(newRoot, 0);
        root = newRoot;
    }
    insertNonFull(root, key, value);
}

template<typename T>
vector<T*> BPlusTree<T>::search(int key) {
    vector<T*> results;
    searchNode(root, key, results);
    return results;
}

template<typename T>
vector<T *> BPlusTree<T>::rangeSearch(int minKey, int maxKey) {
    vector<T *> results;
    BPlusNode<T> *leaf = findLeaf(root, minKey);
    while (leaf != nullptr) {
        for (size_t i = 0; i < leaf->keys.size(); i++) {
            if (leaf->keys[i] >= minKey && leaf->keys[i] <= maxKey) {
                results.push_back(leaf->values[i]);
            }
            else if (leaf->keys[i] > maxKey) {
                return results;
            }
        }
        leaf = leaf->next;
    }
    return results;
}

template<typename T>
void BPlusTree<T>::print() {
    printTree(root, 0);
}


