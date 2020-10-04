/*
test case#1
"112"
1

test case#2
"1432219"
3
*/
class Solution {
public:
    string removeKdigits(string num, int k) {
      int n = num.length();
      if (k >= n) return "0";
      if (0 == k) return num;
      // for num like "...ab...x..." where "a > b", it's always beneficial to remove "a" instead of "x", because "...ab...... > ...b...x..." due to "a > b"
      list<int> tryToKeepOrderedQueue;
      for (auto ch : num) {
        int val = ch-'0';
        while (0 < tryToKeepOrderedQueue.size() && tryToKeepOrderedQueue.back() > val && k > 0) {
          tryToKeepOrderedQueue.pop_back();
          --k;
        }
        tryToKeepOrderedQueue.push_back(val);
      }
      while (!tryToKeepOrderedQueue.empty() && k > 0) {
        tryToKeepOrderedQueue.pop_back();
        --k;
      }
      string result = "";
      while (!tryToKeepOrderedQueue.empty()) {
        auto lead = tryToKeepOrderedQueue.front(); tryToKeepOrderedQueue.pop_front();
        if (result.empty() && 0 == lead) continue;
        result.push_back('0'+lead);
      }
      if (result.empty()) result.push_back('0');
      return result;
    }
};
