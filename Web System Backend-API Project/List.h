#ifndef LIST_H
#define LIST_H
/* List.h
 *
 * doubly-linked, double-ended list with Iterator interface
 * Project UID c1f28c309e55405daf00c565d57ff9ad
 * EECS 280 Project 4
 */

#include <iostream>
#include <cassert> //assert
#include <cstddef> //NULL


template <typename T>
class List {
  //OVERVIEW: a doubly-linked, double-ended list with Iterator interface
public:

  List() : list_size(0), first(nullptr), last(nullptr) {}
  
  //EFFECTS:  returns true if the list is empty
  bool empty() const{
    return first == nullptr;
  }

  //EFFECTS: returns the number of elements in this List
  //HINT:    Traversing a list is really slow.  Instead, keep track of the size
  //         with a private member variable.  That's how std::list does it.
  int size() const {
    return list_size; 
  }

  //REQUIRES: list is not empty
  //EFFECTS: Returns the first element in the list by reference
  T & front(){
    assert(!empty());
    return first->datum;
  }

  //REQUIRES: list is not empty
  //EFFECTS: Returns the last element in the list by reference
  T & back(){
    assert(!empty());
    return last->datum;
  }

  //EFFECTS:  inserts datum into the front of the list
  void push_front(const T &datum){
    Node *p = new Node;
    p->datum = datum;
    p->prev = nullptr;
    p->next = first;
    if(first == nullptr){
      last = first = p;
    }
    else{
      first->prev = p;
      first = p;
    }
    ++list_size;
  }

  //EFFECTS:  inserts datum into the back of the list
  void push_back(const T &datum){
    Node *p = new Node;
    p->datum = datum;
    p->next = nullptr;
    p->prev = last;
    if(last == nullptr){
      first = last = p;
    }
    else{
      last->next = p;
      last = p;
    }
    ++list_size; 
  }

  //REQUIRES: list is not empty
  //MODIFIES: may invalidate list iterators
  //EFFECTS:  removes the item at the front of the list
  void pop_front(){
    assert(!empty());
    Node *popped = first;
    first = first->next;
    --list_size; 
    delete popped;
  }

  //REQUIRES: list is not empty
  //MODIFIES: may invalidate list iterators
  //EFFECTS:  removes the item at the back of the list
  void pop_back(){
    assert(!empty());
    Node *popped = last;
    last = last->prev;
    --list_size;
    if(list_size > 0){
      last->next = nullptr;
    }
    else{
      first = nullptr;
    }
    delete popped;
  }

  //MODIFIES: may invalidate list iterators
  //EFFECTS:  removes all items from the list
  void clear(){
    while(!empty()){
      pop_front();
    }
  }

  // You should add in a default constructor, destructor, copy constructor,
  // and overloaded assignment operator, if appropriate. If these operations
  // will work correctly without defining these, you can omit them. A user
  // of the class must be able to create, copy, assign, and destroy Lists
  ~List(){
    clear();
  }

  List(const List &other){
    first = nullptr; 
    last = nullptr;
    copy_all(other);
  }

  List & operator=(const List &rhs){
    if (this == &rhs) return *this;
    clear();
    copy_all(rhs);
    return *this;
  } 

private:
  //a private type
  int list_size;
  
  struct Node {
    Node *next;
    Node *prev;
    T datum;
  };

  //REQUIRES: list is empty
  //EFFECTS:  copies all nodes from other to this
  void copy_all(const List &other) {
    for (Node *p=other.first; p!=nullptr; p=p->next){
      push_back(p->datum);
    }
  }

  Node *first;   // points to first Node in list, or nullptr if list is empty
  Node *last;    // points to last Node in list, or nullptr if list is empty 

public:
  ////////////////////////////////////////
  class Iterator {
    //OVERVIEW: Iterator interface to List

    // You should add in a default constructor, destructor, copy constructor,
    // and overloaded assignment operator, if appropriate. If these operations
    // will work correctly without defining these, you can omit them. A user
    // of the class must be able to create, copy, assign, and destroy Iterators.

    // Your iterator should implement the following public operators: *,
    // ++ (prefix), default constructor, == and !=.

  public:
    // This operator will be used to test your code. Do not modify it.
    // Requires that the current element is dereferenceable.
    
    
    Iterator() : node_ptr(nullptr) {}

    Iterator& operator--() {
      assert(node_ptr);
      node_ptr = node_ptr->prev;
      return *this;
    }

    Iterator& operator++() {
      assert(node_ptr);
      node_ptr = node_ptr->next;
      return *this;
    }

    T& operator*() {
      assert(node_ptr != nullptr);
      return node_ptr->datum;
    }

    bool operator==(Iterator rhs) const{
       return node_ptr == rhs.node_ptr;
    }

    bool operator!=(Iterator rhs) const{
      return node_ptr != rhs.node_ptr;
    }
    

  private:
    Node *node_ptr; //current Iterator position is a List node
    // add any additional necessary member variables here

    // add any friend declarations here

    friend class List;

    // construct an Iterator at a specific position
    Iterator(Node* p) 
      : node_ptr(p) {}

  };//List::Iterator
  ////////////////////////////////////////

  // return an Iterator pointing to the first element
  Iterator begin() const {
    return Iterator(first);
  }

  // return an Iterator pointing to "past the end"
  Iterator end() const{
    return Iterator();
  }

 //REQUIRES: i is a valid, dereferenceable iterator associated with this list
  //MODIFIES: may invalidate other list iterators
  //EFFECTS: Removes a single element from the list container
  void erase(Iterator i){
     if(list_size == 1 || i == begin()){
      pop_front();
     }
     else if(i == Iterator(last)){
       pop_back();
     }
     else{
      Iterator prev_it = Iterator(i.node_ptr->prev);
      Iterator next_it = Iterator(i.node_ptr->next);
      prev_it.node_ptr->next = next_it.node_ptr;
      next_it.node_ptr->prev = prev_it.node_ptr;
      delete i.node_ptr;
      --list_size;
     }
     
  }

  //REQUIRES: i is a valid iterator associated with this list
  //EFFECTS: inserts datum before the element at the specified position.
  void insert(Iterator i, const T &datum){
    if(empty() || i == begin()){
      push_front(datum);
    }
    else if(i == end()){
      push_back(datum);
    }
    else{
      Node *addition =  new Node;
      Iterator prev_it = Iterator(i.node_ptr->prev);
      addition->datum = datum;
      addition->next = i.node_ptr;
      i.node_ptr->prev = addition;
      addition->prev = prev_it.node_ptr;
      prev_it.node_ptr->next = addition; 
      ++list_size;
    }
  }

  

};//List


////////////////////////////////////////////////////////////////////////////////
// Add your member function implementations below or in the class above
// (your choice). Do not change the public interface of List, although you
// may add the Big Three if needed.  Do add the public member functions for
// Iterator.


#endif // Do not remove this. Write all your code above this line.