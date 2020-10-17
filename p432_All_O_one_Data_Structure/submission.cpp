/*
test case#1
["AllOne","getMaxKey","getMinKey"]
[[],[],[]]

test case#2
["AllOne","inc","inc","inc","getMaxKey","getMinKey","inc","inc","getMaxKey","getMinKey"]
[[],["foo"],["bar"],["bar"],[],[],["foo"],["foo"],[],[]]

test case#3
["AllOne","inc","inc","inc","getMaxKey","getMinKey","dec","getMaxKey","getMinKey","inc","getMaxKey","getMinKey"]
[[],["foo"],["bar"],["bar"],[],[],["foo"],[],[],["foo"],[],[]]

test case#4
["AllOne","inc","inc","getMaxKey","getMinKey","inc","inc","getMaxKey","getMinKey","inc","inc","inc","getMaxKey","getMinKey","inc","inc","inc","inc","getMaxKey","getMinKey","dec","dec","getMaxKey","getMinKey"]
[[],["foo"],["foo"],[],[],["bar"],["bar"],[],[],["bar"],["bar"],["bar"],[],[],["ter"],["ter"],["ter"],["ter"],[],[],["bar"],["bar"],[],[]]

test case#5
["AllOne","inc","inc","inc","dec","inc","inc","getMaxKey"]
[[],["hello"],["world"],["hello"],["world"],["hello"],["leet"],[]]
*/
class DoublyListNode {
public:
  int count;
  unordered_set<string> keysOfCurrentCount;
  DoublyListNode *prev = NULL, *next = NULL;
  DoublyListNode(int aCount) {
    this->count = aCount;
  }
  void print() {
    printf("Node %p has count:%d and keys: ", this, count);
    for (auto &key : keysOfCurrentCount) {
      printf("%s ", key.c_str());
    }
    printf("\n");
  }
};

class AllOne {
private:
  unordered_map<string, int> counter;
  
  DoublyListNode *head = NULL, *tail = NULL; // always maintain a "sorted list" of existing count values
  unordered_map<int, DoublyListNode*> mapper;
  
  void deleteNode(DoublyListNode* oldNode) {
    // printf("\toldNode for prevCount:%d now has no keys\n", oldNode->count);
    DoublyListNode* left = oldNode->prev;
    DoublyListNode* right = oldNode->next;

    if (NULL != left) {
      left->next = right;  
    } else {
      // "head" was "oldNode"
      head = right;
      // printf("\thead has transitted to right with count:%d\n", head->count);
    }

    if (NULL != right) {
      right->prev = left;
    } else {
      // "tail" was "oldNode"
      tail = left;
      // printf("\ttail has transitted to left with count:%d\n", tail->count);
    }
    int oldCount = oldNode->count;
    delete oldNode;
    mapper.erase(oldCount);
  }
  
public:
  /** Initialize your data structure here. */
  AllOne() {

  }

