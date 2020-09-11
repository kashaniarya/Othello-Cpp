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


private:
    HashFunction hashFunction;
    struct Node {
        ElementType data;
        Node* next;
    };
    unsigned int amountOfBuckets;
    unsigned int sz;
    Node** hashTable;
    double loadFactor() const;
};



namespace impl_
{
    template <typename ElementType>
    unsigned int HashSet__undefinedHashFunction(const ElementType& element)
    {
        return 0;
    }
}

template <typename ElementType>
double HashSet<ElementType>::loadFactor() const {
    double retVal = 1.0 * sz / amountOfBuckets;
    return retVal;
}


// Initializes a HashSet to be empty, so that it will use the given
// hash function whenever it needs to hash an element.
template <typename ElementType>
HashSet<ElementType>::HashSet(HashFunction hashFunction)
    : hashFunction{hashFunction}
{
    hashTable = new Node*[DEFAULT_CAPACITY];
    amountOfBuckets = DEFAULT_CAPACITY;
    sz = 0;
    for(int i=0; i < amountOfBuckets; i++) {
        hashTable[i] = nullptr;
    }
}

// Cleans up the HashSet so that it leaks no memory.
template <typename ElementType>
HashSet<ElementType>::~HashSet() noexcept
{
    for(int i=0; i < amountOfBuckets; i++) {
        while(hashTable[i] != nullptr) {
            Node* currentHeadNode = hashTable[i];
            hashTable[i] = currentHeadNode->next;
            delete currentHeadNode;
        }
    }
    delete[] hashTable;
}

// Initializes a new HashSet to be a copy of an existing one.
template <typename ElementType>
HashSet<ElementType>::HashSet(const HashSet& s)
    : hashFunction{impl_::HashSet__undefinedHashFunction<ElementType>}
{
    hashFunction = s.hashFunction;
    hashTable = new Node*[s.amountOfBuckets];
    amountOfBuckets = s.amountOfBuckets;
    sz = s.sz;
    for(int i=0; i < s.amountOfBuckets; i++) {
        hashTable[i] = s.hashTable[i];
    }
}

// Assigns an expiring HashSet into another.
template <typename ElementType>
HashSet<ElementType>::HashSet(HashSet&& s) noexcept
    : hashFunction{impl_::HashSet__undefinedHashFunction<ElementType>}
{
    amountOfBuckets = DEFAULT_CAPACITY;
    hashTable = new Node*[amountOfBuckets];
    for(int i=0; i < amountOfBuckets; i++) {
        hashTable[i] = nullptr;
    }
    std::swap(sz, s.sz);
    std::swap(hashTable, s.hashTable);
    std::swap(amountOfBuckets, s.amountOfBuckets);
    std::swap(hashFunction, s.hashFunction);
}

// Assigns an existing HashSet into another.
template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(const HashSet& s)
{
    if(this != &s) {
        Node **hT = new Node*[s.amountOfBuckets];
        for(int i=0; i < s.amountOfBuckets; i++) {
            hT[i] = s.hashTable[i];
        }
        hashFunction = s.hashFunction;
        sz = s.sz;
        for(int i=0; i < amountOfBuckets; i++) {
            while(hashTable[i] != nullptr) {
                Node* currentHeadNode = hashTable[i];
                hashTable[i] = currentHeadNode->next;
                delete currentHeadNode;
            }
        }
        delete[] hashTable;
        hashTable = hT;
        amountOfBuckets = s.amountOfBuckets;
    }
    return *this;
}

// Assigns an expiring HashSet into another.
template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(HashSet&& s) noexcept
{
    if(this != &s) {
        std::swap(hashTable, s.hashTable);
        std::swap(hashFunction,s.hashFunction);
        std::swap(sz,s.sz);
        std::swap(amountOfBuckets, s.amountOfBuckets);
    }
    return *this;
}


template <typename ElementType>
bool HashSet<ElementType>::isImplemented() const noexcept
{
    return true;
}

