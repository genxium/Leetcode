/*
// Definition for a Node.
class Node {
public:
    int val;
    Node* prev;
    Node* next;
    Node* child;
};
*/

/*
test case#1
[1,2,3,4,5,6,null,null,null,7,8,9,10,null,null,11,12]

test case#2
[1,null,2,null,3,null]
*/
class Solution {
private:
    void build(Node* head, Node* &tail) {
      Node *cur = head;
      while (cur) {
        tail = cur; // makes sure that the returned "tail" is not null
        Node* cachedNext = cur->next;
        if (cur->child) {
          Node* childTail = cur->child;
          build(cur->child, childTail);
          
          // reconnect "cur" and "cur->child"
          cur->next = cur->child;
          cur->child->prev = cur;
          
          if (cachedNext) {
            // reconnect "childTail" and "cachedNext"
            childTail->next = cachedNext;
            cachedNext->prev = childTail;              
          } else {
            tail = childTail; 
          }
          
          // reset "cur->child"
          cur->child = NULL;
        }
        cur = cachedNext;
      }
    }
  
public:
    Node* flatten(Node* head) {
      Node* childTail = head;
      if (!head) return NULL;
      build(head, childTail);
      return head;
    }
};
