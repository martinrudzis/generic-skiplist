/*
SkipList.h
Created by Martin Rudzis
5/5/2021

Input & output: Constructor accepts any number small enough to not cause an std::bad_alloc exception. 
After initialization, insert, erase, and contains may be used with any data type. insert and contains 
return true if the parameter was successfully inserted or erased. Contains returns true if the parameter 
exists in the SkipList.
Program use: 
To use the program, include SkipList.h in the driver file. 
Initialization: SkipList objectName(<number of levels>); OR SkipList objectName; (This uses the default 
number of levels, which is equal to 1.) Use of initializer_list is also supported.
Insertion: objectName.insert(type); Only unique values may be added to the SkipList
Deletion: objectName.erase(type);
Check if item is in SkipList: objectName.contains(int value);
Assumptions:
In the constructor, it is assumed that the user will provide integer values that are greater than 0 
and substantially less than the maximum value allowed by int, since large values (~1,000,000) will result 
in a bad_alloc exception. Bad input in the constructor will cause program exit.
Description:
The aim of this data structure is to create an STL-compliant container that approximates a combination of the 
structural advantages of a linked list with the accessibility of an array. 
Since this data structure uses a structure comparable to a doubly-linked list, a node object will be used 
to store values and form doubly-linked connections between other nodes. This node will support up to four 
connections: back, front, above, and below, and also contain an integer data field.
The SkipList must be initialized by providing an integer value N for the number of levels to include in the 
SkipList. The SkipList will use arrays whose sizes are equal to N and whose indices form the individual 
levels of the SkipList. Adding values to the SkipList requires an insertion function. The workings of this 
method will capitalize off of the tiered structure of the SkipList to speed operations and thereby 
approximate the instant accessibility of an array. It should insert the new item at the lowest level of 
the SkipList, such that the items at that level are in sorted order. 
Once a new item is added to the SkipList, it must be determined whether to add to the item to the level 
above. This must be determined by chance such that there is a 50% chance of adding a item to the level 
immediately above. If it is determined that it should add a value above, then this will be done such that 
the level remains in sorted order. This process is then repeated at the next level. This continues up until 
it either adds the item to the highest level or determines not to add any more instances of the item.
Since the levels and nodes contained in the SkipList are determined at runtime, this data structure will 
require dynamic memory allocation. Hence, the destructor will need to directly deallocate the contents of 
the SkipList.
*/

#pragma once

#include <iostream>
#include <vector>
#include <random>
#include <limits>
#include <iterator>
#include <cstddef>

using namespace std;

template<class T>

class SkipList {


   // Function purpose: Print a SkipList using cout <<
   // Parameters: N/A
   // Preconditions: An initialized SkipList
   // Postconditions: Each level of the SkipList is printed to the console
   // Return value: ostream
   // Functions called: N/A
   template<class TypeName>
   friend ostream& operator<<(ostream &out, const SkipList<TypeName> &skip); 

private:
   
   struct SkipListNode {
      // Function purpose: Initilize a SkipListNode
      // Parameters: A data type<T>
      // Preconditions: Any numerical data types are less than its extreme values
      // Postconditions: An initilized SkipListNode
      // Return value: N/A
      // Functions called: N/A
      explicit SkipListNode(T data);
      T data_;
      SkipListNode* next_;
      SkipListNode* prev_;
      SkipListNode* upLevel_;
      SkipListNode* downLevel_;
   };

   int maxLevel_;
   int size_;
   SkipListNode** heads_;
   SkipListNode** tails_;

   // Function purpose: To insert a SkipListNode at a specific level in the SkipList
   // Parameters: Two SkipListNode pointers, and two data types
   // Preconditions: Pointers are not nullptrs and int level is > 0 
   // Postconditions: A SkipListNode is added to the level
   // Return value: void
   // Functions called: addBefore
   void insertAtLevel(SkipListNode *newValue, SkipListNode *beforeNode, int const level, const T data);

   // Function purpose: To determine whether to add a SkipListNode to the next level
   // Parameters: N/A
   // Preconditions: N/A
   // Postconditions: 1 or 0 has been returned, with a 50% chance of either
   // Return value: bool
   // Functions called: N/A
   bool alsoHigher() const;

   // Function purpose: To insert a SkipListNode above a particular level in the SkipList
   // Parameters: an data type representing the itemto be added, a SkipListNode pointer, and a reference to 
   // a vector of before nodes
   // Preconditions: An item has been added to a SkipList
   // Postconditions: A SkipListNode is added to the level above
   // Return value: void
   // Functions called: alsoHigher, insertAtLevel, addAbove
   void insertHigher(const T data, SkipListNode *base, vector<SkipListNode*>& beforeNodes);

