class Compare {
public:
  bool operator() (vector<int> &lhs, vector<int> &rhs) {
    if (lhs[0] != rhs[0]) {
      return lhs[0] < rhs[0];
    } else {
      // Under the same "start", the one with smaller "end" has less probability of getting overlapped.
      return lhs[1] < rhs[1];
    }
  }
};

class Solution {
public:
    int eraseOverlapIntervals(vector<vector<int>>& intervals) {
      list<vector<int>> nonoverlappingResult;
      
      sort(intervals.begin(), intervals.end(), Compare());
      
      for (auto &candidate : intervals) {
        if (nonoverlappingResult.empty()) {
          nonoverlappingResult.push_back(candidate);
          continue;
        }
        auto &last = nonoverlappingResult.back();
        // Note that "last[0] <= candidate[0]" by the predefined "Compare()".
        if (candidate[0] >= last[1]) {
          nonoverlappingResult.push_back(candidate);
        } else {
          // candidate[0] < last[1]
          if (last[1] > candidate[1]) {
            nonoverlappingResult.pop_back();
            nonoverlappingResult.push_back(candidate);
          }
        }
      }
      
      return intervals.size() - nonoverlappingResult.size();
    }
};
