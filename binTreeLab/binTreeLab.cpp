#include <iostream>
#include <string>
#include <chrono>
#include <sstream>
#include <fstream>
#include <deque>
#include <vector>
#include <list>
#include <stdexcept>
#include <cstdlib>
#include <filesystem>

class BinarySearchTree 
{
public:
	struct TreeNode 
	{
		int value;
		TreeNode* left;
		TreeNode* right;

		explicit TreeNode(const int val, TreeNode* left, TreeNode* right) : value(val), left(left), right(right) {}

		explicit TreeNode(const int val) : value(val), left(nullptr), right(nullptr) {}
	};

private:
	TreeNode* head;

	std::string lineString(int value, int level) 
	{
		std::string res = "\n";
		std::string filler = "   |";
		for (int i = 0; i < level; ++i) 
			res += filler;
		res += "-->";
		res = res + std::to_string(value);
		return res;
	}

	std::string fullViewRecursive(TreeNode* node, int level = 0)
	{
		std::string res;
		if (node == nullptr) throw std::invalid_argument("Can't print from empty node. This probably means that binary tree is empty");
		res += lineString(node->value, level);
		if (node->left != nullptr) res = fullViewRecursive(node->left, level + 1) + res;
		if (node->right != nullptr) res += fullViewRecursive(node->right, level+1);
		return res;
	}

	void deleteNodesAllRecursive(TreeNode* node) 
	{
		if (node == nullptr) return;
		if (node->left != nullptr) deleteNodesAllRecursive(node->left);
		if (node->right != nullptr) deleteNodesAllRecursive(node->right);
		delete node;
	}

	TreeNode* findNode(const int value, TreeNode*& parent) 
	{
		TreeNode* ptr = head;
		while (ptr != nullptr && ptr->value != value ) 
		{
			if (ptr->value < value) ptr = ptr->right;
			else ptr = ptr->left;
		}
		return ptr;
	}

	TreeNode* minValueNode(TreeNode* root, TreeNode*& parent)
	{
		while (root != nullptr && root->left != nullptr)
		{
			parent = root;
			root = root->left;
		}

		return root;
	}

	void clearNode(TreeNode*& node, TreeNode*& parent) 
	{
		if (parent!= nullptr && parent->left == node) {
			parent->left = nullptr;
		}
		if (parent!= nullptr && parent->right == node) {
			parent->right = nullptr;
		}
		delete node;
	}

	bool deleteNode(TreeNode* root, TreeNode* parent, int value)
	{
		if (root == nullptr) return false;

		if (value > root->value)
			return deleteNode(root->left, root, value);
		else if (value < root->value)
			return deleteNode(root->right, root, value);
		else 
		{
			if (root->left == nullptr && root->right == nullptr) 
			{
				if (head == root) clearNode(head, parent);
				else clearNode(root, parent);
				return true;
			}
			else if (root->left == nullptr) 
			{
				TreeNode* temp = root->right;

				clearNode(root, parent);

				if (parent == nullptr) head = temp;
				else if (parent->value > temp->value)
					parent->right = temp;
				else 
					parent->left = temp;

				return true;
			}
			else if (root->right == nullptr) 
			{
				TreeNode* temp = root->left;

				clearNode(root, parent);

				if (parent == nullptr) head = temp;
				else if (parent->value > temp->value) 
					parent->right = temp;
				else 
					parent->left = temp;

				return true;
			}
			
			TreeNode* tempParent = root;
			TreeNode* temp = minValueNode(root->right, tempParent);
			root->value = temp->value;
			clearNode(temp, tempParent);
		}
	}

	std::string DFS_recursive(TreeNode* node) 
	{
		if (node == nullptr) return "";
		
		std::string res = std::to_string(node->value) + " ";
		if (node->left != nullptr) res += DFS_recursive(node->left);
		if (node->right != nullptr) res += DFS_recursive(node->right);

		return res;
	}

