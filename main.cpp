
#include <iostream>
#include <fstream>

#include <unordered_map>

//#define OUTPUT

using namespace std;

// TODO: encapsulate Tree Data Structure

// mark - Tree

class NodeTree
{
public:
	char _char[100];
	float _frequency;
	
	int _code;
	
	NodeTree *_parent;
	NodeTree *_left;
	NodeTree *_right;
	
	NodeTree()
	{
		_char[0] = '\0';
		_frequency = 0.f;
		
		_code = -1;
		_parent = NULL;
		_left = NULL;
		_right = NULL;
	}
	
	NodeTree(char aCharacter, float aFrequency)
	{
		_char[0] = aCharacter;
		_char[1] = '\0';
		_frequency = aFrequency;
		
		_code = -1;
		_parent = NULL;
		_left = NULL;
		_right = NULL;
	}
	
	bool isLeaf()
	{
		return (_left == NULL && _right == NULL);
	}
};

void _printHuffmanCodes(NodeTree *aNode, int *aCode, int aCodeLength);
void _printHuffmanCodes(NodeTree *aNode, int *aCode, int aCodeLength)
{
	// avoid storing parent code of -1 (which is meaningless)
	if (aNode -> _parent)
	{
		aCode[aCodeLength++] = aNode -> _code;
	}
	
	if (aNode -> isLeaf())
	{
		cout << "Character: "
		// TODO: Remove duplicated code!
		<< (aNode -> _char[0] == '\n' ? "'new line'" : aNode -> _char)
		<< " Code: ";
		for (int i = 0; i < aCodeLength; ++i)
		{
			cout << aCode[i];
		}
		cout << endl;
	}
	else
	{
		_printHuffmanCodes(aNode -> _left, aCode, aCodeLength);
		_printHuffmanCodes(aNode -> _right, aCode, aCodeLength);
	}
}

void printHuffmanCodes(NodeTree *aRoot);
void printHuffmanCodes(NodeTree *aRoot)
{
	cout << endl << "Printing Huffman codes..." << endl;
	cout << "----------------" << endl;
	int code[100];
	_printHuffmanCodes(aRoot, code, 0);
	cout << "----------------" << endl;
}

// mark - List

class NodeList
{
public:
    NodeList *_next;
	NodeList *_previous;
	
    NodeTree *_data;
	
    NodeList(NodeList *aNext = NULL, NodeList *aPrevious = NULL, NodeTree *aData = NULL)
    {
        _next = aNext;
		_previous = aPrevious;
        _data = aData;
    }
};

class List
{
private:
	// empty nodes to simplify logic
	NodeList * _pre_head;
	NodeList *_post_tail;
	int _size;
	
	void insert(NodeTree *aData, NodeList *aNodeList)
	{
		NodeList *next = aNodeList -> _next;
		NodeList *newNodeList = new NodeList(next, aNodeList, aData);
		next -> _previous = aNodeList -> _next = newNodeList;
		
		++_size;
	}
	
	NodeList *remove(NodeList *&aNodeList)
	{
		if (isEmpty())
		{
			throw runtime_error("Error! The list is empty!");
		}
		
		NodeList *next = aNodeList -> _next;
		NodeList *previous = aNodeList -> _previous;
		
		next -> _previous = previous;
		previous -> _next = next;
		
		delete aNodeList;
		aNodeList = NULL;
		--_size;
		
		return previous;
	}
	
	NodeTree *lookUp(NodeList *aNodeList)
	{
		if (isEmpty())
		{
			throw runtime_error("Error! The list is empty!");
		}
		return aNodeList -> _data;
	}
	
public:
	List()
	{
		_size = 0;
		_pre_head = new NodeList();
		_post_tail = new NodeList();
		
		// NULL <- _pre_head <-> _post_tail -> NULL
		
		_pre_head -> _previous = _post_tail;
		_post_tail -> _next = _pre_head;
	}
	