   // Function purpose: To connect two SkipLIstNodes using prev_ and next_ pointers
   // Parameters: Two SkipListNode pointers
   // Preconditions: Parameters are not nullptrs
   // Postconditions: Two horizontally connected SkipListNodes
   // Return value: void
   // Functions called: N/A
   static void addBefore(SkipListNode *newNode, SkipListNode *nextNode);

   // Function purpose: To connect two SkipLIstNodes using upLevel_ and downLevel_ pointers
   // Parameters: Two SkipListNode pointers
   // Preconditions: Parameters are not nullptrs
   // Postconditions: Two vertically connected SkipListNodes
   // Return value: void
   // Functions called: N/A
   static void addAbove(SkipListNode *newNode, SkipListNode *belowNode);

   // Function purpose: To copy the items in one SkipList and add them to the current object
   // Parameters: An initialized SkipList
   // Preconditions: SkipList is empty
   // Postconditions: SkipList container the same items as the parameter  
   // Return value: void
   // Functions called: N/A
   void copyContents(const SkipList<T>& skip);

   // Function purpose: To initilize heads_ and tails_ pointers that were set to nullptr
   // Parameters: An int for the number of levels
   // Preconditions: Positive, non-zero parameter
   // Postconditions: Fields of current object have been reinitialized
   // Return value: void
   // Functions called: N/A
   void initializeFields(const int maxLevel);

public:

   class iterator {
      public:
      using iterator_category = std::forward_iterator_tag;
      using difference_type = std::ptrdiff_t;
      using value_type = T;
      using pointer = const T*;
      using reference = const T&;
      
      // Function purpose: To construct an iterator for the SkipList
      // Parameters: A pointer to a SkipList and a pointer to one of its SkipListNodes
      // Preconditions: Initialized SkipList
      // Postconditions: Iterator located at SkipListNode in SkipList
      // Return value: N/A
      // Functions called: N/A
      iterator(SkipList<T> *skpPtr_, SkipListNode *nodePtr_ = nullptr);

      // Function purpose: To return the item stored in nodePtr_
      // Parameters: N/A
      // Preconditions: Initialized iterator
      // Postconditions: Item stored at location in SkipList
      // Return value: const referenst to type <T>
      // Functions called: N/A
      const T& operator*() const;

      // Function purpose: To increment the iterator to the item in nodePtr->next_ (prefix incrementation)
      // Parameters: N/A
      // Preconditions: Initialized iterator
      // Postconditions: Incremented iterator 
      // Return value: iterator
      // Functions called: N/A
      iterator& operator++();

      // Function purpose: To return the current iterator, then increment it to the item in nodePtr->next_ (postfix incrementation)
      // Parameters: N/A
      // Preconditions: Initialized iterator
      // Postconditions: Iterator located at the current position of iterator before it was incremented
      // Return value: iterator
      // Functions called: N/A
      iterator operator++(int);

      // Function purpose: To set current iterator to the same location as another iterator
      // Parameters: N/A
      // Preconditions: An initialized iterator
      // Postconditions: Iterator located at the same position as its right-hand argument 
      // Return value: iterator reference
      // Functions called: N/A      
      iterator& operator=(const iterator& rhs);

      // Function purpose: To determine whether two iterators are at the same location (equal)
      // Parameters: N/A
      // Preconditions: An initialized iterator
      // Postconditions: bool reflecting whether two iterators are at the same location
      // Return value: bool
      // Functions called: N/A
      bool operator==(const iterator& rhs) const;

      // Function purpose: To determine whether two iterators are not at the same location (not equal)
      // Parameters: N/A
      // Preconditions: An initialized iterator
      // Postconditions: bool reflecting whether two iterators are not at the same location
      // Return value: bool
      // Functions called: N/A
      bool operator!=(const iterator& rhs) const;
      private:
      SkipList<T> *skpPtr_;
      SkipListNode *nodePtr_;
   };

   // Function purpose: To initialize a SkipList object
   // Parameters: A data type <T>
   // Preconditions: skiplist.h has been included in the driver file
   // Postconditions: An initilized SkipList 
   // Return value: N/A
   // Functions called: N/A
   explicit SkipList(int maxLevel = 1);

   // Function purpose: To initialilize a SkipList using initializer_list
   // Parameters: an initilization list
   // Preconditions: N/A
   // Postconditions: initilized Skiplist
   // Return value: N/A
   // Functions called: N/A
   SkipList(initializer_list<T> vals);