	std::string RDFS_recursive(TreeNode* node)
	{
		if (node == nullptr) return "";
		
		std::string res;
		if (node->left != nullptr) res += RDFS_recursive(node->left);
		if (node->right != nullptr) res += RDFS_recursive(node->right);
		res += std::to_string(node->value) + " ";

		return res;
	}

	std::string BFS_fromNode(TreeNode* node) 
	{
		std::string res;

		std::deque<TreeNode*> nodesToProcess;
		nodesToProcess.push_back(node);

		do {
			TreeNode* ptr = nodesToProcess.front();
			res += std::to_string(ptr->value) + " ";
			nodesToProcess.pop_front();

			if (ptr->left != nullptr) nodesToProcess.push_back(ptr->left);
			if (ptr->right != nullptr) nodesToProcess.push_back(ptr->right);
		} while (!nodesToProcess.empty());

		return res;
	}

public:
	void insert(const int item)
	{
		TreeNode* currIter = head;
		TreeNode* parent = nullptr;

		while (currIter != nullptr)
		{
			parent = currIter;
			if (currIter->value <= item) currIter = currIter->left;
			else currIter = currIter->right;
		}

		if (parent == nullptr) head = new TreeNode(item);
		else if (parent->value <= item) parent->left = new TreeNode(item);
		else parent->right = new TreeNode(item);
	}

	bool remove(const int item)
	{
		return deleteNode(head, nullptr, item);
	}

	bool remove_subtree(const int item)
	{
		TreeNode* parent = nullptr;
		auto node = findNode(item, parent);
		deleteNodesAllRecursive(node);
		return true;
	}

	bool remove_parent(const int item)
	{
		TreeNode* parent = nullptr;
		auto node = findNode(item, parent);
		remove(parent->value);
		return true;
	}

	bool contains(const int item) 
	{
		TreeNode* parent = nullptr;
		return findNode(item, parent) != nullptr;
	}

	std::string serialize()
	{
		if (head == nullptr) return "\nBinary tree is empty";
		return fullViewRecursive(head);
	}

	~BinarySearchTree() 
	{
		deleteNodesAllRecursive(head);
	}

	std::string DFS() 
	{
		return DFS_recursive(head);
	}

	std::string BFS() 
	{
		return BFS_fromNode(head);
	}

	std::string RDFS() 
	{
		return RDFS_recursive(head);
	}
};

BinarySearchTree tree;
std::list<int> list;
std::vector<int> dynamicArray;

constexpr auto FILL_RANDOM = "random";
constexpr auto FILL_CONSOLE = "console";
constexpr auto FILL_FILE = "file";
constexpr auto DELETE = "delete";
constexpr auto INSERT = "insert";
constexpr auto FIND = "find";
constexpr auto PRINT = "print";
constexpr auto DEPTH_SEARCH = "dfs";
constexpr auto BREADTH_SEARCH = "bfs";
constexpr auto REVERSE_SEARCH = "rdfs";
constexpr auto GENERATE_TEST = "rdfs";

constexpr auto EXIT = "exit";
constexpr auto HELP = "help";
constexpr auto ABOUT = "about";

inline void askFor(const char* item)
{
	std::cout << "\n" << item << " : ";
}

inline void printLine(const char* item)
{
	std::cout << "\n" << item;
}
void infoEndpoint()
{
	std::cout << "\nThis lab is about binary search tree";
	std::cout << "\nUser should be able to perform CRD operations";
	std::cout << "\nUser should also see benchmarks to compare to linear structures";
	std::cout << "\nThis program stores integer numbers in this binary tree";
	std::cout << "\nTo view all available operations, print " << HELP;
}

inline void printCommandDescription(const char* commandName, const char* desc)
{
	std::cout << "\n" << commandName << " - " << desc;
}

