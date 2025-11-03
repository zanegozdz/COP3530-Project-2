#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <vector>
#include <string>

#include "TestData.h"

using namespace std;

TEST_CASE("Insert - Single Element", "[insert]") {
    TestBPlusTree tree(4);
    TestData* data = new TestData(1, "First");

    tree.insert(10, data);
    std::vector<TestData*> results = tree.search(10);

    REQUIRE(results.size() == 1);
    REQUIRE(results[0]->id == 1);
    REQUIRE(results[0]->name == "First");

    delete data;
}

TEST_CASE("Insert - Multiple Elements", "[insert]") {
    TestBPlusTree tree(4);
    TestData* data1 = new TestData(1, "First");
    TestData* data2 = new TestData(2, "Second");
    TestData* data3 = new TestData(3, "Third");

    tree.insert(10, data1);
    tree.insert(20, data2);
    tree.insert(30, data3);

    REQUIRE(tree.search(10).size() == 1);
    REQUIRE(tree.search(20).size() == 1);
    REQUIRE(tree.search(30).size() == 1);

    delete data1;
    delete data2;
    delete data3;
}

TEST_CASE("Insert - Out of Order", "[insert]") {
    TestBPlusTree tree(4);
    TestData* data1 = new TestData(1, "First");
    TestData* data2 = new TestData(2, "Second");
    TestData* data3 = new TestData(3, "Third");

    SECTION("Insert elements in random order") {
        tree.insert(30, data3);
        tree.insert(10, data1);
        tree.insert(20, data2);

        // Should still find all elements
        std::vector<TestData*> results1 = tree.search(10);
        REQUIRE(results1.size() == 1);
        REQUIRE(results1[0]->name == "First");

        std::vector<TestData*> results2 = tree.search(20);
        REQUIRE(results2.size() == 1);
        REQUIRE(results2[0]->name == "Second");

        std::vector<TestData*> results3 = tree.search(30);
        REQUIRE(results3.size() == 1);
        REQUIRE(results3[0]->name == "Third");
    }

    delete data1;
    delete data2;
    delete data3;
}

TEST_CASE("Insert - Duplicate Keys", "[insert]") {
    TestBPlusTree tree(4);
    TestData* data1 = new TestData(1, "First");
    TestData* data2 = new TestData(2, "Second");

    SECTION("Insert multiple elements with same key") {
        tree.insert(10, data1);
        tree.insert(10, data2);

        std::vector<TestData*> results = tree.search(10);

        REQUIRE(results.size() == 2);
        // Both should be present
        bool foundFirst = false;
        bool foundSecond = false;
        for (auto* result : results) {
            if (result->name == "First") foundFirst = true;
            if (result->name == "Second") foundSecond = true;
        }
        REQUIRE(foundFirst);
        REQUIRE(foundSecond);
    }

    delete data1;
    delete data2;
}

TEST_CASE("Insert - Causes Split", "[insert][split]") {
    TestBPlusTree tree(3);  // Order 3: max 2 keys per node
    TestData* data1 = new TestData(1, "A");
    TestData* data2 = new TestData(2, "B");
    TestData* data3 = new TestData(3, "C");
    TestData* data4 = new TestData(4, "D");

    SECTION("Insert enough elements to force a split") {
        tree.insert(10, data1);
        tree.insert(20, data2);
        tree.insert(30, data3);  // Should cause split
        tree.insert(40, data4);

        // All elements should still be searchable
        REQUIRE(tree.search(10).size() == 1);
        REQUIRE(tree.search(20).size() == 1);
        REQUIRE(tree.search(30).size() == 1);
        REQUIRE(tree.search(40).size() == 1);

        REQUIRE(tree.search(10)[0]->name == "A");
        REQUIRE(tree.search(20)[0]->name == "B");
        REQUIRE(tree.search(30)[0]->name == "C");
        REQUIRE(tree.search(40)[0]->name == "D");
    }

    delete data1;
    delete data2;
    delete data3;
    delete data4;
}