// add() adds an element to the set.  If the element is already in the set,
// this function has no effect.  This function triggers a resizing of the
// array when the ratio of size to capacity would exceed 0.8.  In the case
// where the array is resized, this function runs in linear time (with
// respect to the number of elements, assuming a good hash function);
// otherwise, it runs in constant time (again, assuming a good hash
// function).
template <typename ElementType>
void HashSet<ElementType>::add(const ElementType& element)
{
    if(contains(element) == false) {
        unsigned int numberLocation = hashFunction(element) % amountOfBuckets;
        Node* headNode = hashTable[numberLocation];
        Node* newNode = new Node();
        newNode->data = element;
        newNode->next = nullptr;
        if(headNode == nullptr) {
            hashTable[numberLocation] = newNode;
        }
        else {
            while(headNode->next != nullptr) {
                headNode = headNode->next;
            }
            headNode->next = newNode;
        }
        sz += 1;
        if(loadFactor() > 0.8) {
            unsigned int old_amountOfBuckets = amountOfBuckets;
            amountOfBuckets = amountOfBuckets * 2;
            Node** hT = new Node*[amountOfBuckets];
            for(int i=0; i < amountOfBuckets; i++) {
                hT[i] = nullptr;
            }
            for(int i=0; i < old_amountOfBuckets; i++) {
                Node* currentHeadNode = hashTable[i];
                while(currentHeadNode != nullptr) {
                    unsigned int new_numberLocation = hashFunction(currentHeadNode->data) % amountOfBuckets;
                    Node* new_headNode = hT[new_numberLocation];
                    Node* new_newNode = new Node();
                    new_newNode->data = currentHeadNode->data;
                    new_newNode->next = nullptr;
                    if(new_headNode == nullptr) {
                        hT[new_numberLocation] = new_newNode;
                    }
                    else {
                        while(new_headNode->next != nullptr) {
                            new_headNode = new_headNode->next;
                        }
                        new_headNode->next = new_newNode;
                    }
                    currentHeadNode = currentHeadNode->next;
                }
            }
            for(int i=0; i < old_amountOfBuckets; i++) {
                while(hashTable[i] != nullptr) {
                    Node* currentHeadNode = hashTable[i];
                    hashTable[i] = currentHeadNode->next;
                    delete currentHeadNode;
                }
            }
            delete[] hashTable;
            hashTable = hT;
        }
    }
}

// contains() returns true if the given element is already in the set,
// false otherwise.  This function runs in constant time (with respect
// to the number of elements, assuming a good hash function).
template <typename ElementType>
bool HashSet<ElementType>::contains(const ElementType& element) const
{
    Node* currentHeadNode = hashTable[hashFunction(element) % amountOfBuckets];
    while(currentHeadNode != nullptr) {
        if(currentHeadNode->data == element) {
            return true;
        }
        currentHeadNode = currentHeadNode->next;
    }
    return false;
}


template <typename ElementType>
unsigned int HashSet<ElementType>::size() const noexcept
{
    return sz;
}

// elementsAtIndex() returns the number of elements that hashed to a
// particular index in the array.  If the index is out of the boundaries
// of the array, this function returns 0.
template <typename ElementType>
unsigned int HashSet<ElementType>::elementsAtIndex(unsigned int index) const
{
    if(index < 0 || index >= amountOfBuckets) {
        return 0;
    }
    else {
        int returnValue = 0;
        Node* temp_currentNode = hashTable[index];
        while(temp_currentNode != nullptr) {
            returnValue++;
            temp_currentNode = temp_currentNode->next;
        }
        return returnValue;
    }
}

// isElementAtIndex() returns true if the given element hashed to a
// particular index in the array, false otherwise.  If the index is
// out of the boundaries of the array, this functions returns 0.
template <typename ElementType>
bool HashSet<ElementType>::isElementAtIndex(const ElementType& element, unsigned int index) const
{
    if(index < 0 || index > amountOfBuckets) {
        return 0;
    }
    else {
        Node* temp_currentNode = hashTable[index];
        while(temp_currentNode != nullptr) {
            if(temp_currentNode->data == element) {
                return true;
            }
            temp_currentNode = temp_currentNode->next;
        }
        return false;
    }
}



#endif // HASHSET_HPP

