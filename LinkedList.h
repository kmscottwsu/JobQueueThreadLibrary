#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "StdLibraryDependencies.h"

class Node
{
public:
  Node(int memberData)
  {
    memberData_ = memberData;
    pNext_ = NULL;
  };

  Node()
  {
    memberData_ = 0;
    pNext_ = NULL;
  }

  Node(const Node& node)
  {
    memberData_ = node.memberData_;
    pNext_ = NULL;
  };

  ~Node() {


  };
private:
  int memberData_;
  std::atomic<Node*> pNext_;

  friend class LinkedList;
};


class LinkedList
{
public:
  LinkedList()
  {
    pHead_.store(NULL);
    pTail_.store(NULL);
  };

  ~LinkedList()
  {

  };

  void addData(int memberData)
  {
    //this only works with a single producer.

    //create new node
    Node* pNewNode = new Node(memberData);

    //modify old head to point to new node
    pHead_.load()->pNext_ = pNewNode;//can do this since it is atomic ptr

    pHead_ = pNewNode;//same here

    if (pTail_ == NULL) {
      pTail_.store(pHead_.load());
    }

  };

  bool getData(int& memberData)
  {
    bool check = false;
    Node *pHead, *pTail, *pNext;
    //for this to work, can only delete nodes that
    //are sufficiently far away from the current head

    while (!check) {
      pTail = pTail_.load();
      if (pTail == NULL) {//if empty
        return false;
      }
      check = pTail_.compare_exchange_weak(pTail, pTail->pNext_.load());
    }
    memberData = pTail->memberData_;


  };

private:
  std::atomic<Node*> pHead_;
  std::atomic<Node*> pTail_;


};



#endif