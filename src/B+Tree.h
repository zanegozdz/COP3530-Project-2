//
// Created by Raymond Adams on 11/2/25.
//

#ifndef PROJECT2_B_TREE_H
#define PROJECT2_B_TREE_H

#include <vector>
#include <string>

using namespace std;

template<typename T>
class BPlusTree;

//B+ tree node
template<typename T>
class BPlusNode {
private:
    bool isLeaf;
    vector<int> keys;
    vector<BPlusNode*> children;
    vector<T*> values;
    BPlusNode* next;
    int order;

    friend class BPlusTree<T>;

public:
    BPlusNode(int ord, bool leaf = false);
    ~BPlusNode();
};

//B+ tree
template<typename T>
class BPlusTree {
private:
    BPlusNode<T>* root;
    int order;

    //Private helper methods
    void splitChild(BPlusNode<T>* parent, int index);
    void insertNonFull(BPlusNode<T>* node, int key, T* value);
    void searchNode(BPlusNode<T>* node, int key, vector<T*>& results);
    BPlusNode<T>* findLeaf(BPlusNode<T>* node, int key);
    void printTree(BPlusNode<T>* node, int level);

public:
    BPlusTree(int ord = 32);
    ~BPlusTree();

    //search
    void insert(int key, T* value);
    vector<T*> search(int key);
    vector<T*> rangeSearch(int minKey, int maxKey);
    void print();
};

#endif //PROJECT2_B_TREE_H