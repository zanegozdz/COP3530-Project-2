#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

#include "B+Tree.h"
#include "TestData.h"

TEST_CASE("B+ Tree Real - Insert") {
    BPlusTree<int, string> tree(4);
    string data1 = "First";
    string data2 = "Second";
    string data3 = "Third";
    string data4 = "Fourth";
    string data5 = "Fifth";

    SECTION("Insert Single Element") {
        tree.insert(10, &data1);
        auto result = tree.search(10);
        REQUIRE(result.size() == 1);
        REQUIRE(result[0] == &data1);
    }

}


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

TEST_CASE("searchNode functionality", "[BPlusTree][searchNode]") {
    TestBPlusTree tree(4);

    SECTION("search in empty leaf node") {
        TestBPlusNode* leaf = new TestBPlusNode(4, true);
        std::vector<TestData*> results;

        tree.searchNode(leaf, 10, results);

        REQUIRE(results.empty());
        delete leaf;
    }

    SECTION("search in leaf node with single key - found") {
        TestBPlusNode* leaf = new TestBPlusNode(4, true);
        leaf->keys = {15};
        leaf->values = {new TestData(15, "data15")};
        std::vector<TestData*> results;

        tree.searchNode(leaf, 15, results);

        REQUIRE(results.size() == 1);
        REQUIRE(results[0]->id == 15);
        REQUIRE(results[0]->name == "data15");
        delete leaf;
    }

    SECTION("search in leaf node with single key - not found") {
        TestBPlusNode* leaf = new TestBPlusNode(4, true);
        leaf->keys = {15};
        leaf->values = {new TestData(15, "data15")};
        std::vector<TestData*> results;

        tree.searchNode(leaf, 20, results);

        REQUIRE(results.empty());
        delete leaf;
    }

    SECTION("search in leaf node with multiple keys - found") {
        TestBPlusNode* leaf = new TestBPlusNode(4, true);
        leaf->keys = {10, 20, 30, 40};
        leaf->values = {
            new TestData(10, "data10"),
            new TestData(20, "data20"),
            new TestData(30, "data30"),
            new TestData(40, "data40")
        };
        std::vector<TestData*> results;

        tree.searchNode(leaf, 20, results);

        REQUIRE(results.size() == 1);
        REQUIRE(results[0]->id == 20);
        REQUIRE(results[0]->name == "data20");
        delete leaf;
    }

    SECTION("search in leaf node with duplicate keys") {
        TestBPlusNode* leaf = new TestBPlusNode(4, true);
        leaf->keys = {10, 10, 20, 30};
        leaf->values = {
            new TestData(10, "data10a"),
            new TestData(10, "data10b"),
            new TestData(20, "data20"),
            new TestData(30, "data30")
        };
        std::vector<TestData*> results;

        tree.searchNode(leaf, 10, results);

        REQUIRE(results.size() == 2);
        REQUIRE(results[0]->id == 10);
        REQUIRE(results[0]->name == "data10a");
        REQUIRE(results[1]->id == 10);
        REQUIRE(results[1]->name == "data10b");
        delete leaf;
    }

    SECTION("search in internal node - key less than all internal keys") {
        TestBPlusNode* internal = new TestBPlusNode(4, false);
        internal->keys = {20, 40, 60};

        // Create leaf child
        TestBPlusNode* leaf1 = new TestBPlusNode(4, true);
        leaf1->keys = {5, 10, 15};
        leaf1->values = {
            new TestData(5, "data5"),
            new TestData(10, "data10"),
            new TestData(15, "data15")
        };

        internal->children = {leaf1, nullptr, nullptr, nullptr};

        std::vector<TestData*> results;

        // Search for key 10 (less than first internal key 20)
        tree.searchNode(internal, 10, results);

        REQUIRE(results.size() == 1);
        REQUIRE(results[0]->id == 10);

        delete internal;
    }

    SECTION("search in internal node - key between first and second internal keys") {
        TestBPlusNode* internal = new TestBPlusNode(4, false);
        internal->keys = {20, 40, 60};

        // Create leaf child
        TestBPlusNode* leaf2 = new TestBPlusNode(4, true);
        leaf2->keys = {25, 30, 35};
        leaf2->values = {
            new TestData(25, "data25"),
            new TestData(30, "data30"),
            new TestData(35, "data35")
        };

        internal->children = {nullptr, leaf2, nullptr, nullptr};

        std::vector<TestData*> results;

        // Search for key 30 (between 20 and 40)
        tree.searchNode(internal, 30, results);

        REQUIRE(results.size() == 1);
        REQUIRE(results[0]->id == 30);

        delete internal;
    }

    SECTION("search in internal node - key equals internal key") {
        TestBPlusNode* internal = new TestBPlusNode(4, false);
        internal->keys = {20, 40, 60};

        // Create leaf child - when key equals internal key, it goes to next child
        TestBPlusNode* leaf3 = new TestBPlusNode(4, true);
        leaf3->keys = {40, 45, 50};
        leaf3->values = {
            new TestData(40, "data40"),
            new TestData(45, "data45"),
            new TestData(50, "data50")
        };

        internal->children = {nullptr, nullptr, leaf3, nullptr};

        std::vector<TestData*> results;

        // Search for key 40, equals second internal key
        tree.searchNode(internal, 40, results);

        REQUIRE(results.size() == 1);
        REQUIRE(results[0]->id == 40);

        delete internal;
    }

    SECTION("search in internal node - key greater than all internal keys") {
        TestBPlusNode* internal = new TestBPlusNode(4, false);
        internal->keys = {20, 40, 60};

        // Create leaf child
        TestBPlusNode* leaf4 = new TestBPlusNode(4, true);
        leaf4->keys = {65, 70, 75};
        leaf4->values = {
            new TestData(65, "data65"),
            new TestData(70, "data70"),
            new TestData(75, "data75")
        };

        internal->children = {nullptr, nullptr, nullptr, leaf4};

        std::vector<TestData*> results;

        // Search for key 70, greater than last internal key 60
        tree.searchNode(internal, 70, results);

        REQUIRE(results.size() == 1);
        REQUIRE(results[0]->id == 70);

        delete internal;
    }

    SECTION("search in two-level tree") {
        // Build root internal node
        TestBPlusNode* root = new TestBPlusNode(4, false);
        root->keys = {30};

        // Build left child (internal node)
        TestBPlusNode* leftInternal = new TestBPlusNode(4, false);
        leftInternal->keys = {15};

        TestBPlusNode* leaf1 = new TestBPlusNode(4, true);
        leaf1->keys = {5, 10};
        leaf1->values = {new TestData(5, "data5"), new TestData(10, "data10")};

        TestBPlusNode* leaf2 = new TestBPlusNode(4, true);
        leaf2->keys = {20, 25};
        leaf2->values = {new TestData(20, "data20"), new TestData(25, "data25")};

        leftInternal->children = {leaf1, leaf2};

        // Build right child (leaf node directly)
        TestBPlusNode* leaf3 = new TestBPlusNode(4, true);
        leaf3->keys = {35, 40, 45};
        leaf3->values = {
            new TestData(35, "data35"),
            new TestData(40, "data40"),
            new TestData(45, "data45")
        };

        root->children = {leftInternal, leaf3};

        std::vector<TestData*> results;

        // Test searches at different levels
        tree.searchNode(root, 10, results); // Through leftInternal -> leaf1
        REQUIRE(results.size() == 1);
        REQUIRE(results[0]->id == 10);

        results.clear();
        tree.searchNode(root, 25, results); // Through leftInternal -> leaf2
        REQUIRE(results.size() == 1);
        REQUIRE(results[0]->id == 25);

        results.clear();
        tree.searchNode(root, 40, results); // Directly to leaf3
        REQUIRE(results.size() == 1);
        REQUIRE(results[0]->id == 40);

        results.clear();
        tree.searchNode(root, 100, results); // Not found
        REQUIRE(results.empty());

        delete root;
    }
}

