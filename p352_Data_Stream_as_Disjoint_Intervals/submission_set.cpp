#define INTERVAL_T pair<int, int>

/*
test case #1
["SummaryRanges","addNum","getIntervals","addNum","getIntervals","addNum","getIntervals","addNum","getIntervals","addNum","getIntervals","addNum","getIntervals","addNum","getIntervals","addNum","getIntervals","addNum","getIntervals","addNum","getIntervals"]
[[],[6],[],[6],[],[0],[],[4],[],[8],[],[7],[],[6],[],[4],[],[7],[],[5],[]]
*/
class SummaryRanges {
private:
  /*
  Be very careful when using "collection.erase(it)", which blocks forever if "it" is "collection.end()"! 
  */
  set<INTERVAL_T, less<INTERVAL_T>> collection;
public:
  /** Initialize your data structure here. */
  SummaryRanges() {
        
  }
    
  void addNum(int val) {
    /*
    A fact to notice is that each "addNum(...)" would adhere AT MOST 2 existing intervals.
    */
    //printf("Inserting [%d, %d]\n", val, val);
    if (collection.empty()) {
      // The initial case.  
      collection.insert({val, val});
      return;
    }
    
    INTERVAL_T newInterval = {val, INT_MAX}; // It's important to use INT_MAX as the right boundary here for search, because in the case of inserting "[6,6]" into "[[1,1], [4,4], [6,8]]" the expected "existingRightInterval" should be "collection.end()".
    
    auto it = collection.upper_bound(newInterval);
    
    if (it != collection.end()) {
      // Now that "it" is "existingRightInterval".
      //printf("\texistingRightInterval [%d, %d]\n", it->first, it->second);

      if (it->first == newInterval.first+1) {
        newInterval.second = it->second;
        it = collection.erase(it); // moves to next of "existingRightInterval"
      }
    }
    if (newInterval.second == INT_MAX) {
      newInterval.second = val;
    }
    
    if (it != collection.begin()) {
      --it;
    }
    
    if (it != collection.end()) {
      // Now that "it" is "existingLeftInterval".
      //printf("\texistingLeftInterval [%d, %d]\n", it->first, it->second);
      if (it->first <= newInterval.first && newInterval.first <= it->second) {
        // The "newInterval" is already included.
        return;
      }
      if (it->second == newInterval.first-1) {
        newInterval.first = it->first;
        collection.erase(it);       
      }
    }
    
    collection.insert(newInterval);
  }
    
  vector<vector<int>> getIntervals() {
    vector<vector<int>> ans;
    for (auto &it : collection) {
      ans.push_back({it.first, it.second});
    }
    return ans;
  }
};

/**
 * Your SummaryRanges object will be instantiated and called as such:
 * SummaryRanges* obj = new SummaryRanges();
 * obj->addNum(val);
 * vector<vector<int>> param_2 = obj->getIntervals();
 */
