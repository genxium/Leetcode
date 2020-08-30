/*
This problem is a 2D generalization of https://github.com/genxium/Leetcode/tree/master/p300_Longest_Increasing_Subsequence.
*/

/*
test case #1
[[5,4],[6,4],[6,7],[2,3]]

test case #2
[[1,3],[3,5],[6,7],[6,8],[8,4],[9,5]]
*/
#define VI vector<int>
class EnvelopeSortCompare {
public:
  bool operator() (VI const& lhs, VI const& rhs) const {
    if (lhs[0] != rhs[0]) return lhs[0] > rhs[0]; // larger width comes first
    return lhs[1] < rhs[1]; // smaller height comes first upon equal widths
  }
};

class CacheSearchCompare {
public:
  bool operator() (VI const& lhs, VI const& rhs) const {
    return lhs[0] > rhs[0] && lhs[1] > rhs[1];
  }
};

class Solution {
public:
    int maxEnvelopes(vector<vector<int>>& envelopes) {
      sort(envelopes.begin(), envelopes.end(), EnvelopeSortCompare());
      vector<VI> sortedCache;
      
      for (auto &envelope : envelopes) {
        auto lower = lower_bound(sortedCache.begin(), sortedCache.end(), envelope, CacheSearchCompare());
        if (lower == sortedCache.end()) {
          sortedCache.push_back(envelope);
        } else {
          /*
          Now that "(*lower)[0] >= envelope[0]", because of the way "envelopes" is sorted.
          
          if "(*lower)[0] == envelope[0] a.k.a. width" then we know that "(*lower)[1] <= envelope[1] a.k.a. height", again because of the way "envelopes" is sorted
          
          if "(*lower)[0] > envelope[0] a.k.a. width" then we know that "(*lower)[1] <= envelope[1] a.k.a. height", because otherwise it violates the search criteria.
          
          Either way we shouldn't keep the original "(*lower)" because it won't contribute to a longer "sortedCache".
          
          To exemplify, consider a case where the current "(*lower) == {9, 5}" and that the incoming "envelope == {9, 6}", and by definition of the search criteria one possible "(*(lower+1)) == {8, 4}" is assumed as well. It's clear that "*lower = envelope" is optimal.
          
          However for an edge-case where the current "(*lower) == {9, 5}" and that the incoming "envelope == {8, 5}", and by definition of the search criteria one possible "(*(lower+1)) == {8, 4}" is assumed as well, it's NON-TRIVIAL to conclude that "(*lower)" will quickly be updated to "{8, max(when width==8)}".
          */
          *lower = envelope;
        }
      }
      /*
      for (auto &ele : sortedCache) {
        printf("{w: %d, h: %d}\n", ele[0], ele[1]);  
      }
      printf("\n");
      */
      return sortedCache.size();
    }
};
