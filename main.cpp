
#include <iostream>

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

void _printHaffmanCodes(NodeTree *aNode, int *aCode, int aCodeLength);
void _printHaffmanCodes(NodeTree *aNode, int *aCode, int aCodeLength)
{
	// avoid storing parent code of -1 (which is meaningless)
	if (aNode -> _parent)
	{
		aCode[aCodeLength++] = aNode -> _code;
	}
	
	if (aNode -> isLeaf())
	{
		cout << "Character: " << aNode -> _char << " Code: ";
		for (int i = 0; i < aCodeLength; ++i)
		{
			cout << aCode[i];
		}
		cout << endl;
	}
	else
	{
		_printHaffmanCodes(aNode -> _left, aCode, aCodeLength);
		_printHaffmanCodes(aNode -> _right, aCode, aCodeLength);
	}
}

void printHaffmanCodes(NodeTree *aRoot);
void printHaffmanCodes(NodeTree *aRoot)
{
	cout << endl << "Printing Haffman codes..." << endl;
	cout << "----------------" << endl;
	int code[100];
	_printHaffmanCodes(aRoot, code, 0);
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

// mark -  Haffman Codes Algorithm

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

void buildHaffmanCodes(Queue &aLeavesQueue, NodeTree *&aRoot);
void buildHaffmanCodes(Queue &aLeavesQueue, NodeTree *&aRoot)
{
	cout << endl << "Building Haffman codes..." << endl;
	cout << "----------------" << endl;
	
	Queue treeNodesQueue;
	while (!aLeavesQueue.isEmpty() || treeNodesQueue.getSize() > 1)
	{
		cout << aLeavesQueue;
		cout << treeNodesQueue;
		
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
		
		cout << "----------------" << endl;
	}
	
	cout << treeNodesQueue;
	cout << "----------------" << endl;
	
	aRoot = treeNodesQueue.dequeue();
}

void preprocess(int anArraysSize, char *anAlphabet, float *aFrequencies, Queue &aLeavesQueue);
void preprocess(int anArraysSize, char *anAlphabet, float *aFrequencies, Queue &aLeavesQueue)
{
	// TODO: sort array before
	for (int i = 0; i < anArraysSize; ++i)
	{
		int index = anArraysSize - i - 1;
		NodeTree *nodeTree = new NodeTree(anAlphabet[index], aFrequencies[index]);
		aLeavesQueue.enqueue(nodeTree);
	}
}

// mark - main

int main() {
	
	char alphabet[] = {'a', 'b', 'c', 'd', 'e'};
	float frequencies[] = {0.32, 0.25, 0.2, 0.18, 0.05};
	int size = 5;
	
	Queue leavesQueue;
	// don't forget to clear dynamically allocated memory for list nodes and tree nodes
	preprocess(size, alphabet, frequencies, leavesQueue);
	cout << leavesQueue;
	
	NodeTree *root = NULL;
	// list nodes memory is cleared here
	buildHaffmanCodes(leavesQueue, root);
	printHaffmanCodes(root);
	
	// TODO: clear tree nodes memory
}
