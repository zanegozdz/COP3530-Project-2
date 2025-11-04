//
// Created by Raymond Adams on 11/3/25.
//

#ifndef PROJECT2_TESTDATA_H
#define PROJECT2_TESTDATA_H

#include <vector>
#include <string>

struct TestData {
    int id;
    std::string name;

    TestData(int i, std::string n) : id(i), name(n) {}
};

// Non-template B+ Tree Node for TestData
class TestBPlusNode {
public:
    bool isLeaf;
    std::vector<int> keys;
    std::vector<TestBPlusNode*> children;
    std::vector<TestData*> values;
    TestBPlusNode* next;
    int order;

    TestBPlusNode(int ord, bool leaf = false);
    ~TestBPlusNode();
};

// Non-template B+ Tree for TestData
class TestBPlusTree {
public:
    TestBPlusNode* root;
    int order;

    void splitChild(TestBPlusNode* parent, int index);
    void insertNonFull(TestBPlusNode* node, int key, TestData* value);
    void searchNode(TestBPlusNode* node, int key, std::vector<TestData*>& results);
    TestBPlusNode* findLeaf(TestBPlusNode* node, int key);

    TestBPlusTree(int ord = 4);
    ~TestBPlusTree();

    void insert(int key, TestData* value);
    std::vector<TestData*> search(int key);
    std::vector<TestData*> rangeSearch(int minKey, int maxKey);
};


// TestBPlusNode Implementation
TestBPlusNode::TestBPlusNode(int ord, bool leaf)
    : isLeaf(leaf), next(nullptr), order(ord) {}

TestBPlusNode::~TestBPlusNode() {
    if (!isLeaf) {
        for (auto child : children) {
            delete child;
        }
    }
}

// TestBPlusTree Implementation
TestBPlusTree::TestBPlusTree(int ord) : order(ord) {
    root = new TestBPlusNode(order, true);
}

TestBPlusTree::~TestBPlusTree() {
    delete root;
}

void TestBPlusTree::splitChild(TestBPlusNode* parent, int index) {
    TestBPlusNode* node = parent->children[index];
    int mid = node->keys.size() / 2;

    TestBPlusNode* newNode = new TestBPlusNode(order, node->isLeaf);

    if (node->isLeaf) {
        newNode->keys.assign(node->keys.begin() + mid, node->keys.end());
        newNode->values.assign(node->values.begin() + mid, node->values.end());

        node->keys.resize(mid);
        node->values.resize(mid);

        newNode->next = node->next;
        node->next = newNode;

        parent->keys.insert(parent->keys.begin() + index, newNode->keys[0]);
    } else {
        newNode->keys.assign(node->keys.begin() + mid + 1, node->keys.end());
        newNode->children.assign(node->children.begin() + mid + 1, node->children.end());

        int midKey = node->keys[mid];
        node->keys.resize(mid);
        node->children.resize(mid + 1);

        parent->keys.insert(parent->keys.begin() + index, midKey);
    }

    parent->children.insert(parent->children.begin() + index + 1, newNode);
}

void TestBPlusTree::insertNonFull(TestBPlusNode* node, int key, TestData* value) {
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
        int i = node->keys.size() - 1;
        while (i >= 0 && key < node->keys[i]) {
            i--;
        }
        i++;

        if (node->children[i]->keys.size() >= order - 1) {
            splitChild(node, i);
            if (key > node->keys[i]) {
                i++;
            }
        }

        insertNonFull(node->children[i], key, value);
    }
}

void TestBPlusTree::insert(int key, TestData* value) {
    TestBPlusNode* r = root;

    if (r->keys.size() >= order - 1) {
        TestBPlusNode* newRoot = new TestBPlusNode(order);
        newRoot->children.push_back(root);
        splitChild(newRoot, 0);
        root = newRoot;
    }

    insertNonFull(root, key, value);
}

void TestBPlusTree::searchNode(TestBPlusNode* node, int key, std::vector<TestData*>& results) {
    if (node->isLeaf) {
        for (size_t i = 0; i < node->keys.size(); i++) {
            if (node->keys[i] == key) {
                results.push_back(node->values[i]);
            }
        }
    } else {
        int i = 0;
        while (i < node->keys.size() && key >= node->keys[i]) {
            i++;
        }
        searchNode(node->children[i], key, results);
    }
}

std::vector<TestData*> TestBPlusTree::search(int key) {
    std::vector<TestData*> results;
    searchNode(root, key, results);
    return results;
}

TestBPlusNode* TestBPlusTree::findLeaf(TestBPlusNode* node, int key) {
    if (node->isLeaf) return node;

    int i = 0;
    while (i < node->keys.size() && key >= node->keys[i]) {
        i++;
    }
    return findLeaf(node->children[i], key);
}

std::vector<TestData*> TestBPlusTree::rangeSearch(int minKey, int maxKey) {
    std::vector<TestData*> results;
    TestBPlusNode* leaf = findLeaf(root, minKey);

    while (leaf != nullptr) {
        for (size_t i = 0; i < leaf->keys.size(); i++) {
            if (leaf->keys[i] >= minKey && leaf->keys[i] <= maxKey) {
                results.push_back(leaf->values[i]);
            } else if (leaf->keys[i] > maxKey) {
                return results;
            }
        }
        leaf = leaf->next;
    }
    return results;
}


#endif //PROJECT2_TESTDATA_H