	~List()
	{
		NodeList *nodeList = _pre_head;
		while (nodeList)
		{
			NodeList *tempNodeList = nodeList;
			nodeList = nodeList -> _previous;
			
			delete tempNodeList;
		}
	}
	
	bool isEmpty()
	{
		return getSize() == 0;
	}
	
	int getSize() const
	{
		return _size;
	}
	
	void pushBack(NodeTree *aData)
	{
		insert(aData, _post_tail);
	}
	
	void pushFront(NodeTree *aData)
	{
		insert(aData, _pre_head -> _previous);
	}
	
	NodeTree *popBack()
	{
		NodeList *nodeList = _post_tail -> _next;
		NodeTree *data = nodeList -> _data;
		remove(nodeList);
		return data;
	}
	
	NodeTree *popFront()
	{
		NodeList *nodeList = _pre_head -> _previous;
		NodeTree *data = nodeList -> _data;
		remove(nodeList);
		return data;
	}
	
	NodeTree *lookUpFront()
	{
		return lookUp(_pre_head -> _previous);
	}
	
	NodeTree *lookUpBack()
	{
		return lookUp(_post_tail -> _next);
	}
	
	friend ostream &operator<<(ostream &anOutputStream, const List &aList)
	{
		NodeList *nodeList = aList._pre_head -> _previous;
		while (nodeList != aList._post_tail)
		{
			NodeTree *node = nodeList -> _data;
			anOutputStream << "Character: "
			// TODO: Remove duplicated code!
			<< (node -> _char[0] == '\n' ? "'new line'" : node -> _char)
			<< " Frequency: " << node -> _frequency << endl;
			
			nodeList = nodeList -> _previous;
		}

		return anOutputStream;
	}
};

// mark - Queue

class Queue
{
private:
	List _list;
	
public:
	Queue()
	{
	}
	
	~Queue()
	{
	}
	
	bool isEmpty()
	{
		return _list.isEmpty();
	}
	
	int getSize() const
	{
		return _list.getSize();
	}
	
	void enqueue(NodeTree *aData)
	{
		_list.pushBack(aData);
	}
	
	NodeTree *dequeue()
	{
		return _list.popFront();
	}
	
	NodeTree *lookUp()
	{
		return _list.lookUpFront();
	}
	
	friend ostream &operator<<(ostream &anOutputStream, const Queue &anQueue)
	{
		anOutputStream << anQueue._list;
		return anOutputStream;
	}
};

// mark -  Huffman Codes Algorithm

NodeTree *mergeChildren(NodeTree *aFirstChild, NodeTree *aSecondChild);
NodeTree *mergeChildren(NodeTree *aFirstChild, NodeTree *aSecondChild)
{
    aFirstChild -> _code = 1; // left
    aSecondChild -> _code = 0; // right
    
    NodeTree *parent = new NodeTree();
    parent -> _frequency = aFirstChild -> _frequency + aSecondChild -> _frequency;
    parent -> _left = aFirstChild;
    parent -> _right = aSecondChild;
    
    char *chParent = parent -> _char;
    char *ch = aFirstChild -> _char;
    while(*ch != '\0')
    {
        *(chParent++) = *(ch++);
    }
    ch = aSecondChild -> _char;
    while(*ch != '\0')
    {
        *(chParent++) = *(ch++);
    }
    *chParent = '\0';
    
    aFirstChild -> _parent = aSecondChild -> _parent = parent;
    
    return parent;
}