TEST_CASE("Insert - Large Number of Elements", "[insert]") {
    TestBPlusTree tree(4);
    std::vector<TestData*> allData;

    SECTION("Insert 100 elements") {
        for (int i = 0; i < 100; i++) {
            TestData* data = new TestData(i, "Data" + std::to_string(i));
            allData.push_back(data);
            tree.insert(i * 10, data);
        }

        // Verify all elements can be found
        for (int i = 0; i < 100; i++) {
            std::vector<TestData*> results = tree.search(i * 10);
            REQUIRE(results.size() == 1);
            REQUIRE(results[0]->id == i);
        }

        // Search for non-existent key
        std::vector<TestData*> notFound = tree.search(999);
        REQUIRE(notFound.size() == 0);
    }

    // Cleanup
    for (auto* data : allData) {
        delete data;
    }
}

TEST_CASE("Insert - Negative Keys", "[insert]") {
    TestBPlusTree tree(4);
    TestData* data1 = new TestData(1, "Negative");
    TestData* data2 = new TestData(2, "Zero");
    TestData* data3 = new TestData(3, "Positive");

    SECTION("Insert with negative, zero, and positive keys") {
        tree.insert(-10, data1);
        tree.insert(0, data2);
        tree.insert(10, data3);

        REQUIRE(tree.search(-10).size() == 1);
        REQUIRE(tree.search(0).size() == 1);
        REQUIRE(tree.search(10).size() == 1);

        REQUIRE(tree.search(-10)[0]->name == "Negative");
        REQUIRE(tree.search(0)[0]->name == "Zero");
        REQUIRE(tree.search(10)[0]->name == "Positive");
    }

    delete data1;
    delete data2;
    delete data3;
}

TEST_CASE("Insert - Range Search After Insert", "[insert][range]") {
    TestBPlusTree tree(4);
    std::vector<TestData*> allData;

    SECTION("Insert and perform range search") {
        for (int i = 1; i <= 10; i++) {
            TestData* data = new TestData(i, "Data" + std::to_string(i));
            allData.push_back(data);
            tree.insert(i * 10, data);
        }

        // Range search from 25 to 65
        std::vector<TestData*> results = tree.rangeSearch(25, 65);

        // Should find keys: 30, 40, 50, 60
        REQUIRE(results.size() == 4);

        // Verify the results
        std::vector<int> expectedIds = {3, 4, 5, 6};
        for (size_t i = 0; i < results.size(); i++) {
            bool found = false;
            for (int expectedId : expectedIds) {
                if (results[i]->id == expectedId) {
                    found = true;
                    break;
                }
            }
            REQUIRE(found);
        }
    }

    // Cleanup
    for (auto* data : allData) {
        delete data;
    }
}

TEST_CASE("Insert - Search Non-Existent Key", "[insert][search]") {
    TestBPlusTree tree(4);
    TestData* data1 = new TestData(1, "First");
    TestData* data2 = new TestData(2, "Second");

    SECTION("Search for key that doesn't exist") {
        tree.insert(10, data1);
        tree.insert(30, data2);

        std::vector<TestData*> results = tree.search(20);
        REQUIRE(results.size() == 0);
    }

    delete data1;
    delete data2;
}

TEST_CASE("Insert - Multiple Splits", "[insert][split]") {
    TestBPlusTree tree(3);  // Small order to force multiple splits
    std::vector<TestData*> allData;

    SECTION("Insert 20 elements to cause multiple splits") {
        for (int i = 1; i <= 20; i++) {
            TestData* data = new TestData(i, "Item" + std::to_string(i));
            allData.push_back(data);
            tree.insert(i, data);
        }

        // Verify all can be found
        for (int i = 1; i <= 20; i++) {
            std::vector<TestData*> results = tree.search(i);
            REQUIRE(results.size() == 1);
            REQUIRE(results[0]->id == i);
        }
    }

    // Cleanup
    for (auto* data : allData) {
        delete data;
    }
}

