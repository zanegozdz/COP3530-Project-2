#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <B+Tree.h>

using namespace std;

int main(){
	BPlusTree<int> tree(4);

	// Insert some data
	int* value1 = new int(100);
	int* value2 = new int(200);
	int* value3 = new int(300);
	int* value4 = new int(400);

	tree.insert(10, value1);
	tree.insert(20, value2);
	tree.insert(30, value3);
	tree.insert(40, value4);

	// Search for a specific key
	vector<int*> results = tree.search(20);
	if (!results.empty()) {
		cout << "Found value: " << *results[0] << endl; // Output: Found value: 200
	}

	// Range search
	vector<int*> rangeResults = tree.rangeSearch(15, 35);
	cout << "Range search found " << rangeResults.size() << " results:" << endl;
	for (auto* result : rangeResults) {
		cout << *result << " ";
	}
	cout << endl; // Output: 200 300

	// Print tree structure
	cout << "Tree structure:" << endl;
	tree.print();

	//Clean up
	delete value1;
	delete value2;
	delete value3;
	delete value4;

	return 0;
}