   // Function purpose: To destroy a SkipList object after program execution
   // Parameters: N/A
   // Preconditions: An initilized SkipList 
   // Postconditions: SkipList no longer exists in memory
   // Return value: N/A
   // Functions called: erase
   virtual ~SkipList();

   // Function purpose: To copy-construct a SkipList
   // Parameters: Skiplist object
   // Preconditions: Initialized SkipList
   // Postconditions: copied Skiplist
   // Return value: N/A
   // Functions called: copyContents
   SkipList(const SkipList& toCopy);

   // Function purpose: To allow a SkipList to be assigned using =
   // Parameters: Skiplist as right hand argument
   // Preconditions: Initialized SkipLists
   // Postconditions: reassigned Skiplist
   // Return value: reference to SkipList
   // Functions called: initilizeFields
   SkipList& operator=(const SkipList& rhs);

   // Function purpose: To move-construct a SkipList
   // Parameters: moved Skiplist
   // Preconditions: Initialized SkipList
   // Postconditions: Move constructed Skiplist
   // Return value: reference to SkipList
   // Functions called: N/A
   SkipList(SkipList&& toMove);

   // Function purpose: To move-assign a SkipList
   // Parameters: moved Skiplist as right hand argument
   // Preconditions: Initialized SkipLists
   // Postconditions: Moved Skiplist
   // Return value: reference to SkipList
   // Functions called: initilizeFields
   SkipList& operator=(SkipList&& rhs);

   // Function purpose: To determine if two SkipLists are equal
   // Parameters: Skiplist as right hand argument
   // Preconditions: Initialized SkipLists
   // Postconditions: bool reflecting whether objects are equal
   // Return value: N/A
   // Functions called: N/A
   bool operator==(const SkipList& rhs) const;

   // Function purpose: To determine if two SkipLists are not equal
   // Parameters: Skiplist as right hand argument
   // Preconditions: Initialized SkipLists
   // Postconditions: bool reflecting whether objects are not equal
   // Return value: N/A
   // Functions called: operator==
   bool operator!=(const SkipList& rhs) const;

   // Function purpose: To add a SkipListNode containing a unique item to a SkipList
   // Parameters: An item to add to a SkipListNode that is placed in the SkipList
   // Preconditions: Any numerical parameters are less than extreme values; an initilized SkipList
   // Postconditions: A bool reflecting the success of the insertion operation
   // Return value: bool
   // Functions called: contains, insertAtLevel, insertHigher
   bool insert(T data);

   // Function purpose: To check if the SkipList contains a given item
   // Parameters: An item to search for 
   // Preconditions: Numerical parameters are less than extreme values 
   // Postconditions: A bool reflecting the existance of the object in the SkipList
   // Return value: bool
   // Functions called: N/A
   bool contains(T data) const;

   // Function purpose: To remove a SkipListNode containing a given item from the SkipList
   // Parameters: An item that is stored in a SkipListNode within the SkipList
   // Preconditions: Parameter exists in the SkipList; an initilized SkipList
   // Postconditions: A bool reflecting the success of the erase operation
   // Return value: bool
   // Functions called: contains, addBefore
   bool erase(T data);

   // Function purpose: To return the number of items in the base level of the SkipList
   // Parameters: N/A
   // Preconditions: Initialized SkipList
   // Postconditions: An integer representing the amount of items in the SkipList
   // Return value: int
   // Functions called: N/A
   int size() const;

   // Function purpose: To remove the contents of a SkipList
   // Parameters: N/A
   // Preconditions: Initialized SkipList
   // Postconditions: Empty SkipList
   // Return value: N/A
   // Functions called: empty(), erase
   void clear();

   // Function purpose: To indicate whether a SkipList is empty
   // Parameters: N/A
   // Preconditions: Initialized SkipList
   // Postconditions: A bool reflecting whether SkipList is empty
   // Return value: bool
   // Functions called: N/A
   bool empty() const;

   // Function purpose: To return an iterator set to the beginning of the lowest level of the SkipList
   // Parameters: N/A
   // Preconditions: Initialized SkipList
   // Postconditions: An iterator at the beginning of the SkipList
   // Return value: iterator
   // Functions called: iterator()
   iterator begin() const;

   // Function purpose: To return an iterator set to the end of the lowest level of the SkipList
   // Parameters: N/A
   // Preconditions: Initialized SkipList
   // Postconditions: An iterator at the end of the SkipList
   // Return value: iterator
   // Functions called: iterator()
   iterator end() const;

};

#include "SkipList.cpp"