TEST_CASE("splitChild correctly splits a leaf node", "[BPlusTree][splitChild][leaf]") {
    TestBPlusTree tree(4);

    // parent node and one child leaf
    TestBPlusNode* parent = new TestBPlusNode(4, false);
    TestBPlusNode* child = new TestBPlusNode(4, true);

    // Fill child
    for (int i = 1; i <= 4; i++) {
        child->keys.push_back(i * 10);
        child->values.push_back(new TestData(i, "val" + std::to_string(i)));
    }

    parent->children.push_back(child);

    // Call splitChild
    tree.splitChild(parent, 0);

    REQUIRE(parent->keys.size() == 1);
    REQUIRE(parent->keys[0] == 30); // newNode->keys[0]

    REQUIRE(parent->children.size() == 2);
    TestBPlusNode* left = parent->children[0];
    TestBPlusNode* right = parent->children[1];

    // Left half
    REQUIRE(left->keys.size() == 2);
    REQUIRE(left->keys[0] == 10);
    REQUIRE(left->keys[1] == 20);

    // Right half
    REQUIRE(right->keys.size() == 2);
    REQUIRE(right->keys[0] == 30);
    REQUIRE(right->keys[1] == 40);

    // Check next pointer
    REQUIRE(left->next == right);
    REQUIRE(right->next == nullptr);

    delete parent; // destructor cleans up children
}

TEST_CASE("splitChild correctly splits an internal node", "[BPlusTree][splitChild][internal]") {
    TestBPlusTree tree(4); // order 4 = max 4 children, max 3 keys

    // Parent and one internal child
    TestBPlusNode* parent = new TestBPlusNode(4, false);
    TestBPlusNode* child = new TestBPlusNode(4, false);

    // Fill child to capacity (3 keys, 4 children for internal node)
    for (int i = 1; i <= 3; i++) {
        child->keys.push_back(i * 10); // keys: [10, 20, 30]
    }
    for (int i = 0; i < 4; i++) {
        child->children.push_back(new TestBPlusNode(4, true));
    }

    parent->children.push_back(child);

    // Call splitChild
    tree.splitChild(parent, 0);

    REQUIRE(parent->keys.size() == 1);
    REQUIRE(parent->keys[0] == 20); // middle key promoted

    REQUIRE(parent->children.size() == 2);
    TestBPlusNode* left = parent->children[0];
    TestBPlusNode* right = parent->children[1];

    // Left node should get first half of keys
    REQUIRE(left->keys.size() == 1);
    REQUIRE(left->keys[0] == 10);
    REQUIRE(left->children.size() == 2); // keys + 1

    // Right node should get remaining keys
    REQUIRE(right->keys.size() == 1);
    REQUIRE(right->keys[0] == 30);
    REQUIRE(right->children.size() == 2); // keys + 1

    // Cleanup
    delete parent;
}

TEST_CASE("splitChild for leaf nodes", "[BPlusTree][splitChild][leaf]") {
    TestBPlusTree tree(4); // order 4

    SECTION("split leaf node with exact capacity") {
        TestBPlusNode* parent = new TestBPlusNode(4, false);
        TestBPlusNode* leaf = new TestBPlusNode(4, true);

        // Fill leaf to capacity (order-1 = 3 keys)
        for (int i = 1; i <= 3; i++) {
            leaf->keys.push_back(i * 10);
            leaf->values.push_back(new TestData(i * 10, "data" + std::to_string(i * 10)));
        }

        parent->children.push_back(leaf);

        tree.splitChild(parent, 0);

        REQUIRE(parent->keys.size() == 1);
        REQUIRE(parent->keys[0] == 20); // First key of new node

        REQUIRE(parent->children.size() == 2);

        TestBPlusNode* left = parent->children[0];
        TestBPlusNode* right = parent->children[1];

        // Left node gets first half
        REQUIRE(left->keys.size() == 1);
        REQUIRE(left->keys[0] == 10);
        REQUIRE(left->values.size() == 1);

        // Right node gets second half
        REQUIRE(right->keys.size() == 2);
        REQUIRE(right->keys[0] == 20);
        REQUIRE(right->keys[1] == 30);
        REQUIRE(right->values.size() == 2);

        // Verify linked list connection
        REQUIRE(left->next == right);
        REQUIRE(right->next == nullptr);

        delete parent;
    }

    SECTION("split leaf node with more than capacity") {
        TestBPlusNode* parent = new TestBPlusNode(4, false);
        TestBPlusNode* leaf = new TestBPlusNode(4, true);

        // Overfill leaf (4 keys when max is 3)
        for (int i = 1; i <= 4; i++) {
            leaf->keys.push_back(i * 5);
            leaf->values.push_back(new TestData(i * 5, "data" + std::to_string(i * 5)));
        }

        parent->children.push_back(leaf);

        tree.splitChild(parent, 0);

        REQUIRE(parent->keys.size() == 1);
        REQUIRE(parent->keys[0] == 15); // First key of new node (keys: 5,10,15,20 -> split at 10)

        TestBPlusNode* left = parent->children[0];
        TestBPlusNode* right = parent->children[1];

        REQUIRE(left->keys.size() == 2); // 5,10
        REQUIRE(left->keys[0] == 5);
        REQUIRE(left->keys[1] == 10);

        REQUIRE(right->keys.size() == 2); // 15,20
        REQUIRE(right->keys[0] == 15);
        REQUIRE(right->keys[1] == 20);

        delete parent;
    }
}

