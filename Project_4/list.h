#ifndef CS20A_LIST_H
#define CS20A_LIST_H

#include<iostream>
#include<assert.h>
// Linked List object that maintains both head and tail pointers
// and the size of the list.  Note that you have to keep the head,
// tail and size consistent with the intended state of the List 
// otherwise very bad things happen. 
template<typename Type>
class List {
public:

	List();

	List(const List<Type>& other);

	List<Type>& operator=(const List<Type>& other);

	~List();

	void	print() const;
	bool	empty() const;

	void	push_front(const Type &item);
	void	push_back(const Type &item);
	void	add(int index, const Type &item);

	// Note that the user must first ensure the list is not empty
	// prior to calling these functions.
	Type	front() const;
	Type	rear() const;
	Type	get_at(int index) const;

	int		size() const;
	int		find(const Type &item) const;

	bool	pop_front();
	bool	pop_rear();
	bool	pop_at(int index);

#ifndef MAKE_MEMBERS_PUBLIC
private:
#endif
	// Forward declare the nodes for our List.
	// Will be implemented along with list's
	// member functions
	class Node;

	// We'll have both head and tail points for 
	// Fast insertion/deletion from both ends.
	Node*	m_head;
	Node*	m_tail;

	// Keep track of size of list
	int		m_size;
};


// List Implementation
//
//  Since List is a template class (which is not an actual 
//  class yet, not until we actually instantiate the list) 
//  we need to keep the implementation together with 
//  the definition.  There are ways to simulate having 
//  separate "implementation/definition" with templates, 
//  but they aren't necessary and can be confusing.
//

// Node definition
//		Already implemented, nothing to do here but to use it.
template<typename Type>
class List<Type>::Node {
public:
	Node() :next(nullptr), prev(nullptr) {}
	Node(Type item, Node* p, Node* n) : item(item), next(n), prev(p) {}

	Type  item;
	Node* next;
	Node* prev;
};



// List default constructor
//		Set head and tail pointer to point to nothing, size is zero
//		Already implemented, nothing to do.
template<typename Type>
List<Type>::List() :m_head(nullptr), m_tail(nullptr), m_size(0) {
}


// Copy constructor  
//		This should create an entirely new linked list with the same 
//		number of Nodes and the Values stored these Nodes in the same 
//		order as seen the other list’s Nodes.  This should not result 
//		in any memory leaks or aliasing.  
template<typename Type>
List<Type>::List(const List<Type>& other) {
    
    //
    if (other.m_head == nullptr){
        m_head = m_tail = nullptr;
        m_size = 0;
    }
    else {
        
        m_size = 0;
        m_head = m_tail = nullptr;
        
        add(0, other.get_at(0));
        
        for (int i= 1; i < other.size(); i++){
            push_back(other.get_at(i));
            //std::cout << std::endl << m_tail -> item;
        }
        
        }


}
// Overloaded assignment operator 
//		Causes the already existing linked list to be identical to the 
//		other linked list without causing any memory leaks or aliasing.
template<typename Type>
List<Type>& List<Type>::operator=(const List<Type>& other) {
	
    if (&other == this)
        return *this;
    else if (other.m_head == nullptr && m_head != nullptr){
        while (m_head != nullptr) pop_rear();
        m_size = 0;
        return *this;
    }
    else {
        
        if (m_head != nullptr){
            delete this;
            m_size = 0;
        }
        //m_head = m_tail = nullptr;
        
        if (other.m_size != 0){
        push_front( other.get_at(0) );
        for (int i= 1; i < other.size(); i++){
            push_back(other.get_at(i));
            //std::cout << std::endl << m_tail -> item;
            }
        }
        
        return *this;
    }

}


// List destructor
//		Traverses the nodes and deletes them without causing memory leaks
template<typename Type>
List<Type>::~List() {
    
    Node * tempPtr = m_head;
    while (tempPtr != nullptr){
        //std::cout << tempPtr -> item << std::endl; //debugging
        Node * n = tempPtr -> next;
        delete tempPtr;
        tempPtr = n;
    }
    //m_head = m_tail = nullptr;
    m_size = 0;
}

// List print
//		Traverses list and prints items in list to console
//		Assumes << is overloaded for object Type.
//		

template<typename Type>
void List<Type>::print() const {
	
    Node * tempPtr;
    tempPtr = m_head;
    
    while(tempPtr != nullptr){
        
        
        if (tempPtr -> next == nullptr){
            std::cout << tempPtr -> item <<std::endl;
        }
        else
            std::cout << tempPtr -> item <<" ";
            
        tempPtr = tempPtr -> next;
    }
  
}

// List empty
//		returns true if there are no items in the list
//		otherwise returns false.
template<typename Type>
bool List<Type>::empty() const {
    
    if (m_head == nullptr) return true; //if head of the list is null, it is empty
    else
        return false;
}


// List push_front
//		Adds a new item to the front of the list
//		updates head, tail, and size accordingly.
//		
//		Two cases to consider: If the list is empty 
//		and if the list has items already it.
//
template<typename Type>
void List<Type>::push_front(const Type &item) {
	
    
    Node * tempPtr = new Node; // allocates memory and invokes empty constructor
    tempPtr -> item = item; //store the new value accordingly
    tempPtr -> next = m_head; //links new node to the old top node in the list
    tempPtr -> prev = nullptr; // prev already null due to empty constructor
    
    //link list pointers accordingly
    if ( empty() ) {m_tail = tempPtr;}
    else
        m_head -> prev = tempPtr;
    
    m_head = tempPtr; //head pointer now holds address of new node
    m_size += 1;
}

