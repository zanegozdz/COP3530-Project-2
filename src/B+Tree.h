//
// Created by Raymond Adams on 11/2/25.
//

#ifndef PROJECT2_B_TREE_H
#define PROJECT2_B_TREE_H

#include <vector>
#include <iostream>
#include <string>

using namespace std;

template<typename K, typename V>
class BPlusTree;

//B+ tree node
template<typename K, typename V>
class BPlusNode {
public:
    bool isLeaf;
    vector<K> keys;
    vector<BPlusNode*> children;
    vector<V*> values;
    BPlusNode* next;
    int order;

    BPlusNode(int ord, bool leaf = false)
    : isLeaf(leaf), next(nullptr), order(ord) {}

    ~BPlusNode() {
        if (!isLeaf) {
            for (auto child : children) {
                delete child;
            }
        }
    }
    //friend class BPlusTree<T>;
};

//B+ tree
template<typename K, typename V>
class BPlusTree {
private:
    BPlusNode<K, V>* root;
    int order;

    //Private helper methods
    //     void splitChild(BPlusNode<T>* parent, int index);
    //     void insertNonFull(BPlusNode<T>* node, int key, T* value);
    //     void searchNode(BPlusNode<T>* node, int key, vector<T*>& results);
    //     BPlusNode<T>* findLeaf(BPlusNode<T>* node, int key);
    //     void printTree(BPlusNode<T>* node, int level);
    //
    // public:
    //     BPlusTree(int ord = 32);
    //     ~BPlusTree();
    //
    //     //search
    //     void insert(int key, T* value);
    //     vector<T*> search(int key);
    //     vector<T*> rangeSearch(int minKey, int maxKey);
    //     void print();

    void splitChild(BPlusNode<K, V> *parent, int index) {
        BPlusNode<K, V> *node = parent->children[index];
        int mid = node->keys.size() / 2;

        BPlusNode<K, V> *newNode = new BPlusNode<K, V>(order, node->isLeaf);

        if (node->isLeaf) {
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

            auto midKey = node->keys[mid];
            node->keys.resize(mid);
            node->children.resize(mid + 1);

            //push up middle key
            parent->keys.insert(parent->keys.begin() + index, midKey);
        }
        parent->children.insert(parent->children.begin() + index + 1, newNode);
    }

    void insertNonFull(BPlusNode<K, V> *node, K key, V *value) {
        if (node->isLeaf) {
            // Find insertion position
            int i = 0;
            while (i < node->keys.size() && key > node->keys[i]) {
                i++;
            }

            // Insert at position i
            node->keys.insert(node->keys.begin() + i, key);
            node->values.insert(node->values.begin() + i, value);
        } else {
            // Find child to insert into
            int i = 0;
            while (i < node->keys.size() && key >= node->keys[i]) {
                i++;
            }

            // Ensure child exists
            if (i >= node->children.size()) {
                node->children.push_back(new BPlusNode<K,V>(order, true));
            }

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

    void searchNode(BPlusNode<K, V> *node, K key, vector<V *> &results) {
        if (node == nullptr) return;

        if (node->isLeaf) {
            // Search leaf for all matching keys
            for (size_t i = 0; i < node->keys.size(); i++) {
                if (node->keys[i] == key) {
                    results.push_back(node->values[i]);
                }
            }
        } else {
            // search all children that might contain the key
            for (size_t i = 0; i < node->children.size(); i++) {
                bool mightContainKey = false;

                if (i == 0) {
                    mightContainKey = (node->keys.size() == 0 || key <= node->keys[0]);
                } else if (i < node->keys.size()) {
                    mightContainKey = (key >= node->keys[i-1] && key <= node->keys[i]);
                } else {
                    mightContainKey = (key >= node->keys[node->keys.size() - 1]);
                }

                if (mightContainKey) {
                    searchNode(node->children[i], key, results);
                }
            }
        }
    }

    BPlusNode<K, V> * findLeaf(BPlusNode<K, V> *node, K key) {
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

    void printTree(BPlusNode<K, V> *node, int level) {
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
        for (auto key : node->keys) {
            cout << key << " ";
        }
        cout << endl;

        if (!node->isLeaf) {
            for (auto child : node->children) {
                printTree(child, level + 1);
            }
        }
    }



public:
    BPlusTree(int ord = 32) : order(ord) {
        root = new BPlusNode<K, V>(order, true);
    }

    void insert(K key, V *value) {
        BPlusNode<K, V> *r = root;

        //split root if full
        if (r->keys.size() >= order - 1) {
            BPlusNode<K, V> *newRoot = new BPlusNode<K, V>(order);
            newRoot->children.push_back(root);
            splitChild(newRoot, 0);
            root = newRoot;
        }
        insertNonFull(root, key, value);
    }

    vector<V*> search(K key) {
        vector<V*> results;
        searchNode(root, key, results);
        return results;
    }

    vector<V *> rangeSearch(int minKey, int maxKey) {
        vector<V *> results;
        BPlusNode<K, V> *leaf = findLeaf(root, minKey);
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

    void print() {
        printTree(root, 0);
    }
};

#endif //PROJECT2_B_TREE_H