  /** Inserts a new key <Key> with value 1. Or increments an existing key by 1. */
  void inc(string key) {
    int prevCount = counter[key];
    ++counter[key];
    int currentCount = prevCount+1;
    // printf("inc %s, prevCount: %d, currentCount: %d\n", key.c_str(), prevCount, currentCount);
    if (0 == prevCount) {
      // 1 == currentCount
      auto candidateIt = mapper.find(currentCount);
      if (candidateIt == mapper.end()) {
        DoublyListNode* newNode = new DoublyListNode(currentCount); 
        newNode->keysOfCurrentCount.insert(key);
        mapper[currentCount] = newNode;  
        // this "newNode" is obviously the new head
        if (NULL == head) {
          head = newNode;
          tail = newNode;
        } else {
          newNode->next = head;
          head->prev = newNode;
          head = newNode;
        }  
      } else {
        DoublyListNode* candidateNode = candidateIt->second; 
        candidateNode->keysOfCurrentCount.insert(key); 
      }
      return;
    }
    
    auto oldIt = mapper.find(prevCount);
    DoublyListNode* oldNode = oldIt->second;
    DoublyListNode* candidateNode = oldNode->next;
    
    // printf("\tthe oldNode is "); oldNode->print();
    // if (NULL == candidateNode) {
    //   printf("\tthe candidateNode is NULL\n");
    // } else {
    //   printf("\tthe candidateNode is "); candidateNode->print();
    // }
    oldNode->keysOfCurrentCount.erase(key);

    if (NULL == candidateNode) {
      candidateNode = new DoublyListNode(currentCount);
      candidateNode->keysOfCurrentCount.insert(key);

      // this "candidateNode" is obviously the new tail
      tail->next = candidateNode;
      candidateNode->prev = tail;
      tail = candidateNode;
      
      mapper[currentCount] = candidateNode;
    } else if (candidateNode->count != currentCount) {
      DoublyListNode* newNode = new DoublyListNode(currentCount);
      newNode->keysOfCurrentCount.insert(key);

      newNode->next = candidateNode;
      newNode->prev = oldNode;
      candidateNode->prev = newNode;
      oldNode->next = newNode;
      
      mapper[currentCount] = newNode;
    } else {
      candidateNode->keysOfCurrentCount.insert(key);
    }
    
    if (oldNode->keysOfCurrentCount.empty()) {
      deleteNode(oldNode);
    }
  }

  /** Decrements an existing key by 1. If Key's value is 1, remove it from the data structure. */
  void dec(string key) {
    int prevCount = counter[key];
    if (0 == prevCount) return;

    --counter[key];
    int currentCount = prevCount-1;
    // printf("dec %s, prevCount: %d, currentCount: %d\n", key.c_str(), prevCount, currentCount);

    auto oldIt = mapper.find(prevCount);
    DoublyListNode* oldNode = oldIt->second;
    DoublyListNode* candidateNode = oldNode->prev;
    
    // printf("\tthe oldNode is "); oldNode->print();
    // if (NULL == candidateNode) {
    //   printf("\tthe candidateNode is NULL\n");
    // } else {
    //   printf("\tthe candidateNode is "); candidateNode->print();
    // }
    oldNode->keysOfCurrentCount.erase(key);
    
    if (0 == currentCount) {
      if (oldNode->keysOfCurrentCount.empty()) {
        deleteNode(oldNode);
      }
      return;
    }
        
    if (NULL == candidateNode) {
      candidateNode = new DoublyListNode(currentCount);
      candidateNode->keysOfCurrentCount.insert(key);
      // this "candidateNode" is obviously the new head
      head->prev = candidateNode;
      candidateNode->next = head;
      head = candidateNode;
      
      mapper[currentCount] = candidateNode;
    } else if (candidateNode->count != currentCount) {
      DoublyListNode* newNode = new DoublyListNode(currentCount);
      newNode->keysOfCurrentCount.insert(key);

      newNode->prev = candidateNode;
      newNode->next = oldNode;
      candidateNode->next = newNode;
      oldNode->prev = newNode;
      
      mapper[currentCount] = newNode;
    } else {
      candidateNode->keysOfCurrentCount.insert(key);      
    }
    
    if (oldNode->keysOfCurrentCount.empty()) {
      deleteNode(oldNode);
    }
  }

  /** Returns one of the keys with maximal value. */
  string getMaxKey() {
    if (NULL == tail) return "";
    //printf("getMaxKey tail not NULL\n");
    auto it = tail->keysOfCurrentCount.begin();
    //printf("\tgetMaxKey returning\n");
    return *it;
  }

  /** Returns one of the keys with Minimal value. */
  string getMinKey() {
    if (NULL == head) return "";
    //printf("getMinKey head not NULL\n");
    auto it = head->keysOfCurrentCount.begin(); 
    //printf("\tgetMinKey returning\n");
    return *it;
  }
};

/**
 * Your AllOne object will be instantiated and called as such:
 * AllOne* obj = new AllOne();
 * obj->inc(key);
 * obj->dec(key);
 * string param_3 = obj->getMaxKey();
 * string param_4 = obj->getMinKey();
 */