void minimumFrequencyTreeNode(Queue &aLeavesQueue, Queue &aTreeNodesQueue, NodeTree *&aTreeNode);
void minimumFrequencyTreeNode(Queue &aLeavesQueue, Queue &aTreeNodesQueue, NodeTree *&aTreeNode)
{
	NodeTree *firstNodeTree = NULL;
	NodeTree *secondNodeTree = NULL;
	if (!aLeavesQueue.isEmpty())
	{
		firstNodeTree = aLeavesQueue.lookUp();
		if (!aTreeNodesQueue.isEmpty())
		{
			secondNodeTree = aTreeNodesQueue.lookUp();
			if (firstNodeTree -> _frequency < secondNodeTree -> _frequency)
			{
				aTreeNode = aLeavesQueue.dequeue();
			}
			else
			{
				aTreeNode = aTreeNodesQueue.dequeue();
			}
		}
		else
		{
			aTreeNode = aLeavesQueue.dequeue();
		}
	}
	else
	{
		if (!aTreeNodesQueue.isEmpty())
		{
			aTreeNode = aTreeNodesQueue.dequeue();
		}
	}
}

void buildHuffmanCodes(Queue &aLeavesQueue, NodeTree *&aRoot);
void buildHuffmanCodes(Queue &aLeavesQueue, NodeTree *&aRoot)
{
	cout << endl << "Building Huffman codes..." << endl;
	
#ifdef OUTPUT
	cout << "----------------" << endl;
#endif
	
	Queue treeNodesQueue;
	while (!aLeavesQueue.isEmpty() || treeNodesQueue.getSize() > 1)
	{
#ifdef OUTPUT
		cout << aLeavesQueue;
		cout << treeNodesQueue;
#endif
		
		// get two minimum tree nodes from the queues
		
		// get first tree node form the list
		NodeTree *firstChild = NULL;
		minimumFrequencyTreeNode(aLeavesQueue, treeNodesQueue, firstChild);
		
		// get second tree node form the list
		NodeTree *secondChild = NULL;
		minimumFrequencyTreeNode(aLeavesQueue, treeNodesQueue, secondChild);
		
		// merge tree nodes
		if (firstChild == NULL || secondChild == NULL)
		{
			throw runtime_error("Error! Children must not be NULL.");
		}
		
		NodeTree *parent = mergeChildren(firstChild, secondChild);
		treeNodesQueue.enqueue(parent);

#ifdef OUTPUT
		cout << "----------------" << endl;
#endif
	}
	
#ifdef OUTPUT
	cout << treeNodesQueue;
	cout << "----------------" << endl;
#endif
	
	aRoot = treeNodesQueue.dequeue();
}

// mark -

// TODO: to Mikhail Buhaiov - Use Entry class everywhere!
class Entry
{
public:
	char _ch;
	float _frequency;
	char _code[100];
	
	Entry(char aChar = '\0', float aFrequency = .0f)
	{
		_code[0] = '\0';
		set(aChar, aFrequency);
	}
	
	void set(char aChar, float aFrequency)
	{
		_ch = aChar;
		_frequency = aFrequency;
	}
	
	bool operator < (const Entry& anEntry) const
	{
		return (_frequency < anEntry._frequency);
	}
};

void preprocess(int anEntriesArraysSize, Entry *anEntries, Queue &aLeavesQueue);
void preprocess(int anEntriesArraysSize, Entry *anEntries, Queue &aLeavesQueue)
{
// TODO: to Mikhail Buhaiov - Learn Merge Sort! Implement it and use it here instead of C++ Standard Library 'sort'!
	sort(anEntries, anEntries + anEntriesArraysSize);
	
	for (int i = 0; i < anEntriesArraysSize; ++i)
	{
		NodeTree *nodeTree = new NodeTree(anEntries[i]._ch, anEntries[i]._frequency);
		aLeavesQueue.enqueue(nodeTree);
	}
}

// mark -

void generateCodes(int anEntriesArraysSize, Entry *anEntries);
void generateCodes(int anEntriesArraysSize, Entry *anEntries)
{
	Queue leavesQueue;
	// don't forget to clear dynamically allocated memory for list nodes and tree nodes
	preprocess(anEntriesArraysSize, anEntries, leavesQueue);
	cout << "Alphabet Frequencies..." << endl;
	cout << "----------------" << endl;
	cout << leavesQueue;
	cout << "----------------" << endl;
	
	// list nodes memory is cleared here
	NodeTree *root = nullptr;
	buildHuffmanCodes(leavesQueue, root);
	printHuffmanCodes(root);
	
	// TODO: clear tree nodes memory
}