TEST_CASE("searchNode edge cases", "[BPlusTree][searchNode][edge]") {
    TestBPlusTree tree(4);

    SECTION("search with keys not sorted in leaf") {
        TestBPlusNode* leaf = new TestBPlusNode(4, true);
        // B+Tree should maintain sorted order, but test the search logic, useless hopefully
        leaf->keys = {30, 10, 20};  // Unsorted
        leaf->values = {
            new TestData(30, "data30"),
            new TestData(10, "data10"),
            new TestData(20, "data20")
        };
        std::vector<TestData*> results;

        // Search should still work if key exists
        tree.searchNode(leaf, 20, results);
        REQUIRE(results.size() == 1);
        REQUIRE(results[0]->id == 20);

        delete leaf;
    }

    SECTION("search in internal node with single key") {
        TestBPlusNode* internal = new TestBPlusNode(4, false);
        internal->keys = {25};

        TestBPlusNode* leaf1 = new TestBPlusNode(4, true);
        leaf1->keys = {10, 15, 20};
        leaf1->values = {
            new TestData(10, "data10"),
            new TestData(15, "data15"),
            new TestData(20, "data20")
        };

        TestBPlusNode* leaf2 = new TestBPlusNode(4, true);
        leaf2->keys = {30, 35, 40};
        leaf2->values = {
            new TestData(30, "data30"),
            new TestData(35, "data35"),
            new TestData(40, "data40")
        };

        internal->children = {leaf1, leaf2};

        std::vector<TestData*> results;

        tree.searchNode(internal, 15, results); // Goes to leaf1
        REQUIRE(results.size() == 1);
        REQUIRE(results[0]->id == 15);

        results.clear();
        tree.searchNode(internal, 35, results); // Goes to leaf2
        REQUIRE(results.size() == 1);
        REQUIRE(results[0]->id == 35);

        delete internal;
    }

    SECTION("search accumulates results in existing vector") {
        TestBPlusNode* leaf = new TestBPlusNode(4, true);
        leaf->keys = {10, 20, 30};
        leaf->values = {
            new TestData(10, "data10"),
            new TestData(20, "data20"),
            new TestData(30, "data30")
        };

        // Start with existing results
        std::vector<TestData*> results;
        results.push_back(new TestData(5, "existing"));

        tree.searchNode(leaf, 20, results);

        REQUIRE(results.size() == 2);
        REQUIRE(results[0]->id == 5);
        REQUIRE(results[1]->id == 20);

        // Cleanup
        for (auto* data : results) delete data;
        delete leaf;
    }
}

