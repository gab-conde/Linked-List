#include <iostream>
#include <vector>
using namespace std;

template<typename T>
class LinkedList {

	public:

	// a class for the construction of Nodes
	// must be defined before we can create Node*
	struct Node {

		// data members of the Node class
		Node* prev;         // pointer to the previous Node
		Node* next;			// pointer to the next Node
		T data;				// storage for the Node's data

		// not defining the big 5 because we do not need anything special for the Node class
	};

	private:

	// data members of the LinkedList class
	// data members are private so they cannot be accessed outside of the class
	Node* head;				// pointer to the head of the LinkedList
	Node* tail;				// pointer to the tail of the LinkedList
	mutable Node* current;	// iterator for printing, deleting, etc.
	mutable Node* queued;	// stores prev/next pointer of current
	unsigned int size;		// number of Nodes currently in the LinkedList
    

	public:

	/* ---------- CONSTRUCTION / DESTRUCTION ---------- */
	// we cannot use default destructor (must delete all Nodes)

	// Constructor
	// Sets all pointers to null and size to zero.
	LinkedList<T>() {
		head = nullptr;
		tail = nullptr;
		current = nullptr;
		queued = nullptr;
		size = 0;
	}

	// Copy Constructor
    // Creates a new LinkedList and adds a copy of each Node from the other LinkedList to the new LinkedList.
	LinkedList<T>(const LinkedList<T>& otherList) {
		Clone(otherList);
	}

	// Destructor
	// Deletes all Nodes and resets data members to nullptr and 0.
	~LinkedList<T>() {
		Clear();
		
		// reset list data members
		// current and queued are already set to nullptr
		head = nullptr;
		tail = nullptr;
		size = 0;	
	}


	/* ---------- OPERATORS ---------- */

	// Returns the data from the Node at the specified index.
    // Throws an out_of_range error if no such Node exists.
	T& operator[](unsigned int index) {
		Node* node = GetNode(index);
		return node->data;
	}

    // Returns a constant version of the data from the Node at the specified index.
    // Throws an out_of_range error if no such Node exists.
	const T& operator[](unsigned int index) const {
		Node* node = GetNode(index);
		const T& constData = node->data;
		return constData;
	}

    // Determines whether two LinkedLists are exactly equal.
    bool operator==(const LinkedList<T>& rhs) const {
        if (size != rhs.size) {
            return false;
        }
        else if (size == 0) {
            return true;
        }
        current = head;
        rhs.current = head;
        while (current != nullptr) {
            if (current->data != rhs.current->data) {
                return false;
            }
            current = current->next;
            rhs.current = rhs.current->next;
        }
        return true;
    }

	//  Deletes a LinkedList and re-constructs it using the Copy Constructor.
    LinkedList<T>& operator=(const LinkedList<T>& rhs) {
		Clear();
        LinkedList<T>& list = *Clone(rhs);
        return list;
    }

	// private:

	// /* ---------- Helper Functions ---------- */

    // Copies all member variables and Nodes from one LinkedList to another.
    LinkedList<T>* Clone(const LinkedList<T>& otherList) {
        LinkedList<T>();
		size = 0;

		otherList.current = otherList.head;
		while (otherList.current != nullptr) {
            AddTail(otherList.current->data);
			otherList.current = otherList.current->next;
		}
        size = otherList.size;
		return this;
    }

	// Iterates through a LinkedList and returns the Node with the specified value.
	Node* Search(Node* start, const T& value) const {
		current = start;

		while(current != nullptr) {
			if (current->data == value) {
				return current;
			}
			current = current->next;
		}
		return nullptr;
	}


	public:

	/* ---------- BEHAVIORS ---------- */

	// Prints all Nodes in the LinkedList from beginning to end.
	void PrintForward() const {
		current = head;

		while (current != nullptr) {
			
			cout << current->data << endl;
			current = current->next;
		}
	}

	// Prints all Nodes in the LinkedList from end to beginning.
	void PrintReverse() const {
		current = tail;

		while (current != nullptr) {
			
			cout << current->data << endl;
			current = current->prev;
		}
	}


	/* ---------- ACCESSORS ---------- */

    // Returns the number of Nodes in the LinkedList.
	unsigned int NodeCount() const {
		return size;
	}

    // Returns a pointer to the first Node with the specified data.
	Node* Find(const T& data) {
		if (size > 0) {
			Node* foundNode = Search(head, data);
			return foundNode;
		}
		return nullptr;
	}