void helpEndpoint()
{
	std::cout << "\nCommands:";
	printCommandDescription(FILL_RANDOM, "Fill structure of fixed size N with random values");
	printCommandDescription(FILL_CONSOLE, "Fill structure with values from console");
	printCommandDescription(FILL_FILE, "Fill structure with values from file");
	printCommandDescription(INSERT, "Insert element with specified value into tree");
	printCommandDescription(DELETE, "Delete first occurrence of element with specified value");
	printCommandDescription(FIND, "Check if value exists in structure");
	printCommandDescription(PRINT, "Print binary search tree in console");
	printCommandDescription(BREADTH_SEARCH, "Print binary search tree in console using breadth search");
	printCommandDescription(DEPTH_SEARCH, "Print binary search tree in console using depth search");
	printCommandDescription(REVERSE_SEARCH, "Print binary search tree in console using reverse depth search");

	printCommandDescription(ABOUT, "View info about this program");
	printCommandDescription(EXIT, "Stop program execution");
}

void exitEndpoint()
{
	std::cout << "\nProgram execution stopped";
}

void fillRandomEndpoint()
{
	int amount;
	askFor("amount of random items");
	std::cin >> amount;

	long long treeTimeTotal = 0, listTimeTotal = 0, arrayTimeTotal = 0;
	for (int i = 0; i < amount; ++i) 
	{
		int num = std::rand() % 199 - 99;
		{
			const auto startedTime = std::chrono::steady_clock::now();
			tree.insert(num);
			const auto endedTime = std::chrono::steady_clock::now();

			treeTimeTotal += std::chrono::duration_cast<std::chrono::nanoseconds>(endedTime - startedTime).count();
		}
		{
			const auto startedTime = std::chrono::steady_clock::now();
			list.push_back(num);
			const auto endedTime = std::chrono::steady_clock::now();

			listTimeTotal += std::chrono::duration_cast<std::chrono::nanoseconds>(endedTime - startedTime).count();
		}
		{
			const auto startedTime = std::chrono::steady_clock::now();
			dynamicArray.push_back(num);
			const auto endedTime = std::chrono::steady_clock::now();

			arrayTimeTotal += std::chrono::duration_cast<std::chrono::nanoseconds>(endedTime - startedTime).count();
		}
	}

	std::cout << "\nTree was filled with " << amount << " random values";

	std::cout << "\nBinary tree time: " << treeTimeTotal;
	std::cout << "\nList time: " << listTimeTotal;
	std::cout << "\nDynamic array time: " << arrayTimeTotal;
}
void fillFileEndpoint() 
{
	std::cout << "\nSpefify a filename to read numbers from";
	std::cout << "\nThis file should contain an array of numbers";
	askFor("file name");
	std::string fileName;
	std::cin >> fileName;

	std::ifstream in(fileName);

	int num;
	long long treeTimeTotal = 0, listTimeTotal = 0, arrayTimeTotal = 0;
	while (in >> num) 
	{
		{
			const auto startedTime = std::chrono::steady_clock::now();
			tree.insert(num);
			const auto endedTime = std::chrono::steady_clock::now();

			treeTimeTotal += std::chrono::duration_cast<std::chrono::nanoseconds>(endedTime - startedTime).count();
		}
		{
			const auto startedTime = std::chrono::steady_clock::now();
			list.push_back(num);
			const auto endedTime = std::chrono::steady_clock::now();

			listTimeTotal += std::chrono::duration_cast<std::chrono::nanoseconds>(endedTime - startedTime).count();
		}
		{
			const auto startedTime = std::chrono::steady_clock::now();
			dynamicArray.push_back(num);
			const auto endedTime = std::chrono::steady_clock::now();

			arrayTimeTotal += std::chrono::duration_cast<std::chrono::nanoseconds>(endedTime - startedTime).count();
		}
	}

	std::cout << "\nBinary tree time: " << treeTimeTotal;
	std::cout << "\nList time: " << listTimeTotal;
	std::cout << "\nDynamic array time: " << arrayTimeTotal;
}

