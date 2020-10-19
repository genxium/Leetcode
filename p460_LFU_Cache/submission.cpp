#define INVALID -1

/*
test case#1
["LFUCache","put","put","get","put","get","get","put","get","get","get"]
[[2],[1,1],[2,2],[1],[3,3],[2],[3],[4,4],[1],[3],[4]]

test case#2
["LFUCache","put","get"]
[[0],[0,0],[0]]
*/
class DoublyListNode {
public:
  int count;
  list<int> keysOfCurrentCount; // The "keysOfCurrentCount.removeByKey(...)" operation is NOT NECESSARILY required here, moreover a "keysOfCurrentCount.getEarliestInserted()" operation is a hard requirement. Therefore a "key -> list<int>::iterator of the key within `someDoublyListNode->keysOfCurrentCount`" mapping is maintained.
  DoublyListNode *prev = NULL, *next = NULL;
  DoublyListNode(int aCount) {
    this->count = aCount;
  }
  void print() {
    printf("Node %p has count:%d and keys: ", this, count);
    for (auto &key : keysOfCurrentCount) {
      printf("%d ", key);
    }
    printf("\n");
  }
};

typedef list<int>::iterator ListPtrInDoublyListNode;

class LFUCache {
private:
  int capacity = 0;
  unordered_map<int, pair<int, ListPtrInDoublyListNode>> kv;
  
  unordered_map<int, int> counter;
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
  
  ListPtrInDoublyListNode incUseCount(int key) {
    int prevCount = counter[key];
    ++counter[key];
    int currentCount = prevCount+1;
    // printf("inc %s, prevCount: %d, currentCount: %d\n", key.c_str(), prevCount, currentCount);
    
    DoublyListNode* targetNode = NULL;
    if (0 == prevCount) {
      // 1 == currentCount
      auto candidateIt = mapper.find(currentCount);
      if (candidateIt == mapper.end()) {
        targetNode = new DoublyListNode(currentCount); 
        mapper[currentCount] = targetNode;  
        // this "targetNode" is obviously the new head
        if (NULL == head) {
          head = targetNode;
          tail = targetNode;
        } else {
          targetNode->next = head;
          head->prev = targetNode;
          head = targetNode;
        }  
      } else {
        targetNode = candidateIt->second; 
      }
      targetNode->keysOfCurrentCount.push_back(key);
      auto toRet = targetNode->keysOfCurrentCount.end(); --toRet; 
    
      return toRet;
    }
  
    auto prevPair = kv[key];
    auto prevSubListPtr = prevPair.second;
      
    auto oldIt = mapper.find(prevCount);
    DoublyListNode* oldNode = oldIt->second;
    DoublyListNode* candidateNode = oldNode->next;
    
    // printf("\tthe oldNode is "); oldNode->print();
    // if (NULL == candidateNode) {
    //   printf("\tthe candidateNode is NULL\n");
    // } else {
    //   printf("\tthe candidateNode is "); candidateNode->print();
    // }
    // printf("\tAbout to erase itr(of key:%d, *itr:%d) in oldNode:%p\n", key, *prevSubListPtr, oldNode);
    oldNode->keysOfCurrentCount.erase(prevSubListPtr);
    // printf("\tFinished erasing itr in oldNode:%p\n", oldNode);
    
    if (NULL == candidateNode) {
      targetNode = new DoublyListNode(currentCount);

      // this "targetNode" is obviously the new tail
      tail->next = targetNode;
      targetNode->prev = tail;
      tail = targetNode;
      
      mapper[currentCount] = targetNode;
    } else if (candidateNode->count != currentCount) {
      targetNode = new DoublyListNode(currentCount);

      targetNode->next = candidateNode;
      targetNode->prev = oldNode;
      candidateNode->prev = targetNode;
      oldNode->next = targetNode;
      
      mapper[currentCount] = targetNode;
    } else {
      targetNode = candidateNode;
    }
    targetNode->keysOfCurrentCount.push_back(key);
    auto toRet = targetNode->keysOfCurrentCount.end(); --toRet; 
    
    if (oldNode->keysOfCurrentCount.empty()) {
      deleteNode(oldNode);
    }
    
    return toRet;
  }
  
  void evictLFUKey() {
    // Should evict 1 key in head->keysOfCurrentCount
    // When there's a tie, evict the "least recently used one"
    int toEvictKey = head->keysOfCurrentCount.front();

    auto toEvictPair = kv[toEvictKey];
    auto toEvictSubListPtr = toEvictPair.second;
    
    // printf("\tthe head node is "); head->print();
    // printf("\tevicting itr(of key:%d, *itr: %d) since the capacity is reached\n", toEvictKey, *toEvictSubListPtr);

    head->keysOfCurrentCount.erase(toEvictSubListPtr);
    
    if (head->keysOfCurrentCount.empty()) {
      deleteNode(head);
    }
    
    kv.erase(toEvictKey);
    counter.erase(toEvictKey);
  }

public:
  LFUCache(int capacity) {
    this->capacity = capacity;
  }

  int get(int key) {
    if (0 == capacity) return INVALID;
    if (kv.find(key) == kv.end()) return INVALID;
    // printf("get key:%d\n", key);
    ListPtrInDoublyListNode newSubListPtr = incUseCount(key);
    auto& targetPair = kv[key]; 
    targetPair.second = newSubListPtr;
    return targetPair.first;
  }

  void put(int key, int value) {
    if (0 == capacity) return;
    // printf("put key:%d, value:%d\n", key, value);
    if (kv.find(key) == kv.end()) {
      if (capacity == kv.size()) {
        evictLFUKey();
      }
    }

    ListPtrInDoublyListNode newSubListPtr = incUseCount(key);
    kv[key] = {value, newSubListPtr};
  }
};

/**
 * Your LFUCache object will be instantiated and called as such:
 * LFUCache* obj = new LFUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */
