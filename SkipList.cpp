/*
SkipList.cpp
Created by Martin Rudzis
5/5/2021
*/

// Function purpose: Print a SkipList using cout <<
// Parameters: N/A
// Preconditions: An initialized SkipList
// Postconditions: Each level of the SkipList is printed to the console
// Return value: ostream
// Functions called: N/A
template<class T>
SkipList<T>::SkipListNode::SkipListNode(T data) {
   this->data_ = data;
   this->prev_ = nullptr;
   this->next_ = nullptr;
   this->upLevel_ = nullptr;
   this->downLevel_ = nullptr;
}

// Function purpose: To construct an iterator for the SkipList
// Parameters: A pointer to a SkipList and a pointer to one of its SkipListNodes
// Preconditions: Initialized SkipList
// Postconditions: Iterator located at SkipListNode in SkipList
// Return value: N/A
// Functions called: N/A
template<class T>
SkipList<T>::iterator::iterator(SkipList<T> *skpPtr, SkipListNode *nodePtr) {
   this->skpPtr_ = skpPtr;
   this->nodePtr_ = nodePtr;
}

// Function purpose: To return the item stored in nodePtr_
// Parameters: N/A
// Preconditions: Initialized iterator
// Postconditions: Item stored at location in SkipList
// Return value: const referenst to type <T>
// Functions called: N/A
template<class T>
const T& SkipList<T>::iterator::operator*() const {
   return nodePtr_->data_;
}

// Function purpose: To increment the iterator to the item in nodePtr->next_ (prefix incrementation)
// Parameters: N/A
// Preconditions: Initialized iterator
// Postconditions: Incremented iterator 
// Return value: iterator
// Functions called: N/A
template<class T>
typename SkipList<T>::iterator& SkipList<T>::iterator::operator++() {
   nodePtr_ = nodePtr_->next_;
   return (*this);
}

// Function purpose: To return the current iterator, then increment it to the item in nodePtr->next_ (postfix incrementation)
// Parameters: N/A
// Preconditions: Initialized iterator
// Postconditions: Iterator located at the current position of iterator before it was incremented
// Return value: iterator
// Functions called: N/A
template<class T>
typename SkipList<T>::iterator SkipList<T>::iterator::operator++(int) {
   iterator temp = *this;
   ++(*this);
   return temp;
}

// Function purpose: To set current iterator to the same location as another iterator
// Parameters: N/A
// Preconditions: An initialized iterator
// Postconditions: Iterator located at the same position as its right-hand argument 
// Return value: iterator reference
// Functions called: N/A      
template<class T>
typename SkipList<T>::iterator& SkipList<T>::iterator::operator=(const iterator &rhs) {
   this->skpPtr_ = rhs.skpPtr_;
   this->nodePtr_ = rhs.nodePtr_;
   return (*this);
}

// Function purpose: To determine whether two iterators are at the same location (equal)
// Parameters: N/A
// Preconditions: An initialized iterator
// Postconditions: bool reflecting whether two iterators are at the same location
// Return value: bool
// Functions called: N/A
template<class T>
bool SkipList<T>::iterator::operator==(const iterator& rhs) const {
   return (skpPtr_ == rhs.skpPtr_) && (nodePtr_ == rhs.nodePtr_);
}

// Function purpose: To determine whether two iterators are not at the same location (not equal)
// Parameters: N/A
// Preconditions: An initialized iterator
// Postconditions: bool reflecting whether two iterators are not at the same location
// Return value: bool
// Functions called: N/A
template<class T>
bool SkipList<T>::iterator::operator!=(const iterator& rhs) const {
   return (skpPtr_ == rhs.skpPtr_) && !(nodePtr_ == rhs.nodePtr_);
}

// Function purpose: Print a SkipList using cout <<
// Parameters: N/A
// Preconditions: An initialized SkipList
// Postconditions: Each level of the SkipList is printed to the console
// Return value: ostream
// Functions called: N/A
template<class TypeName>
ostream &operator<<(ostream &out, const SkipList<TypeName> &skp) {
   for (int i = skp.maxLevel_ - 1; i >= 0; i--) {
      cout << "Level: " << i << " -- ";
      typename SkipList<TypeName>::SkipListNode *curr = skp.heads_[i];
      if (curr == nullptr) {cout << "empty";}
      while (curr != nullptr) {
         if (curr->next_ == nullptr) { cerr << curr->data_; } 
         else { cerr << curr->data_ << ", "; }
         curr = curr->next_;
      }
      cout << endl;
   }
   return out;
}

// Function purpose: To initialize a SkipList object
// Parameters: A data type <T>
// Preconditions: skiplist.h has been included in the driver file
// Postconditions: An initilized SkipList 
// Return value: N/A
// Functions called: N/A
template<class T>
SkipList<T>::SkipList(int maxLevel) {
   try 
   {
      if (maxLevel <= 0) {
         cout << "Error: Invalid number of levels" << endl;
         exit(1);
      } 
      else {
         maxLevel_ = maxLevel;
         size_ = 0;
         heads_ = new SkipListNode*[maxLevel_];
         tails_ = new SkipListNode*[maxLevel_];

         for (int i = 0; i < maxLevel_; i++) {
            heads_[i] = nullptr;
            tails_[i] = nullptr;
         }
      }
   }
   catch(exception &e) 
   {
      cout << "Error: Invalid number of levels" << endl;
      exit(1);
   }
}

// Function purpose: To initialilize a SkipList using initializer_list
// Parameters: an initilization list
// Preconditions: N/A
// Postconditions: initilized Skiplist
// Return value: N/A
// Functions called: N/A
template<class T> 
SkipList<T>::SkipList(initializer_list<T> vals) {
   maxLevel_ = 1;
   size_ = 0;
   heads_ = new SkipListNode*[maxLevel_];
   tails_ = new SkipListNode*[maxLevel_];
   for (int i = 0; i < maxLevel_; i++) {
      heads_[i] = nullptr;
      tails_[i] = nullptr;
   }
   if (vals.size() > 0) {
      for (T i : vals) {
         insert(i);
      }
   } 
}

// Function purpose: To destroy a SkipList object after program execution
// Parameters: N/A
// Preconditions: An initilized SkipList 
// Postconditions: SkipList no longer exists in memory
// Return value: N/A
// Functions called: erase
template<class T>
SkipList<T>::~SkipList() {
   if (heads_ != nullptr && tails_ != nullptr) { clear(); }
   delete[] heads_;
   delete[] tails_;
   heads_ = nullptr;
   tails_ = nullptr;
}

// Function purpose: To initilize heads_ and tails_ pointers that were set to nullptr
// Parameters: An int for the number of levels
// Preconditions: Positive, non-zero parameter
// Postconditions: Fields of current object have been reinitialized
// Return value: void
// Functions called: N/A
template<class T>
void SkipList<T>::initializeFields(const int maxLevel) {
   maxLevel_ = maxLevel;
   size_ = 0;
   delete[] heads_;
   delete[] tails_;
   heads_ = new SkipListNode*[maxLevel_];
   tails_ = new SkipListNode*[maxLevel_];

   for (int i = 0; i < maxLevel_; i++) {
      heads_[i] = nullptr;
      tails_[i] = nullptr;
   }
}

// Function purpose: To copy-construct a SkipList
// Parameters: Skiplist object
// Preconditions: Initialized SkipList
// Postconditions: copied Skiplist
// Return value: N/A
// Functions called: copyContents
template<class T>
SkipList<T>::SkipList(const SkipList<T>& toCopy) {
   maxLevel_ = toCopy.maxLevel_;
   size_ = 0;
   heads_ = new SkipListNode*[maxLevel_];
   tails_ = new SkipListNode*[maxLevel_];

   for (int i = 0; i < maxLevel_; i++) {
      heads_[i] = nullptr;
      tails_[i] = nullptr;
   }
   copyContents(toCopy);
}

// Function purpose: To allow a SkipList to be assigned using =
// Parameters: Skiplist as right hand argument
// Preconditions: Initialized SkipLists
// Postconditions: reassigned Skiplist
// Return value: reference to SkipList
// Functions called: initilizeFields
template<class T>
SkipList<T>& SkipList<T>::operator=(const SkipList<T>& rhs) {
    if (this != &rhs) {
      if (heads_ == nullptr && tails_ == nullptr) {
         initializeFields(rhs.maxLevel_);
      }
      else {
         clear();
      }
      copyContents(rhs);
   }
   return *this;
}

// Function purpose: To move-construct a SkipList
// Parameters: moved Skiplist
// Preconditions: Initialized SkipList
// Postconditions: Move constructed Skiplist
// Return value: reference to SkipList
// Functions called: N/A
template<class T>
SkipList<T>::SkipList(SkipList<T>&& toMove) {
   size_ = toMove.size_;
   maxLevel_ = toMove.maxLevel_;
   heads_ = toMove.heads_;
   tails_ = toMove.tails_;
   toMove.size_ = 0;
   toMove.maxLevel_ = 0;
   toMove.heads_ = nullptr;
   toMove.tails_ = nullptr;
}

// Function purpose: To move-assign a SkipList
// Parameters: moved Skiplist as right hand argument
// Preconditions: Initialized SkipLists
// Postconditions: Moved Skiplist
// Return value: reference to SkipList
// Functions called: initilizeFields
template<class T>
SkipList<T>& SkipList<T>::operator=(SkipList<T>&& rhs) {
   if (this != &rhs) {
      if (heads_ == nullptr && tails_ == nullptr) {
         initializeFields(rhs.maxLevel_);
      }
      else {
         clear();
      }
      delete[] heads_;
      delete[] tails_;
      size_ = rhs.size_;
      maxLevel_ = rhs.maxLevel_;
      heads_ = rhs.heads_;
      tails_ = rhs.tails_;
      rhs.size_ = 0;
      rhs.maxLevel_ = 0;
      rhs.heads_ = nullptr;
      rhs.tails_ = nullptr;
   }
   return *this;
}

// Function purpose: To determine if two SkipLists are equal
// Parameters: Skiplist as right hand argument
// Preconditions: Initialized SkipLists
// Postconditions: bool reflecting whether objects are equal
// Return value: N/A
// Functions called: N/A
template<class T>
bool SkipList<T>::operator==(const SkipList<T>& rhs) const {
   const SkipListNode * ptr1 = this->heads_[0];
   const SkipListNode *ptr2 = rhs.heads_[0];
   while (ptr1 != nullptr && ptr2 != nullptr) {
      if (ptr1->data_ != ptr2->data_) { return false; }
      ptr1 = ptr1->next_;
      ptr2 = ptr2->next_;
   }
   if (ptr1 == nullptr || ptr2 == nullptr) { return ptr1 == ptr2; }
   return true;
}

// Function purpose: To determine if two SkipLists are not equal
// Parameters: Skiplist as right hand argument
// Preconditions: Initialized SkipLists
// Postconditions: bool reflecting whether objects are not equal
// Return value: N/A
// Functions called: operator==
template<class T>
bool SkipList<T>::operator!=(const SkipList<T>& rhs) const{
   return !(*this == rhs);
}

// Function purpose: To insert a SkipListNode at a specific level in the SkipList
// Parameters: Two SkipListNode pointers, and two data types
// Preconditions: Pointers are not nullptrs and int level is > 0 
// Postconditions: A SkipListNode is added to the level
// Return value: void
// Functions called: addBefore
template<class T>
void SkipList<T>::insertAtLevel(SkipListNode *newValue, SkipListNode *beforeNode, int const level, const T data) {
   if (beforeNode == nullptr) { 
      heads_[level] = newValue; 
      tails_[level] = newValue;
   }
   else if (beforeNode->data_ > data) {
      SkipListNode *nextNode = beforeNode;
      heads_[level] = newValue;
      addBefore(newValue, nextNode);
   }
   else if (beforeNode->next_ == nullptr) {
      tails_[level] = newValue;
      addBefore(beforeNode, newValue);
   }
   else {
      SkipListNode *nextNode = beforeNode->next_;
      addBefore(beforeNode, newValue);
      addBefore(newValue, nextNode);
   }
}

// Function purpose: To determine whether to add a SkipListNode to the next level
// Parameters: N/A
// Preconditions: N/A
// Postconditions: 1 or 0 has been returned, with a 50% chance of either
// Return value: bool
// Functions called: N/A
template<class T>
bool SkipList<T>::alsoHigher() const {
   static default_random_engine generator;    
   static bernoulli_distribution coin(0.5); 
   return coin(generator);
}

// Function purpose: To insert a SkipListNode above a particular level in the SkipList
// Parameters: an data type representing the itemto be added, a SkipListNode pointer, and a reference to 
// a vector of before nodes
// Preconditions: An item has been added to a SkipList
// Postconditions: A SkipListNode is added to the level above
// Return value: void
// Functions called: alsoHigher, insertAtLevel, addAbove
template<class T>
void SkipList<T>::insertHigher(const T data, SkipListNode *base, vector<SkipListNode*>& beforeNodes) {
   int currentLevel = 1;
   while (alsoHigher() && currentLevel < maxLevel_) {
      SkipListNode *newValue = new SkipListNode(data);
      insertAtLevel(newValue, beforeNodes[currentLevel], currentLevel, data);
      addAbove(newValue, base);
      currentLevel++;
      base = newValue;
   }
}

// Function purpose: To connect two SkipLIstNodes using prev_ and next_ pointers
// Parameters: Two SkipListNode pointers
// Preconditions: Parameters are not nullptrs
// Postconditions: Two horizontally connected SkipListNodes
// Return value: void
// Functions called: N/A
template<class T>
void SkipList<T>::addBefore(SkipListNode *newNode, SkipListNode *nextNode) {
   if (newNode != nullptr && nextNode != nullptr) {
      newNode->next_ = nextNode;
      nextNode->prev_ = newNode;
   }
}

// Function purpose: To connect two SkipLIstNodes using upLevel_ and downLevel_ pointers
// Parameters: Two SkipListNode pointers
// Preconditions: Parameters are not nullptrs
// Postconditions: Two vertically connected SkipListNodes
// Return value: void
// Functions called: N/A
template<class T>
void SkipList<T>::addAbove(SkipListNode *newNode, SkipListNode *belowNode) {
   if (newNode != nullptr && belowNode != nullptr) {
      newNode->downLevel_ = belowNode;
      belowNode->upLevel_ = newNode;
   }
}

// Function purpose: To add a SkipListNode containing a unique item to a SkipList
// Parameters: An item to add to a SkipListNode that is placed in the SkipList
// Preconditions: Any numerical parameters are less than extreme values; an initilized SkipList
// Postconditions: A bool reflecting the success of the insertion operation
// Return value: bool
// Functions called: contains, insertAtLevel, insertHigher
template<class T>
bool SkipList<T>::insert(T data) {
   bool containsValue = contains(data);
   if (containsValue) {
      cout << "Error: '" << data << "' has already been added" << endl;
   }
   else {
      SkipListNode *newValue = new SkipListNode(data);
      int currentIndex = maxLevel_ - 1;
      SkipListNode *curr = heads_[currentIndex];
      vector <SkipListNode*> beforeNodes(maxLevel_);

      while ((curr == nullptr || curr->data_ > data) && currentIndex > 0) {
         beforeNodes[currentIndex] = curr;
         currentIndex--;
         curr = heads_[currentIndex];
      }
      if (curr != nullptr) {
         for (int i = currentIndex; i >= 0; i--) {
            while (curr->next_ != nullptr && curr->next_->data_ < data) { 
               curr = curr->next_; 
            }
            beforeNodes[i] = curr;
            if (i > 0) { curr = curr->downLevel_; }
         }
      }
      insertAtLevel(newValue, beforeNodes[0], 0, data);
      insertHigher(data, newValue, beforeNodes);
      size_ += 1;
   }
   return containsValue;
}

// Function purpose: To check if the SkipList contains a given item
// Parameters: An item to search for 
// Preconditions: Numerical parameters are less than extreme values 
// Postconditions: A bool reflecting the existance of the object in the SkipList
// Return value: bool
// Functions called: N/A
template<class T>
bool SkipList<T>::contains(T data) const {
  int topIndex = this->maxLevel_ - 1;
   SkipListNode *curr = this->heads_[topIndex];
   int currentIndex = topIndex;
   while ((curr == nullptr || curr->data_ > data) && currentIndex > 0) {
      currentIndex--;
      curr = this->heads_[currentIndex];
   }
   for (int i = currentIndex; i >= 0; i--) {
      if (curr != nullptr && curr->data_ <= data) {     
         while (curr->next_ != nullptr && curr->next_->data_ <= data) {
            curr = curr->next_; 
         }
         if (curr->data_ == data) { return true; }
         else if (i > 0) { curr = curr->downLevel_; }
      }
   }
   return false; 
}

