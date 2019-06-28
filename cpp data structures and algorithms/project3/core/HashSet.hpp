// HashSet.hpp
//
// ICS 46 Winter 2019
// Project #3: Set the Controls for the Heart of the Sun
//
// A HashSet is an implementation of a Set that is a separately-chained
// hash table, implemented as a dynamically-allocated array of linked
// lists.  At any given time, the HashSet has a "size" indicating
// how many elements are stored within it, along with a "capacity"
// indicating the size of the array.
//
// As elements are added to the HashSet and the proportion of the HashSet's
// size to its capacity exceeds 0.8 (i.e., there are more than 80% as many
// elements as there are array cells), the HashSet should be resized so
// that it is twice as large as it was before.
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the information
// in your data structure.  Instead, you'll need to use a dynamically-
// allocated array and your own linked list implemenation; the linked list
// doesn't have to be its own class, though you can do that, if you'd like.

#ifndef HASHSET_HPP
#define HASHSET_HPP

#include <functional>
#include "Set.hpp"
#include <memory>
#include <algorithm>
// #include "StringHashing.hpp"
// #include <iostream>


template <typename ElementType>
class HashSet : public Set<ElementType>
{
public:
    // The default capacity of the HashSet before anything has been
    // added to it.
    static constexpr unsigned int DEFAULT_CAPACITY = 10;

    // A HashFunction is a function that takes a reference to a const
    // ElementType and returns an unsigned int.
    using HashFunction = std::function<unsigned int(const ElementType&)>;
private:
    struct Node{
		ElementType value;
		Node* next;
	};

public:
    // Initializes a HashSet to be empty, so that it will use the given
    // hash function whenever it needs to hash an element.
    explicit HashSet(HashFunction hashFunction);

    // Cleans up the HashSet so that it leaks no memory.
    virtual ~HashSet() noexcept;

    // Initializes a new HashSet to be a copy of an existing one.
    HashSet(const HashSet& s);

    // Initializes a new HashSet whose contents are moved from an
    // expiring one.
    HashSet(HashSet&& s) noexcept;

    // Assigns an existing HashSet into another.
    HashSet& operator=(const HashSet& s);

