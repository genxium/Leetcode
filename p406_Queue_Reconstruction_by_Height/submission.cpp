#define VI vector<int>

class Compare {
public:
  bool operator() (VI const &lhs, VI const & rhs) {
    if (lhs[0] == rhs[0]) {
      // equal "h"s, the one with smaller "k" should come first
      return lhs[1] < rhs[1];
    }
    
    /* 
    Otherwise the larger "h" comes first, regardless of "k".
    Consider 
    ```
    [[7,0], [4,4], [7,1], [5,0], [6,1], [5,2]]
    ```
    , after sorted it becomes
    ```
    [[7,0], [7,1], [6,1], [5,0], [5,2], [4,4]]
    ```
    , and when traversed for insertion, elements with the same "k" would result in "increasing h" if we insert each at "ans.begin()+k", while guaranteeing that **upon insertion there're "k" previously inserted elements (i.e. whose "h"s are not smaller than the one being inserted)**. 
    */
    return lhs[0] > rhs[0];
  }
};

class Solution {
public:
    vector<vector<int>> reconstructQueue(vector<vector<int>>& people) {
      vector<VI> ans;
      sort(people.begin(), people.end(), Compare());
      for (auto person : people) {
        int k = person[1];
        ans.insert(ans.begin() + k, person);
      }
      return ans;
    }
};