TEST_CASE("splitChild for internal nodes", "[BPlusTree][splitChild][internal]") {
    TestBPlusTree tree(4); // order 4

    SECTION("split internal node with exact capacity") {
        TestBPlusNode* parent = new TestBPlusNode(4, false);
        TestBPlusNode* internal = new TestBPlusNode(4, false);

        // Fill internal node to capacity (order-1 = 3 keys, 4 children)
        internal->keys = {10, 20, 30};
        for (int i = 0; i < 4; i++) {
            internal->children.push_back(new TestBPlusNode(4, true));
        }

        parent->children.push_back(internal);

        tree.splitChild(parent, 0);

        REQUIRE(parent->keys.size() == 1);
        REQUIRE(parent->keys[0] == 20); // Middle key promoted (keys: 10,20,30 -> mid index = 1)

        REQUIRE(parent->children.size() == 2);

        TestBPlusNode* left = parent->children[0];
        TestBPlusNode* right = parent->children[1];

        // Left gets keys before middle (index 0)
        REQUIRE(left->keys.size() == 1);
        REQUIRE(left->keys[0] == 10);
        REQUIRE(left->children.size() == 2);

        // Right gets keys after middle (indices 2+)
        REQUIRE(right->keys.size() == 1);
        REQUIRE(right->keys[0] == 30);
        REQUIRE(right->children.size() == 2);

        delete parent;
    }

    SECTION("split internal node with 4 keys") {
        TestBPlusNode* parent = new TestBPlusNode(4, false);
        TestBPlusNode* internal = new TestBPlusNode(4, false);

        // 4 keys, 5 children (overfull for order 4)
        internal->keys = {5, 15, 25, 35};
        for (int i = 0; i < 5; i++) {
            internal->children.push_back(new TestBPlusNode(4, true));
        }

        parent->children.push_back(internal);
        tree.splitChild(parent, 0);

        REQUIRE(parent->keys.size() == 1);
        // With 4 keys, mid = 4/2 = 2, so key at index 2 (25) is promoted
        REQUIRE(parent->keys[0] == 25);

        TestBPlusNode* left = parent->children[0];
        TestBPlusNode* right = parent->children[1];

        REQUIRE(left->keys.size() == 2); // 5,15
        REQUIRE(left->keys[0] == 5);
        REQUIRE(left->keys[1] == 15);
        REQUIRE(left->children.size() == 3);

        REQUIRE(right->keys.size() == 1); // 35
        REQUIRE(right->keys[0] == 35);
        REQUIRE(right->children.size() == 2);

        delete parent;
    }
}

TEST_CASE("splitChild boundary cases", "[BPlusTree][splitChild][boundary]") {
    SECTION("minimum possible split") {
        TestBPlusTree tree(2); // Minimum order; useless test case since we have 270,000 data points

        TestBPlusNode* parent = new TestBPlusNode(2, false);
        TestBPlusNode* leaf = new TestBPlusNode(2, true);

        // Order 2: max 1 key, so 2 keys triggers split
        leaf->keys.push_back(10);
        leaf->values.push_back(new TestData(10, "data10"));
        leaf->keys.push_back(20);
        leaf->values.push_back(new TestData(20, "data20"));

        parent->children.push_back(leaf);
        tree.splitChild(parent, 0);

        REQUIRE(parent->keys.size() == 1);
        REQUIRE(parent->keys[0] == 20); // First key of right node

        REQUIRE(parent->children[0]->keys.size() == 1);
        REQUIRE(parent->children[0]->keys[0] == 10);
        REQUIRE(parent->children[1]->keys.size() == 1);
        REQUIRE(parent->children[1]->keys[0] == 20);

        delete parent;
    }
}
