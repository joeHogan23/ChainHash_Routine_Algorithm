#include "pch.h"
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <ostream>
#include <fstream>
using namespace std;

//--------------------------------------------------------------------------------------------------//
//											class Node  											//
//--------------------------------------------------------------------------------------------------//

class Node {
private:
	string m_id;
	string m_name;
	string m_email;
	string m_faveFood;

	//Linked list which embeds Nodes with same hash value.
	Node* m_next = nullptr;

public:

	//Default Constructor
	Node() {};

	//Overloaded Constructor
	Node(string id, string name, string email, string faveFood) : m_id(id), m_name(name), m_email(email), m_faveFood(faveFood) {};

	//Destructor
	~Node() {};

	//------------------------------------------
	//			Getters/Setters
	//------------------------------------------

	string getId() { return m_id; }
	void setId(string id) { m_id = id; }

	string getName() { return m_name; }
	void setName(string n) { m_name = n; }

	string getEmail() { return m_email; }
	void setEmail(string e) { m_email = e; }

	string getFaveFood() { return m_faveFood; }
	void setFaveFood(string f) { m_faveFood = f; }

	//Points to next object in linked list
	Node *getNext() { return m_next; }
	void setNext(Node *n) { m_next = n; }
};

//--------------------------------------------------------------------------------------------------//
//										class HashTable												//
//--------------------------------------------------------------------------------------------------//

class HashTable {
private:
	int m_size;
	vector<Node*> m_nodes;

public:
	// --- Constructor ---
	HashTable(int size) {

		//Create enough space to store objects
		m_nodes = vector<Node*>(m_size = size);
	};

	// --- indexes an element into vector of nodes. Checks for collision and places Node into LinkedList ---
	void hashRoutine(Node node)
	{
		//Position in vector
		int key = getKey(node.getId());

		//Checks if there is collision
		if (m_nodes[key] != 0)
		{
			Node *tmpNode = m_nodes[key];
			cout << tmpNode->getId();

			//Walks through list of Nodes at position in vector
			while (tmpNode->getNext() != nullptr)
			{
				//Traverses to the next Node
				tmpNode = tmpNode->getNext();
				cout << "-->" << tmpNode->getId();
			}
			//Add a new node at the tail of the list
			tmpNode->setNext(new Node(node));
			cout << "-->" << tmpNode->getNext()->getId() << endl;
		}
		else
			//If nothing is at position in vector, create a new node
			m_nodes[key] = new Node(node);
	}

	// --- Look for Node by key ---
	Node searchForNode(string keyValue)
	{
		int key = getKey(keyValue);

		cout << "Node with Primary key: " << keyValue << " found at position: " << key << endl;
		Node *tmpNode = m_nodes[key];

		//Will traverse through embedded Nodes if there are any
		while (tmpNode->getNext() != 0) {

			//Compares the currentNode to the KeyValue
			if (keyValue == tmpNode->getId()) {
				break;
			}

			//Traverse through embedded nodes at position in m_nodes
			tmpNode = tmpNode->getNext();
		}
		return *tmpNode;
	}

	// --- Takes data from CSV, and runs hashroutine to index them ---
	void generateFromCSV(string fileName)
	{
		//file that will be input into m_nodes
		fstream file;

		string line;

		//Reusable node
		Node node;
		file.open(fileName);
		
		//Get all CSV values in line
		while (file.good())
		{
			getline(file, line, ',');
			node.setId(line);
			getline(file, line, ',');
			node.setName(line);
			getline(file, line, ',');
			node.setEmail(line);
			getline(file, line, '\n');
			node.setFaveFood(line);

			hashRoutine(node);
		}
	}

	// --- Prints entire vector and displays all element ids', including chain-linked Nodes ---
	void displayLocations() {
		//Iterate through vector of nodes
		for (int i = 0; i < m_size; i++) {
			Node* tmpNode = m_nodes[i];

			//If null, skip tmpNode
			if (tmpNode == 0) {
				cout << ((i < 10) ? "Position 0" : "Position ") << i << ": EMPTY" << endl;
				continue;
			}

			//Output first node Id at position
			cout << ((i < 10) ? "Position 0" : "Position ") << i << ": " << tmpNode->getId();

			//Iterate through the linked list within tmpNode if there are any
			while (tmpNode->getNext() != 0) {
				//Output current node in list
				cout << " --> " << tmpNode->getNext()->getId();

				//Go to next node in linked list
				tmpNode = tmpNode->getNext();
			}
			cout << endl;
		}
	}

	// -- Assign an index, clamping key size with modulus operator ---
	int getKey(string keyValue)
	{
		int key = 0;

		//Add all ascii values in string to sum 
		for (int j = 0; j < keyValue.length(); j++)
		{
			//To reduce collision, multiply ascii value at position by (position + 10) 
			key += (keyValue[j] * (j + 10));
		}
		//Restrain key span to the length of vector
		return key % m_size;
	}

	// --- Set HashTable Nodes ---
	void setTable(vector<Node*> n) {
		m_nodes = n;
	}

	// --- Get HashTable Nodes ---
	vector<Node*> getTable() {
		return m_nodes;
	}
};


//--------------------------------------------------------------------------------------------------//
//												Driver												//
//--------------------------------------------------------------------------------------------------//

int main()
{
	HashTable hashTable = HashTable(100);
	cout << "Positions Shared:" << endl;
	hashTable.generateFromCSV("mydata.csv");

	cout << endl;

	//Examples of Nodes at the same position in hashTable

	Node search = hashTable.searchForNode("P12");
	cout << "Key: " << search.getId() << endl << "Name: " << search.getName() << endl << "Email: " << search.getEmail() << endl << "Favorite Food: " << search.getFaveFood() << endl << endl;

	search = hashTable.searchForNode("P93");
	cout << "Key: " << search.getId() << endl << "Name: " << search.getName() << endl << "Email: " << search.getEmail() << endl << "Favorite Food: " << search.getFaveFood() << endl << endl;

	search = hashTable.searchForNode("P100");
	cout << "Key: " << search.getId() << endl << "Name: " << search.getName() << endl << "Email: " << search.getEmail() << endl << "Favorite Food: " << search.getFaveFood() << endl << endl;


	system("pause");
	cout << endl << endl;

	hashTable.displayLocations();
	system("pause");
}