#define INVALID -1

bool isContainedBy(int l1, int r1, int l2, int r2) {
  if (l2 < l1 && r1 < r2) return true;
  return false;
}

class Seg {
  public:
    char val;
    int l, r;    
    vector<int> sortedPosList;
    Seg(): val(' '), l(INVALID), r(INVALID) {

    }
    Seg(char aVal, int aL) {
      val = aVal;
      l = aL;
      r = aL;
      sortedPosList.push_back(aL);
    }
};

bool intersects(pair<int, int> &closure, Seg &seg) {
  if (closure.first < seg.l && seg.l < closure.second && closure.second < seg.r) return true;
  if (seg.l < closure.first && closure.first < seg.r && seg.r < closure.second) return true;
  /*
  0 1   2 3 4 5 6   7 8 9
  a a | b c a b c | a a a

  seg {val: 'a', l: 0, r: 9}
  closure {2, 6}
  ```
  lower_bound(seg.sortedPosList.begin(), seg.sortedPosList.end(), closure.first)
  ```
  returns "position: 4" which is within "closure".
  */
  auto lower = lower_bound(seg.sortedPosList.begin(), seg.sortedPosList.end(), closure.first);
  if (lower != seg.sortedPosList.end() && *lower < closure.second) {
    return true;
  }

  return false;
}

class SegCompare {
  public:
    bool operator() (Seg const &lhs, Seg const &rhs) {
      int lw = lhs.r - lhs.l;
      int rw = rhs.r - rhs.l;
      if (lw != rw) {
        return lw < rw;
      }
      if (lhs.l != rhs.l) {
        return lhs.l < rhs.l;
      } 
      return lhs.r < rhs.r;
    }
};

class ClosureCompare {
  public:
    bool operator() (pair<int, int> const &lhs, pair<int, int> const &rhs) {
      int lw = lhs.second - lhs.first;
      int rw = rhs.second - rhs.first;
      if (lw != rw) {
        return lw < rw;
      }
      if (lhs.first != rhs.first) {
        return lhs.first < rhs.first;
      } 
      return lhs.second < rhs.second;
    }
};

class Solution {
  public:
    vector<string> maxNumOfSubstrings(string s) {
      /*
         test case #1
         "abab"

         test case #2
         "ababa"

         test case #3
         "aabcebecff"
       */
      vector<Seg> segs;
      for (int i = 0; i < 26; ++i) {
          Seg segment;
          segs.push_back(segment);
      }
      for (int i = 0; i < s.length(); ++i) {
        int idx = s[i]-'a';
        if (segs[idx].l == INVALID) {
          segs[idx].l = segs[idx].r = i;
          segs[idx].val = s[i];
          segs[idx].sortedPosList.push_back(i);
        } else {
          segs[idx].r = i;
        }
        segs[idx].sortedPosList.push_back(i);
      }
      sort(segs.begin(), segs.end(), SegCompare());
      vector<pair<int, int>> closures;
      vector<bool> used(26, false);
      for (auto &seg : segs) {
        if (seg.l == INVALID) continue;
        //printf("evaluating seg{val:%c, l:%d, r:%d}\n", seg.val, seg.l, seg.r);
        int djsIndex = seg.val - 'a';
        if (used[djsIndex]) continue;
        pair<int, int> currentClosure = {seg.l, seg.r};
        used[djsIndex] = true;

        bool hasIntersectingSegs = true;
        while (hasIntersectingSegs) {
          hasIntersectingSegs = false;
          for (auto &anotherSeg : segs) {
            if (anotherSeg.l == INVALID) continue;
            int anotherDjsIndex = anotherSeg.val - 'a';
            if (used[anotherDjsIndex]) continue;
            if (intersects(currentClosure, anotherSeg)) {
              /*
                 In this case, none of the segs within "currentClosure" and "anotherSeg" would be chosen alone.
               */
              //printf("\tit intersects with anotherSeg{val:%c, l:%d, r:%d}\n", anotherSeg.val, anotherSeg.l, anotherSeg.r);
              hasIntersectingSegs = true;
              if (currentClosure.second < anotherSeg.r) {
                currentClosure.second = anotherSeg.r;                
              } 
              if (currentClosure.first > anotherSeg.l) {
                currentClosure.first = anotherSeg.l;
              }
              used[anotherDjsIndex] = true;
            }
          }  
        }
        closures.push_back(currentClosure);
      }

      sort(closures.begin(), closures.end(), ClosureCompare());
      /*
      printf("closures:\n");
      for (auto &single : closures) {
        printf("[%d, %d]: %s\n", single.first, single.second, s.substr(single.first, single.second-single.first+1).c_str());
      }
      printf("\n");
      */
      // re-purpose "used[]"
      for (int i = 0; i < closures.size(); ++i) {
        used[i] = false;
      }
      vector<string> ans;
      for (int i = 0; i < closures.size(); ++i) {
        auto currentClosure = closures[i];
        bool isValid = true;
        for (int j = 0; j < i; ++j) {
          auto prevClosure = closures[j];
          if (used[j] && isContainedBy(prevClosure.first, prevClosure.second, currentClosure.first, currentClosure.second)) {
            isValid = false;
            break;
          }
        }
        if (isValid) {
          ans.push_back(s.substr(currentClosure.first, currentClosure.second-currentClosure.first+1)); 
          used[i] = true;
        }
      }
      return ans;
    }
};