    // Assigns an expiring HashSet into another.
    HashSet& operator=(HashSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement a HashSet, false otherwise.
    virtual bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function triggers a resizing of the
    // array when the ratio of size to capacity would exceed 0.8.  In the case
    // where the array is resized, this function runs in linear time (with
    // respect to the number of elements, assuming a good hash function);
    // otherwise, it runs in constant time (again, assuming a good hash
    // function).
    virtual void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function runs in constant time (with respect
    // to the number of elements, assuming a good hash function).
    virtual bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    virtual unsigned int size() const noexcept override;


    // elementsAtIndex() returns the number of elements that hashed to a
    // particular index in the array.  If the index is out of the boundaries
    // of the array, this function returns 0.
    unsigned int elementsAtIndex(unsigned int index) const;


    // isElementAtIndex() returns true if the given element hashed to a
    // particular index in the array, false otherwise.  If the index is
    // out of the boundaries of the array, this functions returns 0.
    bool isElementAtIndex(const ElementType& element, unsigned int index) const;


    void empty(Node** Vtable); //sets every node to nullptr, DOES NOT DEALLOCATE

    float load_factor();

    void rehash();

    void smol_add(const ElementType& element, Node** target_table);

    void print();

    void delete_a_bucket(Node* bucket);

    void deallocate_all(Node** a_table, unsigned int cap);

private:
	

    HashFunction hashFunction;

    Node** table;

    unsigned int capacity;
    unsigned int _size;


};



namespace impl_
{
    template <typename ElementType>
    unsigned int HashSet__undefinedHashFunction(const ElementType& element)
    {
        return 0;
    }
}


template <typename ElementType> //constructor
HashSet<ElementType>::HashSet(HashFunction hashFunction)
    : hashFunction{hashFunction}, table{new Node*[DEFAULT_CAPACITY]}, capacity{DEFAULT_CAPACITY}, _size{0}
{
	empty(table);
}


template <typename ElementType> //destructor
HashSet<ElementType>::~HashSet() noexcept
{
	deallocate_all(table, capacity);
}

template <typename ElementType> 
void HashSet<ElementType>::delete_a_bucket(Node* bucket){
	Node* first = bucket;
	while (first != nullptr){
		Node* keep = first->next;
		delete first;
		first = keep;
	}
}

template <typename ElementType>
void HashSet<ElementType>::deallocate_all(Node** a_table, unsigned int cap){
	for (int i = 0; i < cap; i++){
		delete_a_bucket(a_table[i]);
	}

	delete[] a_table;
}

template <typename ElementType> //copy constructor
HashSet<ElementType>::HashSet(const HashSet& s)
    : hashFunction{impl_::HashSet__undefinedHashFunction<ElementType>}, table{new Node*[s.capacity]}, capacity{s.capacity}, _size{s.size()}
{
	hashFunction = s.hashFunction;
	empty(table);

	for (int i = 0; i < capacity; i++){
		Node* navigator = s.table[i];
		Node* pointer = nullptr;
		while (navigator != nullptr){
			if (table[i] == nullptr){
				table[i] = new Node{navigator->value, nullptr};
				pointer = table[i];
			}
			else{
				pointer->next = new Node{navigator->value, nullptr};
				pointer = pointer->next;
			}
			navigator = navigator->next;
		}
	}
}


template <typename ElementType> //move constructor
HashSet<ElementType>::HashSet(HashSet&& s) noexcept
    : hashFunction{impl_::HashSet__undefinedHashFunction<ElementType>}, table{new Node*[s.capacity]}, capacity{s.capacity}, _size{s.size()}
{
	hashFunction = s.hashFunction;
	empty(table);
	std::swap(table, s.table);
	// std::cout<<"triggered"<<std::endl;
}


template <typename ElementType> //asignment operator
HashSet<ElementType>& HashSet<ElementType>::operator=(const HashSet& s)
{
	if ( &s!= this){
		
		Node** temp_table = new Node*[s.capacity];
		empty(temp_table);

		for (int i = 0; i < s.capacity; i++){
			Node* navigator = s.table[i];
			Node* pointer = nullptr;
			while (navigator != nullptr){
				if (temp_table[i] == nullptr){
					temp_table[i] = new Node{navigator->value, nullptr};
					pointer = temp_table[i];
				}
				else{
					pointer->next = new Node{navigator->value, nullptr};
					pointer = pointer->next;
				}
				navigator = navigator->next;
			}
		}
		
		deallocate_all(table, capacity);
		hashFunction = s.hashFunction;
		_size = s._size;
		capacity = s.capacity;
		table = temp_table;
	}
    return *this;
}


template <typename ElementType> //move operator
HashSet<ElementType>& HashSet<ElementType>::operator=(HashSet&& s) noexcept
{
	if (&s != this){
		hashFunction = s.hashFunction;
		capacity = s.capacity;
		_size = s._size;
		std::swap(table, s.table);
	}
    return *this;
}


template <typename ElementType>
bool HashSet<ElementType>::isImplemented() const noexcept
{
    return true;
}


template <typename ElementType>
void HashSet<ElementType>::add(const ElementType& element)
{
	// std::cout << "adding: " << element << std::endl;
	smol_add(element, table);
	if (load_factor() > float(0.8)){
		rehash();

	}
		
	
}


template <typename ElementType>
bool HashSet<ElementType>::contains(const ElementType& element) const
{
    unsigned int index = hashFunction(element)%capacity;
    if (isElementAtIndex(element, index)){
    	return true;
    }
 
    return false;
}


template <typename ElementType>
unsigned int HashSet<ElementType>::size() const noexcept
{
    return _size;
}


template <typename ElementType>
unsigned int HashSet<ElementType>::elementsAtIndex(unsigned int index) const
{
	if (index >= capacity){
		return 0;
	}
    unsigned int count = 0;
    Node* head = table[index];
    while (head != nullptr){
    	count += 1;
    	head = head->next;
    }
    return count;
}


template <typename ElementType>
bool HashSet<ElementType>::isElementAtIndex(const ElementType& element, unsigned int index) const
{
	if (index >= capacity){
		return 0;
	}
    Node* head = table[index];
    while (head != nullptr){
    	if (head->value == element){
    		return true;
    	}
    	head = head->next;
    }
    return false;
}

template<typename ElementType>
void HashSet<ElementType>::empty(Node** Vtable){
	for (int i = 0; i < capacity; i ++){
		Vtable[i] = nullptr;
	}
}
template<typename ElementType>
float HashSet<ElementType>::load_factor(){
	return (size()*1.0)/(capacity*1.0);
}

template<typename ElementType>
void HashSet<ElementType>::rehash(){
	// rehash_count += 1;
	// std::cout<<"rehashing"<< rehash_count << std::endl;
	unsigned int temp_capacity = capacity;
	capacity *= 2;
	Node** temp_table = new Node*[capacity];

	empty(temp_table);

	for (int i = 0; i < temp_capacity; i++){

		Node* head = table[i];

		while (head != nullptr){

			int temp_size = _size;
			smol_add(head->value, temp_table);
			_size = temp_size;


			head = head -> next;
		}
	}
	// std::cout << "new capacity: " << capacity << std::endl;
	// // table[temp_capacity-1] = std::shared_ptr<Node>{};
	// std::cout << "old capacity: " << temp_capacity << std::endl;
	// for (int i =0; i < temp_capacity; i++){
	// 	table[i] = nullptr;
	// }
	// table[0] = nullptr;
	// std::cout << "deleting table"<<std::endl;
	deallocate_all(table, temp_capacity);
	// std::cout << "deleted" << std::endl;
	table = temp_table;

	

}

template<typename ElementType>
void HashSet<ElementType>::smol_add(const ElementType& element, Node** target_table){
	unsigned int index = hashFunction(element)%capacity;
	// std::cout<<"hash value for: " << element << " is "<< hashFunction(element) << std::endl;

	if (target_table[index] == nullptr){
		target_table[index] = new Node{element, nullptr};
		_size++;
	}
	else if (target_table[index]->value == element){
		return;
	}
	else{
		Node* head = target_table[index];
		while (head->next != nullptr){
			if (head->next->value == element){
				return;
			}
			head = head -> next;
		}
		head->next = new Node{element, nullptr};
		_size++;
	}

}

// template<typename ElementType>
// void HashSet<ElementType>::print(){
// 	for (int i = 0; i < capacity; i++){
// 		std::cout << i << ": ";
// 		if (table[i] == nullptr){
// 			std::cout << "null" << std::endl;
// 			continue;
// 		}
// 		std::shared_ptr<Node> navi = table[i];
// 		while (navi != nullptr){
// 			std::cout << navi->value << ", ";
// 			navi = navi->next;
// 		}
// 		std::cout<<std::endl;

// 	}
// 	std::cout<< "capacity: " << capacity << "; size: " << size() << "; loadfactor: "<< load_factor() << std::endl;
// }


#endif // HASHSET_HPP