// List push_back
//		Adds a new item to the rear of the list
//		updates head, tail, and size accordingly.
//
//		Two cases to consider: If the list is empty 
//		and if the list has items already it.
//
template<typename Type>
void List<Type>::push_back(const Type &item) {
	
    Node * tempPtr = new Node;
    tempPtr -> item = item;
    tempPtr -> prev = m_tail;
    tempPtr -> next = nullptr;
    
    if ( empty() ) m_head = tempPtr;
    else
        m_tail -> next = tempPtr;
    
    m_tail = tempPtr;
    m_size += 1;
}

// List add
//		Adds a new item at the index indicated
//		updates head, tail, and size accordingly.
//
//		If the index less than or equal to zero add to the front.
//		If the index is greater than or equal to
//			the size of the list, add to the rear.
//		All other cases, add the item to the appropriate
//			location
//
template<typename Type>
void List<Type>::add(int index, const Type &item) {
	
    if (index <= 0)
        push_front(item);
    else if ( index >= size()  ){
        push_back(item);
    }
    else {
        Node * temp = m_head;
        int counter = 0;
        
        while(counter != index){
            temp = temp -> next;
            counter++;
        }
        Node * newNode = new Node;
        newNode -> item = item;
        newNode -> next = temp;
        newNode -> prev = temp -> prev;
        temp -> prev -> next = newNode;
        temp -> prev = newNode;
        m_size++;
    }
             
}

// 8. List front
//		returns the first item in the list.
//
template<typename Type>
Type List<Type>::front() const {
	// Force check that the head is pointing to a Node
	// Typical solution for deployement code is to throw
	// exceptions, but since we haven't covered that yet
	// we'll make due with assert, which is used for testing
	assert(m_head != nullptr);

	return m_head -> item;
}

// List rear
//		returns the last item in the list
//
template<typename Type>
Type List<Type>::rear() const {

	// Force check that the tail is pointing to a Node
	// Typical solution for deployement code is to throw
	// exceptions, since we haven't covered that yet
	// we'll make due with assert, which is used for testing
	assert(m_tail != nullptr);

	return m_tail -> item;
	
}

// List get_at
//		returns the item at index
//
template<typename Type>
Type List<Type>::get_at(int index) const {

	// Force index to be correct before getting the Item
	// Typical solution for deployement code is to throw
	// exceptions, since we haven't covered that yet
	// we'll make due with assert, which is used for testing
	assert(index >= 0 && index < m_size );
    
    Node * tempPtr;
    tempPtr = m_head;
    int counter = 0;
    
    while(counter != index){
        tempPtr = tempPtr -> next;
        counter++;
    }
	return tempPtr -> item;
}

// List size
//		return the size of the list
template<typename Type>
int List<Type>::size() const {

	return m_size;
}

// List find
//		Traverses the list to see if the item is in the list
//		if it is return the index of that item, otherwise
//		return -1. Assumes the == operator is overloaded for
//		object Type.
template<typename Type>
int List<Type>::find(const Type &item) const {
    int index = 0;
    
    Node * tempNode = m_head;
    while(tempNode != nullptr){
        if (tempNode -> item == item)
            return index;
        tempNode = tempNode -> next;
        index++;
    }
	return -1;
}

// List pop_front
//		Removes the first item in the list returns true
//		if the item was deleted, false otherwise. Updates 
//		head, tail, and size accordingly.
//
//		Must consider at least two cases where the list: is empty, 
//		has one or more items.
//		
template<typename Type>
bool List<Type>::pop_front() {
    
    Node * removeNode;
    removeNode = m_head;
    
    if ( empty() ) return false;
    else if (m_size > 1) {
        m_head = removeNode -> next;
        m_head -> prev = nullptr;
        m_size--;
    }
    else {
        m_head = m_tail = nullptr;
        m_size--;
    }
    
    delete removeNode;
    return true;
}
// List pop_rear
//		Removes the last item in the list returns true
//		if the item was deleted, false otherwise. Updates 
//		head, tail, and size accordingly.
//
//		Must consider at least three cases where the list: is empty, 
//		has one item, has two or more items.
//	
template<typename Type>
bool List<Type>::pop_rear() {
    
    Node * removeNode;
    removeNode = m_tail;
    
    if ( empty() ) return false;
    else if (m_size > 1) {
        m_tail = removeNode -> prev;
        m_tail -> next = nullptr;
        m_size--;
    }
    else {
        m_head = m_tail = nullptr;
        m_size--;
    }
    
    delete removeNode;
    return true;
}

// List pop_at
//		Removes the item at the index returns true
//		if the item was deleted, false otherwise. Updates 
//		head, tail, and size accordingly. If the index is
//		out of bounds return false.
//
template<typename Type>
bool List<Type>::pop_at(int index) {
    
    int counter = 0;
    Node * removeNode = m_head;
    
    
    if (index < 0 || index >= m_size )
        return false;
    else if (index == 0){
        pop_front();
        return true;
    }
    else if (index == (m_size-1)){
        pop_rear();
        return true;
    }
    else {
        
        while(counter != index){
            removeNode = removeNode -> next;
            counter++;
        }
        
        //previous node's next will point to current node's next
        removeNode->prev->next = removeNode->next;
        //next node's prev will point to current node's prev
        removeNode->next->prev = removeNode->prev;
        delete removeNode;
        m_size--;
        
        return true;
    }


}

#endif