// Function purpose: To remove a SkipListNode containing a given item from the SkipList
// Parameters: An item that is stored in a SkipListNode within the SkipList
// Preconditions: Parameter exists in the SkipList; an initilized SkipList
// Postconditions: A bool reflecting the success of the erase operation
// Return value: bool
// Functions called: contains, addBefore
template<class T>
bool SkipList<T>::erase(T data) {
   bool containsValue = contains(data);
   if (!containsValue) {
      cout << "Error: Does not contain value to be erased" << endl;
   }
   else {
      int currentIndex = maxLevel_ - 1;
      SkipListNode *curr = heads_[currentIndex];
      vector <SkipListNode*> locations(maxLevel_);
      
      while (curr == nullptr || curr->data_ > data) {
         currentIndex--;
         curr = heads_[currentIndex];
      }
      for (int i = currentIndex; i >= 0; i--) {
         while (curr->next_ != nullptr && curr->next_->data_ <= data) { 
            curr = curr->next_; 
         }
         if (curr->data_ == data) { locations[i] = (curr); }
         if (i > 0) { curr = curr->downLevel_; }
      }

      for (int i = locations.size() - 1; i >= 0; i--) {
         if (locations[i] != nullptr) {
            SkipListNode *toDelete = locations[i];
            if (toDelete->next_ == nullptr && toDelete->prev_ == nullptr) {
               heads_[i] = nullptr;
               tails_[i] = nullptr;   
            }
            else if (toDelete->next_ == nullptr) {
               SkipListNode *endNode = toDelete->prev_;
               endNode->next_ = nullptr;
               tails_[i] = endNode;
            }
            else if (toDelete->prev_ == nullptr) {
               SkipListNode *firstNode = toDelete->next_;
               firstNode->prev_ = nullptr;
               heads_[i] = firstNode;
            }
            else {
               SkipListNode *previous = toDelete->prev_;
               addBefore(previous, toDelete->next_);
            }
            delete toDelete;
            toDelete = nullptr;
         }
      }
      size_ -= 1;
   }
   return containsValue;
}


// Function purpose: To return the number of items in the base level of the SkipList
// Parameters: N/A
// Preconditions: Initialized SkipList
// Postconditions: An integer representing the amount of items in the SkipList
// Return value: int
// Functions called: N/A
template<class T>
int SkipList<T>::size() const {
   return size_;
}

// Function purpose: To copy the items in one SkipList and add them to the current object
// Parameters: An initialized SkipList
// Preconditions: SkipList is empty
// Postconditions: SkipList container the same items as the parameter  
// Return value: void
// Functions called: N/A
template<class T>
void SkipList<T>::copyContents(const SkipList<T>& skip) {
   const SkipListNode *curr = skip.heads_[0];
   while (curr != nullptr) {
      insert(curr->data_);
      curr = curr->next_;
   }
}

// Function purpose: To remove the contents of a SkipList
// Parameters: N/A
// Preconditions: Initialized SkipList
// Postconditions: Empty SkipList
// Return value: N/A
// Functions called: empty(), erase
template<class T>
void SkipList<T>::clear() {
   if (heads_ != nullptr && tails_ != nullptr) {
      while (!empty()) {
         erase(heads_[0]->data_);
      }
   }
}

// Function purpose: To indicate whether a SkipList is empty
// Parameters: N/A
// Preconditions: Initialized SkipList
// Postconditions: A bool reflecting whether SkipList is empty
// Return value: bool
// Functions called: N/A
template<class T>
bool SkipList<T>::empty() const {
   bool isEmpty = true;
   for (int i = 0; i < maxLevel_; i++) {
      const SkipListNode *curr = heads_[i];
      if (curr != nullptr) { 
         isEmpty = false;
         return isEmpty; 
      }
   }
   return isEmpty;
}

// Function purpose: To return an iterator set to the beginning of the lowest level of the SkipList
// Parameters: N/A
// Preconditions: Initialized SkipList
// Postconditions: An iterator at the beginning of the SkipList
// Return value: iterator
// Functions called: iterator()
template<class T>
typename SkipList<T>::iterator SkipList<T>::begin() const {
   return iterator(const_cast<SkipList<T>*>(this), const_cast<SkipListNode*>(this->heads_[0]));
}

// Function purpose: To return an iterator set to the end of the lowest level of the SkipList
// Parameters: N/A
// Preconditions: Initialized SkipList
// Postconditions: An iterator at the end of the SkipList
// Return value: iterator
// Functions called: iterator()
template<class T>
typename SkipList<T>::iterator SkipList<T>::end() const {
   return iterator(const_cast<SkipList<T>*>(this), nullptr);
}