TEST_CASE("findLeaf functionality", "[BPlusTree][findLeaf]") {
    TestBPlusTree tree(4);

    SECTION("findLeaf in single leaf tree") {
        TestBPlusNode* leaf = new TestBPlusNode(4, true);
        leaf->keys = {10, 20, 30};

        TestBPlusNode* result = tree.findLeaf(leaf, 15);

        REQUIRE(result == leaf);
        REQUIRE(result->isLeaf == true);

        delete leaf;
    }

    SECTION("findLeaf in two-level tree - key less than root key") {
        // Build: root[30] -> leaves [10,20] and [40,50]
        TestBPlusNode* root = new TestBPlusNode(4, false);
        root->keys = {30};

        TestBPlusNode* leftLeaf = new TestBPlusNode(4, true);
        leftLeaf->keys = {10, 20};

        TestBPlusNode* rightLeaf = new TestBPlusNode(4, true);
        rightLeaf->keys = {40, 50};

        root->children = {leftLeaf, rightLeaf};

        // Test keys that should go to left leaf
        REQUIRE(tree.findLeaf(root, 5) == leftLeaf);   // < all keys
        REQUIRE(tree.findLeaf(root, 10) == leftLeaf);  // == first key
        REQUIRE(tree.findLeaf(root, 15) == leftLeaf);  // between keys
        REQUIRE(tree.findLeaf(root, 20) == leftLeaf);  // == last key
        REQUIRE(tree.findLeaf(root, 25) == leftLeaf);  // < root key

        delete root;
    }

    SECTION("findLeaf in two-level tree - key greater than or equal to root key") {
        // Build: root[30] -> leaves [10,20] and [40,50]
        TestBPlusNode* root = new TestBPlusNode(4, false);
        root->keys = {30};

        TestBPlusNode* leftLeaf = new TestBPlusNode(4, true);
        leftLeaf->keys = {10, 20};

        TestBPlusNode* rightLeaf = new TestBPlusNode(4, true);
        rightLeaf->keys = {40, 50};

        root->children = {leftLeaf, rightLeaf};

        // Test keys that should go to right leaf
        REQUIRE(tree.findLeaf(root, 30) == rightLeaf);  // == root key
        REQUIRE(tree.findLeaf(root, 35) == rightLeaf);  // > root key
        REQUIRE(tree.findLeaf(root, 40) == rightLeaf);  // == first key in right
        REQUIRE(tree.findLeaf(root, 45) == rightLeaf);  // between keys in right
        REQUIRE(tree.findLeaf(root, 50) == rightLeaf);  // == last key in right
        REQUIRE(tree.findLeaf(root, 60) == rightLeaf);  // > all keys

        delete root;
    }

    SECTION("findLeaf in three-level tree") {

        TestBPlusNode* root = new TestBPlusNode(4, false);
        root->keys = {50};

        // First internal node with keys 20, 40
        TestBPlusNode* internal1 = new TestBPlusNode(4, false);
        internal1->keys = {20, 40};

        TestBPlusNode* leaf1 = new TestBPlusNode(4, true);
        leaf1->keys = {10, 15};

        TestBPlusNode* leaf2 = new TestBPlusNode(4, true);
        leaf2->keys = {25, 30, 35};

        TestBPlusNode* leaf3 = new TestBPlusNode(4, true);
        leaf3->keys = {45};

        internal1->children = {leaf1, leaf2, leaf3};

        // Second internal node with key 70
        TestBPlusNode* internal2 = new TestBPlusNode(4, false);
        internal2->keys = {70};

        TestBPlusNode* leaf4 = new TestBPlusNode(4, true);
        leaf4->keys = {55, 60, 65};

        TestBPlusNode* leaf5 = new TestBPlusNode(4, true);
        leaf5->keys = {75, 80, 85};

        internal2->children = {leaf4, leaf5};

        root->children = {internal1, internal2};

        // Test different search paths
        REQUIRE(tree.findLeaf(root, 10) == leaf1);   // root->internal1->leaf1
        REQUIRE(tree.findLeaf(root, 15) == leaf1);   // root->internal1->leaf1
        REQUIRE(tree.findLeaf(root, 25) == leaf2);   // root->internal1->leaf2
        REQUIRE(tree.findLeaf(root, 35) == leaf2);   // root->internal1->leaf2
        REQUIRE(tree.findLeaf(root, 45) == leaf3);   // root->internal1->leaf3
        REQUIRE(tree.findLeaf(root, 55) == leaf4);   // root->internal2->leaf4
        REQUIRE(tree.findLeaf(root, 65) == leaf4);   // root->internal2->leaf4
        REQUIRE(tree.findLeaf(root, 75) == leaf5);   // root->internal2->leaf5
        REQUIRE(tree.findLeaf(root, 85) == leaf5);   // root->internal2->leaf5

        delete root;
    }

    SECTION("findLeaf with multiple keys in root") {
        // Build: root[20, 40, 60] -> 4 leaves
        TestBPlusNode* root = new TestBPlusNode(4, false);
        root->keys = {20, 40, 60};

        TestBPlusNode* leaf1 = new TestBPlusNode(4, true);
        leaf1->keys = {5, 10, 15};

        TestBPlusNode* leaf2 = new TestBPlusNode(4, true);
        leaf2->keys = {25, 30, 35};

        TestBPlusNode* leaf3 = new TestBPlusNode(4, true);
        leaf3->keys = {45, 50, 55};

        TestBPlusNode* leaf4 = new TestBPlusNode(4, true);
        leaf4->keys = {65, 70, 75};

        root->children = {leaf1, leaf2, leaf3, leaf4};

        // Test boundary cases around each internal key
        REQUIRE(tree.findLeaf(root, 5) == leaf1);    // < 20
        REQUIRE(tree.findLeaf(root, 15) == leaf1);   // < 20
        REQUIRE(tree.findLeaf(root, 20) == leaf2);   // == 20, goes to next child
        REQUIRE(tree.findLeaf(root, 25) == leaf2);   // between 20-40
        REQUIRE(tree.findLeaf(root, 35) == leaf2);   // between 20-40
        REQUIRE(tree.findLeaf(root, 40) == leaf3);   // == 40,  goes to next child
        REQUIRE(tree.findLeaf(root, 45) == leaf3);   // between 40-60
        REQUIRE(tree.findLeaf(root, 55) == leaf3);   // between 40-60
        REQUIRE(tree.findLeaf(root, 60) == leaf4);   // == 60, goes to next child
        REQUIRE(tree.findLeaf(root, 65) == leaf4);   // > 60
        REQUIRE(tree.findLeaf(root, 75) == leaf4);   // > 60

        delete root;
    }

    SECTION("findLeaf with duplicate searches returns same leaf") {
        TestBPlusNode* root = new TestBPlusNode(4, false);
        root->keys = {30};

        TestBPlusNode* leftLeaf = new TestBPlusNode(4, true);
        leftLeaf->keys = {10, 20};

        TestBPlusNode* rightLeaf = new TestBPlusNode(4, true);
        rightLeaf->keys = {40, 50};

        root->children = {leftLeaf, rightLeaf};

        // Multiple searches for same key should return same leaf
        TestBPlusNode* result1 = tree.findLeaf(root, 15);
        TestBPlusNode* result2 = tree.findLeaf(root, 15);
        TestBPlusNode* result3 = tree.findLeaf(root, 15);

        REQUIRE(result1 == leftLeaf);
        REQUIRE(result2 == leftLeaf);
        REQUIRE(result3 == leftLeaf);
        REQUIRE(result1 == result2);
        REQUIRE(result2 == result3);

        delete root;
    }
}

