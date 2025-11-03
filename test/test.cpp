#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <vector>
#include <string>

#include "TestData.h"

using namespace std;

TEST_CASE("B+ Tree Insert - Single Element", "[insert]") {
    TestBPlusTree tree(4);
    TestData* data = new TestData(1, "First");

    tree.insert(10, data);
    std::vector<TestData*> results = tree.search(10);

    REQUIRE(results.size() == 1);
    REQUIRE(results[0]->id == 1);
    REQUIRE(results[0]->name == "First");

    delete data;
}

TEST_CASE("B+ Tree Insert - Multiple Elements", "[insert]") {
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

TEST_CASE("B+ Tree Insert - Out of Order", "[insert]") {
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

TEST_CASE("B+ Tree Insert - Duplicate Keys", "[insert]") {
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

TEST_CASE("B+ Tree Insert - Causes Split", "[insert][split]") {
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

TEST_CASE("B+ Tree Insert - Large Number of Elements", "[insert]") {
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

TEST_CASE("B+ Tree Insert - Negative Keys", "[insert]") {
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

TEST_CASE("B+ Tree Insert - Range Search After Insert", "[insert][range]") {
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

TEST_CASE("B+ Tree Insert - Search Non-Existent Key", "[insert][search]") {
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

TEST_CASE("B+ Tree Insert - Multiple Splits", "[insert][split]") {
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