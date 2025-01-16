#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <string>
using namespace std; //to silence namespace std


/*
Data Structures: Linked Lists pg 258
Linked list: data structure in which the objects are arranged in a linear order
Unlike an array in which the linear order is determined by the array indices, the 
order in a linked list is determined by a pointer in each object. 
    indice/index: location of an item in an array
since the elemnts of linkedd lists often contain key that can be searched for
    elements: individual data items in an array, accessed with an index or subscript
    keys: value associated with a specific node that helps in identifying or accessing that node
Since the elements of linekd lists often contain keys that can be searched for, linked lists are sometimes called search lists
Linked lists provide a simple flexible representation for dynamic sets, supporitng (though not necessarily efficiently) all operations listed on pg 250

Doubly linked list L: an object with an attribute key and two pointer attributes: next and prev, and may also contain other satellite data
    The next of the tail attribute, and prev of the head attribute is NIL
    This relationship only holds when referring to the head or tail, and can be used as it's definition

Singly linked: each element has a next pointer but not a prev pointer
Sorted: the linear order of the list corresponds to the lienar order of keys stored in elements of the list
    Head of the list is the minimum element, the tail of the list is the maximum elements
Unsorted: the elements of the linked list can appear in any order
Circular list: the prev poitner of the head of the list points to the tail, and the enxt pointer of the tail of the list points to the head

Linked lists in the Standard Template Library: https://en.cppreference.com/w/cpp/container/list
std::list - implemented as a doubly linked list
std::forward_list - specifically a singly linked list

int main() {
    //Create a list containing integers
    list<int> l{12, 45, 8, 6};
    //Add an integer to the front of the list
    l.push_front(25);
    //Add an integer to the back of the list
    l.push_back(13);

    //insert an integer before 16 by searching it
    auto it = find(l.begin(), l.end(), 16);
    if (it != l.end()) 
        l.insert(it, 42);
}
    
Insert and find shown above

Remove() function to delete elements from the list

Hash Tables pg 272
Many applications require a dynamic set that supports only the dictionary operations INSERT, SEARCH, DELETE. 
    dynamic set: a collection of elements that can grow or shrink in size during runtime, allowing for flexible data amangement
    This is particularly relevant in data structures and algorithms, and it constasts with static sets where the size and structure
    are fixed at compile time
Hash table: effective data structure for implementing dictionaries. 
    Time complexity: Although searching for an element in a hash table can take as long
    as searching for an elmeent in a linked list theta(n) time in the worst case-in practice, hasing performs extremely well
    Average time to search for an element in a hash table is O(1). 
It generalizes the simpler notion of an ordinary array. Directly addressing into an ordinary array takes advantage of the O(1) access 
time for any array element. However, it is high in space complexity has to use direct addressing, you must be able to allocate an array 
that contains a position for every possible key. 

When the number of keys actually stored is small relative to the total number of possible keys, hash tables become an effetive
alternative to directly addressing an array, since a hash table typically uses an array of size proportional the number of keys actually stored. 
Instead of using the key as an array index directly, we compute the array index from the key. 

Chaining: a way to handle collisions in which more than one key maps to the same array index. 
*/

struct Node {
    int key;
};

int hashFunction(int k, int m) {
    return k % m; //hash function h(k) = k mod m
}

void DirectAddressInsert(list<Node>& T, Node& x); //used in HashTable class

class HashTable {
    vector<list<Node>> table;
    int m;

public:
    HashTable(int size) : m(size) {
        table.resize(m);
    }
    void insert(int k) {
        int index = hashFunction(k, m);
        Node x = {k};
        DirectAddressInsert(table[index], x);
    }
};

int DirectAddressSearch(const list<Node>& T, int k) {
    auto it = find_if(T.begin(), T.end(), [k](const Node& node) { return node.key == k; }); //grabs an iterator that serves as a generalized pointer-like object providing a way to access and traverse elements in a container
    //without needing to know the details of the contain's structure. We need a custom predicate (or lambda) to find based on key
    if (it != T.end()) {
        return distance(T.begin(), it);
        //int position = distance(T.begin(), it);
    }
    return -1;  //The key is not found
}

void DirectAddressInsert(list<Node>& T, Node& x) {
    T.push_front(x); 
}