TEST_CASE("findLeaf edge cases", "[BPlusTree][findLeaf][edge]") {
    TestBPlusTree tree(4);

    SECTION("findLeaf with negative keys") {
        TestBPlusNode* root = new TestBPlusNode(4, false);
        root->keys = {0};

        TestBPlusNode* leftLeaf = new TestBPlusNode(4, true);
        leftLeaf->keys = {-20, -10};

        TestBPlusNode* rightLeaf = new TestBPlusNode(4, true);
        rightLeaf->keys = {10, 20};

        root->children = {leftLeaf, rightLeaf};

        REQUIRE(tree.findLeaf(root, -30) == leftLeaf); //shouldn't have negative values?
        REQUIRE(tree.findLeaf(root, -20) == leftLeaf);
        REQUIRE(tree.findLeaf(root, -15) == leftLeaf);
        REQUIRE(tree.findLeaf(root, -10) == leftLeaf);
        REQUIRE(tree.findLeaf(root, 0) == rightLeaf);
        REQUIRE(tree.findLeaf(root, 10) == rightLeaf);

        delete root;
    }

    SECTION("findLeaf with zero keys in internal nodes") {
        // Internal node with key 0
        TestBPlusNode* root = new TestBPlusNode(4, false);
        root->keys = {0};

        TestBPlusNode* leftLeaf = new TestBPlusNode(4, true);
        leftLeaf->keys = {-5, -3};

        TestBPlusNode* rightLeaf = new TestBPlusNode(4, true);
        rightLeaf->keys = {2, 5};

        root->children = {leftLeaf, rightLeaf};

        REQUIRE(tree.findLeaf(root, -4) == leftLeaf);
        REQUIRE(tree.findLeaf(root, 0) == rightLeaf);
        REQUIRE(tree.findLeaf(root, 1) == rightLeaf);

        delete root;
    }

    SECTION("findLeaf in minimum order tree") {
        TestBPlusTree minTree(2); // order 2

        TestBPlusNode* root = new TestBPlusNode(2, false);
        root->keys = {50};

        TestBPlusNode* leftLeaf = new TestBPlusNode(2, true);
        leftLeaf->keys = {10}; // max 1 key for order 2

        TestBPlusNode* rightLeaf = new TestBPlusNode(2, true);
        rightLeaf->keys = {60}; // max 1 key for order 2

        root->children = {leftLeaf, rightLeaf};

        REQUIRE(minTree.findLeaf(root, 5) == leftLeaf);
        REQUIRE(minTree.findLeaf(root, 10) == leftLeaf);
        REQUIRE(minTree.findLeaf(root, 50) == rightLeaf);
        REQUIRE(minTree.findLeaf(root, 60) == rightLeaf);

        delete root;
    }

    SECTION("findLeaf with keys at exact boundaries") {
        TestBPlusNode* root = new TestBPlusNode(4, false);
        root->keys = {10, 20, 30};

        TestBPlusNode* leaf1 = new TestBPlusNode(4, true);
        leaf1->keys = {5};

        TestBPlusNode* leaf2 = new TestBPlusNode(4, true);
        leaf2->keys = {15};

        TestBPlusNode* leaf3 = new TestBPlusNode(4, true);
        leaf3->keys = {25};

        TestBPlusNode* leaf4 = new TestBPlusNode(4, true);
        leaf4->keys = {35};

        root->children = {leaf1, leaf2, leaf3, leaf4};

        // Test exact key matches with internal nodes
        REQUIRE(tree.findLeaf(root, 10) == leaf2);  // == first internal key
        REQUIRE(tree.findLeaf(root, 20) == leaf3);  // == second internal key
        REQUIRE(tree.findLeaf(root, 30) == leaf4);  // == third internal key

        delete root;
    }

    SECTION("findLeaf returns actual leaf node") {
        TestBPlusNode* root = new TestBPlusNode(4, false);
        root->keys = {25};

        TestBPlusNode* leftLeaf = new TestBPlusNode(4, true);
        leftLeaf->keys = {10, 20};

        TestBPlusNode* rightLeaf = new TestBPlusNode(4, true);
        rightLeaf->keys = {30, 40};

        root->children = {leftLeaf, rightLeaf};

        TestBPlusNode* result = tree.findLeaf(root, 15);

        REQUIRE(result->isLeaf == true);
        REQUIRE(result == leftLeaf);
        REQUIRE(result->keys.size() == 2);
        REQUIRE(result->keys[0] == 10);
        REQUIRE(result->keys[1] == 20);

        delete root;
    }

    SECTION("findLeaf with empty tree (just root leaf)") {
        TestBPlusNode* rootLeaf = new TestBPlusNode(4, true);
        rootLeaf->keys = {5, 10, 15};

        // Searching in a tree where root is already a leaf
        REQUIRE(tree.findLeaf(rootLeaf, 5) == rootLeaf);
        REQUIRE(tree.findLeaf(rootLeaf, 10) == rootLeaf);
        REQUIRE(tree.findLeaf(rootLeaf, 15) == rootLeaf);
        REQUIRE(tree.findLeaf(rootLeaf, 20) == rootLeaf); // Not found, but still returns leaf

        delete rootLeaf;
    }

    SECTION("findLeaf with large key values") {
        TestBPlusNode* root = new TestBPlusNode(4, false);
        root->keys = {1000, 2000, 3000};

        TestBPlusNode* leaf1 = new TestBPlusNode(4, true);
        leaf1->keys = {100, 200, 300};

        TestBPlusNode* leaf2 = new TestBPlusNode(4, true);
        leaf2->keys = {1500, 1600, 1700};

        TestBPlusNode* leaf3 = new TestBPlusNode(4, true);
        leaf3->keys = {2500, 2600, 2700};

        TestBPlusNode* leaf4 = new TestBPlusNode(4, true);
        leaf4->keys = {3500, 3600, 3700};

        root->children = {leaf1, leaf2, leaf3, leaf4};

        REQUIRE(tree.findLeaf(root, 500) == leaf1);
        REQUIRE(tree.findLeaf(root, 1000) == leaf2);
        REQUIRE(tree.findLeaf(root, 1550) == leaf2);
        REQUIRE(tree.findLeaf(root, 2000) == leaf3);
        REQUIRE(tree.findLeaf(root, 2550) == leaf3);
        REQUIRE(tree.findLeaf(root, 3000) == leaf4);
        REQUIRE(tree.findLeaf(root, 3550) == leaf4);

        delete root;
    }
}