// mark - Application

void getFileName(const char *&aFileName);
void getFileName(const char *&aFileName)
{
	aFileName = "Message.txt";
}

// TODO: to Mikhail Buhaiov - Learn about Hash Table Data Structure form the book I gave you!
// Implement your out Hash Table and use it here instead of C++ Standard Library 'unordered_map'!
// http://www.cplusplus.com/reference/unordered_map/unordered_map/

void computeAlphabetFrequencies(int &anEntriesArraysSize, Entry *&anEntries, const char *&aFileName);
void computeAlphabetFrequencies(int &anEntriesArraysSize, Entry *&anEntries, const char *&aFileName)
{
	cout << "File To Encode: " << aFileName << endl << endl;
	
	fstream fileStream(aFileName, ios::in);
	if (fileStream.is_open())
	{
		cout << "Computing Alphabet Frequencies..." << endl << endl;

		unordered_map<char, int> alphabetOccurenceMap;
		unordered_map<char, int>::iterator iterator;
		int count = 0;

		for(char ch = fileStream.get(); !fileStream.eof(); ch = fileStream.get())
		{
			count += 1;
			
			iterator = alphabetOccurenceMap.find(ch);
			if (iterator == alphabetOccurenceMap.end())
				alphabetOccurenceMap[ch] = 1;
			else
				iterator -> second += 1;
		}
		
		anEntriesArraysSize = int(alphabetOccurenceMap.size());
		anEntries = new Entry[anEntriesArraysSize];
		
		int i;
		for (i = 0, iterator = alphabetOccurenceMap.begin();
			 iterator != alphabetOccurenceMap.end();
			 ++i, ++iterator)
		{
//			cout << iterator -> first << " " << iterator -> second << endl;
			
			// iterator -> first - char
			// iterator -> second - number of occurences of the char
			anEntries[i].set(iterator -> first, float(iterator -> second) / float(count));
		}
		
		fileStream.close();
	}
	else
	{
		cout << "Error. File is not found." << endl;
	}
}

void generateCodes(const char *&aFileName);
void generateCodes(const char *&aFileName)
{
	int size = -1;
	Entry *entries = nullptr;
	computeAlphabetFrequencies(size, entries, aFileName);
	generateCodes(size, entries);
	
	// use entries to encode file
	delete [] entries;
}

void encodeFile();
void encodeFile()
{
	const char *fileName = nullptr;
	getFileName(fileName);
	generateCodes(fileName);
}

// mark -

int menu();
int menu()
{
	cout << "Encodings Demonstration Program (Huffman Codes)." << endl;
	cout << "0 - Exit." << endl;
	cout << "1 - Encode." << endl;
	cout << "2 - Decode." << endl;
	cout << "> ";
	
	float integer;
	cin >> integer;
	if(cin.fail())
	{
		cin.clear();
		cin.ignore(2000, '\n');
		integer = -1;
	}
	else if (integer < 0 || integer > 4 || (integer - int(integer)) > 0.0001)
	{
		integer = -1;
	}
	return int(integer);
}

int run();
int run()
{
	int isInitialRun = true;
	int option = -1;
	do
	{
		if (isInitialRun)
			isInitialRun = false;
		else
			cout << endl;
		
		option = menu();
		cout << endl;
		if (option == -1)
		{
			cout << "Invalid selection. Please, try again." << endl;
			continue;
		}
		
		switch (option)
		{
			case 0:
			{
				cout << "Thank you! Good bye!" << endl;
				break;
			}
			case 1:
			{
				encodeFile();
				break;
			}
			case 2:
			{
				break;
			}
			default:
				break;
		}
	}
	while(option != 0);
	return 0;
}

// mark -

int main() {
	return run();
}