void fillEndpoint()
{
	std::cout << "\nPrint number to insert into binary search tree";
	std::cout << "\nTo stop this, enter a non-number value";
	askFor("number");
	std::string num;
	long long treeTimeTotal = 0, listTimeTotal = 0, arrayTimeTotal = 0;
	while (std::cin >> num)
	{
		try
		{
			const int n = stoi(num);
			{
				const auto startedTime = std::chrono::steady_clock::now();
				tree.insert(n);
				const auto endedTime = std::chrono::steady_clock::now();

				treeTimeTotal += std::chrono::duration_cast<std::chrono::nanoseconds>(endedTime - startedTime).count();
			}
			{
				const auto startedTime = std::chrono::steady_clock::now();
				list.push_back(n);
				const auto endedTime = std::chrono::steady_clock::now();

				listTimeTotal += std::chrono::duration_cast<std::chrono::nanoseconds>(endedTime - startedTime).count();
			}
			{
				const auto startedTime = std::chrono::steady_clock::now();
				dynamicArray.push_back(n);
				const auto endedTime = std::chrono::steady_clock::now();

				arrayTimeTotal += std::chrono::duration_cast<std::chrono::nanoseconds>(endedTime - startedTime).count();
			}

			askFor("number");
		}
		catch (std::invalid_argument&)
		{
			std::cout << "\nReceived a non-number value";
			std::cout << "\nStopping command execution";
			break;
		}
	}

	std::cout << "\nBinary tree time: " << treeTimeTotal;
	std::cout << "\nList time: " << listTimeTotal;
	std::cout << "\nDynamic array time: " << arrayTimeTotal;
}

void insertEndpoint()
{
	int num;
	askFor("number to insert");
	long long treeTimeTotal = 0, listTimeTotal = 0, arrayTimeTotal = 0;
	std::cin >> num;
	{
		const auto startedTime = std::chrono::steady_clock::now();
		tree.insert(num);
		const auto endedTime = std::chrono::steady_clock::now();

		treeTimeTotal += std::chrono::duration_cast<std::chrono::nanoseconds>(endedTime - startedTime).count();
	}
	{
		const auto startedTime = std::chrono::steady_clock::now();
		list.push_back(num);
		const auto endedTime = std::chrono::steady_clock::now();

		listTimeTotal += std::chrono::duration_cast<std::chrono::nanoseconds>(endedTime - startedTime).count();
	}
	{
		const auto startedTime = std::chrono::steady_clock::now();
		dynamicArray.push_back(num);
		const auto endedTime = std::chrono::steady_clock::now();

		arrayTimeTotal += std::chrono::duration_cast<std::chrono::nanoseconds>(endedTime - startedTime).count();
	}

	std::cout << "\nNumber " << num << " was inserted";

	std::cout << "\nBinary tree time: " << treeTimeTotal;
	std::cout << "\nList time: " << listTimeTotal;
	std::cout << "\nDynamic array time: " << arrayTimeTotal;
}

void printEndpoint()
{
	std::cout << "\nTree visual representation" << tree.serialize();
}

void deleteEndpoint()
{
	int num;
	askFor("number to delete");
	std::cin >> num;
	long long treeTimeTotal = 0, listTimeTotal = 0, arrayTimeTotal = 0;
	bool res, resList, resArray;
	{
		const auto startedTime = std::chrono::steady_clock::now();
		res = tree.remove(num);
		const auto endedTime = std::chrono::steady_clock::now();

		treeTimeTotal += std::chrono::duration_cast<std::chrono::nanoseconds>(endedTime - startedTime).count();
	}
	{
		const auto startedTime = std::chrono::steady_clock::now();
		auto itList = std::find(list.begin(), list.end(), num);
		resList = itList != list.end();
		if (resList) 
			list.erase(itList);
		const auto endedTime = std::chrono::steady_clock::now();

		listTimeTotal += std::chrono::duration_cast<std::chrono::nanoseconds>(endedTime - startedTime).count();
	}
	{
		const auto startedTime = std::chrono::steady_clock::now();
		auto itArr = std::find(dynamicArray.begin(), dynamicArray.end(), num);
		resArray = itArr != dynamicArray.end();
		if (resArray) 
			dynamicArray.erase(itArr);
		const auto endedTime = std::chrono::steady_clock::now();

		arrayTimeTotal += std::chrono::duration_cast<std::chrono::nanoseconds>(endedTime - startedTime).count();
	}

	std::cout << "\nNumber " << num << " was ";
	if (!res) std::cout << "not ";
	std::cout << "removed";

	std::cout << "\nBinary tree time: " << treeTimeTotal;
	std::cout << "\nList time: " << listTimeTotal;
	std::cout << "\nDynamic array time: " << arrayTimeTotal;
}