TEST_CASE("rangeSearch: edge cases and multi-leaf traversal", "[BPlusTree][rangeSearch]") {
    TestBPlusTree tree(4);

    SECTION("Empty tree") {
        auto results = tree.rangeSearch(10, 20);
        REQUIRE(results.empty());
    }

    SECTION("Single leaf, range inside keys") {
        TestBPlusNode* leaf = new TestBPlusNode(4, true);
        leaf->keys = {10, 20, 30};
        leaf->values = {
            new TestData(10, "A"),
            new TestData(20, "B"),
            new TestData(30, "C")
        };
        leaf->next = nullptr;

        delete tree.root;
        tree.root = leaf;

        auto results = tree.rangeSearch(15, 25);
        REQUIRE(results.size() == 1);
        REQUIRE(results[0]->id == 20);

        results = tree.rangeSearch(5, 35);
        REQUIRE(results.size() == 3);
    }

    SECTION("Multiple linked leaves") {
        // two linked leaves
        TestBPlusNode* leaf1 = new TestBPlusNode(4, true);
        TestBPlusNode* leaf2 = new TestBPlusNode(4, true);

        leaf1->keys = {10, 20};
        leaf1->values = {new TestData(10, "A"), new TestData(20, "B")};
        leaf2->keys = {30, 40};
        leaf2->values = {new TestData(30, "C"), new TestData(40, "D")};

        leaf1->next = leaf2;
        leaf2->next = nullptr;

        delete tree.root;
        tree.root = leaf1;

        // Range for both leaves
        auto results = tree.rangeSearch(15, 40);
        REQUIRE(results.size() == 3);
        REQUIRE(results[0]->id == 20);
        REQUIRE(results[1]->id == 30);
        REQUIRE(results[2]->id == 40);  // boundary check

        // Range that matches all keys
        results = tree.rangeSearch(0, 50);
        REQUIRE(results.size() == 4);

        // Range below all keys
        results = tree.rangeSearch(0, 5);
        REQUIRE(results.empty());

        // Range above all keys
        results = tree.rangeSearch(45, 50);
        REQUIRE(results.empty());
    }

    SECTION("Partial overlap with second leaf") {
        // Linked leaves with gaps
        TestBPlusNode* leaf1 = new TestBPlusNode(4, true);
        TestBPlusNode* leaf2 = new TestBPlusNode(4, true);

        leaf1->keys = {10, 20};
        leaf1->values = {new TestData(10, "A"), new TestData(20, "B")};
        leaf2->keys = {30, 40};
        leaf2->values = {new TestData(30, "C"), new TestData(40, "D")};

        leaf1->next = leaf2;
        leaf2->next = nullptr;

        delete tree.root;
        tree.root = leaf1;

        // Range starts in first leaf and ends in second
        auto results = tree.rangeSearch(15, 40);
        REQUIRE(results.size() == 3);
        REQUIRE(results[0]->id == 20);
        REQUIRE(results[1]->id == 30);
        REQUIRE(results[2]->id == 40); // still included because <= maxKey
    }

    SECTION("Range entirely within second leaf") {
        TestBPlusNode* leaf1 = new TestBPlusNode(4, true);
        TestBPlusNode* leaf2 = new TestBPlusNode(4, true);

        leaf1->keys = {10, 20};
        leaf1->values = {new TestData(10, "A"), new TestData(20, "B")};
        leaf2->keys = {30, 40};
        leaf2->values = {new TestData(30, "C"), new TestData(40, "D")};

        leaf1->next = leaf2;
        leaf2->next = nullptr;

        delete tree.root;
        tree.root = leaf1;

        auto results = tree.rangeSearch(35, 40);
        REQUIRE(results.size() == 1);
        REQUIRE(results[0]->id == 40);
    }

    SECTION("Range completely outside tree") {
        TestBPlusNode* leaf = new TestBPlusNode(4, true);
        leaf->keys = {10, 20, 30};
        leaf->values = {new TestData(10,"A"), new TestData(20,"B"), new TestData(30,"C")};
        leaf->next = nullptr;

        delete tree.root;
        tree.root = leaf;

        auto results = tree.rangeSearch(100, 200);
        REQUIRE(results.empty());

        results = tree.rangeSearch(-50, 5);
        REQUIRE(results.empty());
    }
}