	// Returns a constant pointer to the first Node with the specified data.
	const Node* Find(const T& data) const {
		if (size > 0) {
			const Node* foundNode = Search(head, data);
			return foundNode;
		}
		return nullptr;
	}

	// Fills a vector with 
	void FindAll(vector<Node*>& outData, const T& value) const {
		if (size > 0) {
			current = head;
			Node* foundNode = nullptr;
			while (current != nullptr) {
				foundNode = Search(current, value);
				outData.push_back(foundNode);
				current = foundNode->next;
			}
		}
	}
    
    // Returns a pointer to the Node at the specified index.
    // Throws an out_of_range error if no such Node exists.
    Node* GetNode(unsigned int index) {
        if (index > size || size == 0) {
            throw out_of_range("");
        }
        // if index is in the first half of the LinkedList, start from head and iterate forward
        if (index <= (size - 1) / 2) {
            current = head;
            unsigned int currentIndex = 0;
            while (currentIndex != index) {
                current = current->next;
                currentIndex++;
            }
            return current;
        }
        // if index is in the second half of the LinkedList, start from end and iterate backwards
        current = tail;
        unsigned int currentIndex = size - 1;
        while(currentIndex != index) {
            current = current->prev;
            currentIndex--;
        }
        return current;
    }

    // Returns a constant pointer to the Node at the specified index.
    // Throws an out_of_range error if no such index exists.
    const Node* GetNode(unsigned int index) const {
		if (index > size || size == 0) {
            throw out_of_range("");
        }
        // if index is in the first half of the LinkedList, start from head and iterate forward
        if (index <= (size - 1) / 2) {
            current = head;
            unsigned int currentIndex = 0;
            while (currentIndex != index) {
                current = current->next;
                currentIndex++;
            }
            const Node* constNode = current;
			return constNode;
        }
        // if index is in the second half of the LinkedList, start from end and iterate backwards
        current = tail;
        unsigned int currentIndex = size - 1;
        while(currentIndex != index) {
            current = current->prev;
            currentIndex--;
        }
		const Node* constNode = current;
        return constNode;
    }

    // Returns a pointer to the head of the LinkedList.
    Node* Head() {
        return head;
    }

    // Returns a constant pointer to the head of the LinkedList.
    const Node* Head() const {
        const Node* constHead = head;
        return constHead;
    }

    // Returns a pointer to the tail of the LinkedList.
    Node* Tail() {
        return tail;
    }

    // Returns a constant pointer to the tail of the LinkedList.
    const Node* Tail() const {
        const Node* constTail = tail;
        return constTail;
    }


	/* ---------- INSERTION ---------- */

	// Creates a new Node and adds it at the beginning of the LinkedList.
    // Increments the size of the LinkedList.
	void AddHead(const T& data) {
        Node* newNode = new Node();
		newNode->data = data;
        if (size == 0) {
            newNode->next = nullptr;
            tail = newNode;
        }
		else {
            newNode->next = head;
            head->prev = newNode;
        }
        newNode->prev = nullptr;
        head = newNode;
        
		size++;
	}

	// Creates a new Node and adds it at the end of the LinkedList.
    //  Increments the size of the LinkedList.
	void AddTail(const T& data) {
		Node* newNode = new Node();
		newNode->data = data;
        if (size == 0) {
            newNode->prev = nullptr;
            head = newNode;
        }
        else {
		    newNode->prev = tail;
		    tail->next = newNode;
        }
		newNode->next = nullptr;
        tail = newNode;

		size++;
	}

    // Adds an array of data to the head of the LinkedList, creating a new Node for each datum.
	void AddNodesHead(const T* data, unsigned int count) {
		for (unsigned int i = count; i > 0; i--) {
			AddHead(data[i-1]);
		}
	}

    // Adds an array of data to the tail of the LinkedList, creating a new Node for each datum.
	void AddNodesTail(const T* data, unsigned int count) {
		for (unsigned int i = 0; i < count; i++) {
			AddTail(data[i]);
		}
	}


	/* ---------- REMOVAL ---------- */

	// Deletes all Nodes from the LinkedList and resets size to 0.
	void Clear() {
		current = tail;

		while (current != nullptr) {
			queued = current->prev;
			delete current;
			current = queued;
		}
	}
};