void findEndpoint()
{
	int num;
	askFor("number to search");
	std::cin >> num;
	long long treeTimeTotal = 0, listTimeTotal = 0, arrayTimeTotal = 0;
	bool res, resList, resArray;
	{
		const auto startedTime = std::chrono::steady_clock::now();
		res = tree.contains(num);
		const auto endedTime = std::chrono::steady_clock::now();

		treeTimeTotal += std::chrono::duration_cast<std::chrono::nanoseconds>(endedTime - startedTime).count();
	}
	{
		const auto startedTime = std::chrono::steady_clock::now();
		auto itList = std::find(list.begin(), list.end(), num);
		resList = itList != list.end();
		const auto endedTime = std::chrono::steady_clock::now();

		listTimeTotal += std::chrono::duration_cast<std::chrono::nanoseconds>(endedTime - startedTime).count();
	}
	{
		const auto startedTime = std::chrono::steady_clock::now();
		auto itArr = std::find(dynamicArray.begin(), dynamicArray.end(), num);
		resArray = itArr != dynamicArray.end();
		const auto endedTime = std::chrono::steady_clock::now();

		arrayTimeTotal += std::chrono::duration_cast<std::chrono::nanoseconds>(endedTime - startedTime).count();
	}

	std::cout << "\nNumber " << num << " was ";
	if (res) std::cout << "not ";
	std::cout << "found";

	std::cout << "\nBinary tree time: " << treeTimeTotal;
	std::cout << "\nList time: " << listTimeTotal;
	std::cout << "\nDynamic array time: " << arrayTimeTotal;
}

void reverseSearchEndpoint()
{
	std::cout << "\nReverse depth search results:\n" << tree.RDFS();
}

void depthSearchEndpoint()
{
	std::cout << "\nDepth search results:\n" << tree.DFS();
}

void breadthSearchEndpoint()
{
	std::cout << "\nBreadth search results:\n" << tree.BFS();
}

void processEndpoint(const char* endpoint)
{
	if (strcmp(endpoint, FILL_RANDOM) == 0) return fillRandomEndpoint();
	if (strcmp(endpoint, FILL_CONSOLE) == 0) return fillEndpoint();
	if (strcmp(endpoint, FILL_FILE) == 0) return fillFileEndpoint();
	if (strcmp(endpoint, INSERT) == 0) return insertEndpoint();
	if (strcmp(endpoint, PRINT) == 0) return printEndpoint();
	if (strcmp(endpoint, FIND) == 0) return findEndpoint();
	if (strcmp(endpoint, DELETE) == 0) return deleteEndpoint();
	if (strcmp(endpoint, DEPTH_SEARCH) == 0) return depthSearchEndpoint();
	if (strcmp(endpoint, BREADTH_SEARCH) == 0) return breadthSearchEndpoint();
	if (strcmp(endpoint, REVERSE_SEARCH) == 0) return reverseSearchEndpoint();
	if (strcmp(endpoint, HELP) == 0) return helpEndpoint();
	if (strcmp(endpoint, ABOUT) == 0) return infoEndpoint();
	if (strcmp(endpoint, EXIT) == 0) return exitEndpoint();

	std::cout << "\nCommand not found";
	std::cout << "\nPrint help to view list of all commands\n";
}

int main()
{
	std::string endpoint;
	infoEndpoint();
	srand(time(nullptr));
	bool shouldAskForYourCommand = true;
	while (endpoint != EXIT)
	{
		if (shouldAskForYourCommand) askFor("\nYour Command");
		std::getline(std::cin, endpoint);
		shouldAskForYourCommand = true;

		if (!endpoint.empty())
			processEndpoint(endpoint.c_str());
		else shouldAskForYourCommand = false;
	}
}