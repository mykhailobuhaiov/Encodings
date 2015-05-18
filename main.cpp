
#include <iostream>
#include <fstream>

#include <stdexcept>

#include <unordered_map>

#define OUTPUT_CONSOLE

using namespace std;

// mark - Tree

// TODO: to Mikhail Buhaiov - Encapsulate Tree Data Structure.
class NodeTree
{
public:
	char _char[100];
	float _frequency;
	
	char _code;
	
	NodeTree *_parent;
	NodeTree *_left;
	NodeTree *_right;
	
	NodeTree()
	{
		_char[0] = '\0';
		_frequency = 0.f;
		
		_code = '-';
        
		_parent = nullptr;
		_left = nullptr;
		_right = nullptr;
	}
	
	NodeTree(char aCharacter, float aFrequency)
	{
		_char[0] = aCharacter;
		_char[1] = '\0';
		_frequency = aFrequency;
		
		_code = '-';
        
		_parent = nullptr;
		_left = nullptr;
		_right = nullptr;
	}
	
	bool isLeaf()
	{
		return (_left == nullptr && _right == nullptr);
	}
    
    bool isRoot()
    {
        return _parent == nullptr;
    }
};

// TODO: to Mikhail Buhaiov - Rework Huffman Codes displaying.

void _printHuffmanCodes(NodeTree *aNode, char *aCode, int aCodeLength);
void _printHuffmanCodes(NodeTree *aNode, char *aCode, int aCodeLength)
{
	// avoid storing parent code of '-' (which is meaningless)
	if (!aNode -> isRoot())
	{
		aCode[aCodeLength++] = aNode -> _code;
	}
	
	if (aNode -> isLeaf())
	{
        // Note! New line character '\n' does not show up. It translates the cursor to the new line!
		cout << "Character: " << aNode -> _char << " Code: ";
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
	char code[100];
	_printHuffmanCodes(aRoot, code, 0);
	cout << "----------------" << endl << endl;
}

// mark - List

class NodeList
{
public:
    NodeList *_next;
	NodeList *_previous;
	
    NodeTree *_data;
	
    NodeList(NodeList *aNext = nullptr, NodeList *aPrevious = nullptr, NodeTree *aData = nullptr)
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
		aNodeList = nullptr;
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
		
		// nullptr <- _pre_head <-> _post_tail -> nullptr
		
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
            
            // Note! New line character '\n' does not show up. It translates the cursor to the new line!
			anOutputStream << "Character: " << node -> _char
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

// mark - Stack

class Stack
{
private:
    List _list;
    
public:
    Stack()
    {
    }
    
    ~Stack()
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
    
    void push(NodeTree *aData)
    {
        _list.pushBack(aData);
    }
    
    NodeTree *pop()
    {
        return _list.popBack();
    }
    
    NodeTree *lookUp()
    {
        return _list.lookUpBack();
    }
    
    friend ostream &operator<<(ostream &anOutputStream, const Stack &aStack)
    {
        anOutputStream << aStack._list;
        return anOutputStream;
    }
};

// mark -  Huffman Codes Algorithm

NodeTree *mergeChildren(NodeTree *aFirstChild, NodeTree *aSecondChild);
NodeTree *mergeChildren(NodeTree *aFirstChild, NodeTree *aSecondChild)
{
    aFirstChild -> _code = '1'; // left
    aSecondChild -> _code = '0'; // right
    
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
	NodeTree *firstNodeTree = nullptr;
	NodeTree *secondNodeTree = nullptr;
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
	cout << "Building Huffman codes..." << endl;
	
#ifdef OUTPUT_CONSOLE
	cout << "----------------" << endl;
#endif
	
	Queue treeNodesQueue;
	while (!aLeavesQueue.isEmpty() || treeNodesQueue.getSize() > 1)
	{
#ifdef OUTPUT_CONSOLE
		cout << aLeavesQueue;
		cout << treeNodesQueue;
#endif
		
		// get two minimum tree nodes from the queues
		
		// get first tree node form the list
		NodeTree *firstChild = nullptr;
		minimumFrequencyTreeNode(aLeavesQueue, treeNodesQueue, firstChild);
		
		// get second tree node form the list
		NodeTree *secondChild = nullptr;
		minimumFrequencyTreeNode(aLeavesQueue, treeNodesQueue, secondChild);
		
		// merge tree nodes
		if (firstChild == nullptr || secondChild == nullptr)
		{
			throw runtime_error("Error! Children must not be nullptr.");
		}
		
		NodeTree *parent = mergeChildren(firstChild, secondChild);
		treeNodesQueue.enqueue(parent);

#ifdef OUTPUT_CONSOLE
		cout << "----------------" << endl;
#endif
	}
	
#ifdef OUTPUT_CONSOLE
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
	char _char;
	float _frequency;
    
    NodeTree *_codeReference;
	
	Entry(char aChar = '\0', float aFrequency = .0f)
	{
		set(aChar, aFrequency);
        
        _codeReference = nullptr;

	}
	
	void set(char aChar, float aFrequency)
	{
		_char = aChar;
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
		NodeTree *nodeTree = new NodeTree(anEntries[i]._char, anEntries[i]._frequency);
		aLeavesQueue.enqueue(nodeTree);
        
        anEntries[i]._codeReference = nodeTree;
	}
}

// mark -

void computeCodes(int anEntriesArraysSize, Entry *anEntries, NodeTree *&aRoot);
void computeCodes(int anEntriesArraysSize, Entry *anEntries, NodeTree *&aRoot)
{
	Queue leavesQueue;
	// don't forget to clear dynamically allocated memory for list nodes and tree nodes
	preprocess(anEntriesArraysSize, anEntries, leavesQueue);
	cout << "Alphabet Frequencies..." << endl;
	cout << "----------------" << endl;
	cout << leavesQueue;
	cout << "----------------" << endl << endl;
	
	// list nodes memory is cleared here
	buildHuffmanCodes(leavesQueue, aRoot);
    printHuffmanCodes(aRoot);
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
	cout << "Openning File (" << aFileName << ") For Alphabet Analysis..." << endl << endl;
	
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

void encodeFile(int anEntriesArraysSize, Entry *anEntries, NodeTree *aRoot, const char *aFileName);
void encodeFile(int anEntriesArraysSize, Entry *anEntries, NodeTree *aRoot, const char *aFileName)
{
    unordered_map<char, NodeTree *> codeReferences;
    for (int i = 0; i < anEntriesArraysSize; ++i)
    {
        codeReferences[anEntries[i]._char] = anEntries[i]._codeReference;
    }
    
    cout << "Openning Files For Encoding..." << aFileName << endl << endl;
    
    char outputFileName[100];
    strcpy(outputFileName, "Encoded_");
    strcat(outputFileName, aFileName);
    fstream outputFileStream(outputFileName, ios::out | ios::trunc);
    
    fstream fileStream(aFileName, ios::in);
    if (fileStream.is_open() && outputFileStream.is_open())
    {
        cout << "Encoding File (" << aFileName << ") into File (" << outputFileName << ") ..." << endl << endl;
        
        Stack stack;
        for(char ch = fileStream.get(); !fileStream.eof(); ch = fileStream.get())
        {
            try
            {
                NodeTree *nodeTree = codeReferences.at(ch);
                
                while (nodeTree)
                {
                    stack.push(nodeTree);
                    nodeTree = nodeTree -> _parent;
                }
                
                while (!stack.isEmpty())
                {
                    nodeTree = stack.pop();
                    
                    // skip root
                    if (nodeTree -> isRoot())
                        continue;
                    
                    outputFileStream << nodeTree -> _code;
                    
#ifdef OUTPUT_CONSOLE
                    cout << nodeTree -> _code;
#endif
                }
                
            }
            catch (out_of_range &anExeption)
            {
                cout << "No entry found for character " << ch << ". Check your algorithm logic." << endl;
                break;
            }
        }
        
#ifdef OUTPUT_CONSOLE
        cout << endl;
#endif
        
        outputFileStream.close();
        fileStream.close();
        
    }
    else
    {
        cout << "Error. File is not found." << endl;
    }
}

void encodeFile(const char *aFileName);
void encodeFile(const char *aFileName)
{
	int size = -1;
	Entry *entries = nullptr;
	computeAlphabetFrequencies(size, entries, aFileName);
    
    NodeTree *root = nullptr;
	computeCodes(size, entries, root);

    encodeFile(size, entries, root, aFileName);
	
	delete [] entries;
    // TODO: to Mikhail Buhaiov - Clear tree nodes memory.
}

void encodeFile();
void encodeFile()
{
	const char *fileName = nullptr;
	getFileName(fileName);
	encodeFile(fileName);
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