void DirectAddressDelete(list<Node>& T, int key) { //Delete only once, the element which appears earliest
    auto it = find_if(T.begin(), T.end(), [key](const Node& node) { return node.key == key; });
    if (it != T.end()) {
        T.erase(it); //delete first occurence
    }
    //T.remove_if([key](const Node& node) { return node.key == key; });
    //Gets rid of all occurences
}

int main(int argc, char* argv[]) {
    int m; //the size of the chained hash table
    cin >> m;
    //create hashtable
    //hash function h(k) = k mod m
    vector<list<Node>> table(m);

    string input;
    cin >> input;
    while (input[0] != 'e') {    //e = exit, finish your program
        char letter = input[0];
        int key = -1;
        if (input.size() > 1) {
            key = stoi(input.substr(1));
        }
        int index = hashFunction(key, m);

        if (letter == 'i') {     //insert key into the table, "i2" implies Insert key 2
            //For collisions, insert the colliding key at the beginning of the linked list. 
            //Insert the key and don't output anything. 
            Node x = {key};
            DirectAddressInsert(table[index], x);
        }
        else if (letter == 'd') {    //delete key from the table. "d2" implies Delete key 2 from the table
            //If there are multiple elements of the same key value, delete the element of the key value that 
            //appears the earliest in the list. If the delete was successful, you have to output
            size_t initialSize = table[index].size();
            DirectAddressDelete(table[index], key);
            if (table[index].size() == initialSize) {
                cout << key << ":DELETE_FAILED;" << endl;
            }
            else {
                cout << key << ":DELETED;" << endl;
            }
        }
        else if (letter == 's') { //Search (key) in the table.       
            int position = DirectAddressSearch(table[index], key);
            if (position != -1) {
                cout << key << ":FOUND_AT" << index << "," << position << ";" << endl;
            }
            else {
                cout << key << ":NOT_FOUND;" << endl;
            }
        }
        else if (letter == 'o') { //Output the entire hash table, 
        //0:6->3->;
        //1:1->;
        //2:;
        for (int i = 0; i < m; i++) {
            cout << i << ":";
            for (const auto& node : table[i]) {
                cout << node.key << "->";
            }
            cout << ";" << endl;
        }
        }
        cin >> input; 
    }
    return 0;
}

/*
Errors: 
1. terminating due to uncaught exception of type std::invalid_argument: stoi: no conversion zsh: abort
    occurs when stoi is trying to convert a string that dose not contain a valid integer. 
    stoi(input.substr(1)) - assumed to provide the substring up until the length (end)
    Invalid for output 'o', switched from outside the if statements to inside 3/4 that contain them
    Still doesn't work-
    Solution: check length while maintaining position outside the if statements. Key is intialized to be outside the table
2. Insert at the end instead of the beginning of the linkedList
    push_front(value) instead of push_back(value)
*/






/*
Inserting into a linked list
Given an element x whose key attribute has already been set, the LIST-PREPEND procedure adds x to the front of the 
linked list. 


class Node { 
public:
    int key; //Two components of a Node: the data it contains, and the pointer next to the next Node
    int data;
    Node* next;
    Node* prev;


    //Constructor to initialize a new node with data
    Node(int new_data){
        data = new_data;
        next = nullptr;
    }
};


Searching a Linked List: finds the first element with key k in list L by a simple lienar search, returning
a pointer to this element. If no object with key k appears in the list, then the procedure returns NIL
    LIST-SEARCH(L, k)
        x = L.head
        while x != NIL and x.key != k
            x = x.next
        return x


int List_Search(Node* L, int k) {
    Node* x = L; //Should access the head of the array L
    while (x->prev != nullptr) {
        x = x->prev;
    }

    while (x != nullptr && x->key != k) { //NULL - traditionally defined as integer constant with value of zero (0) 
    //nullptr: a keyword that represents a null pointer constant, which has no ambiguity in overlaoded function scenarios
    //-> to access it's members, since x is a pointer to an object
        x = x->next; //Shift the pointer so it now points to the next node
    }
};


//The function to check whether a key is present within a linked list
bool searchKey(Node* head, int key) {
    Node* curr = head;

    while (curr != NULL) {
        if (curr->data == key) {
            return true;
        }
        curr = curr->next;
    }
}
*/
