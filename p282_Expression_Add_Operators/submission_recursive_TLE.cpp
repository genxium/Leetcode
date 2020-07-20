class Solution {
  public:
    bool hasInvalidLeadingZero(string& exp) {
      if ('0' == exp[0] && 1 < exp.length()) {
        return true;
      }
      return false;
    }

    void search(string &digitSeg, unordered_map<string, unordered_map<int, unordered_set<string>>> &cache, int origLen, int target, vector<string> &ans) {
      //printf("search digitSeg:%s\n", digitSeg.c_str());
      int L = digitSeg.length();

      for (int prefixLen = 1; prefixLen <= L; ++prefixLen) {
        string prefix = digitSeg.substr(0, prefixLen);
        if (prefixLen < L) {
          if (cache.find(prefix) == cache.end()) {
            search(prefix, cache, origLen, target, ans);
          }
          auto prefixResultDict = cache[prefix];
          string suffix = digitSeg.substr(prefixLen, (L-prefixLen));
          if (cache.find(suffix) == cache.end()) {
            search(suffix, cache, origLen, target, ans);
          }
          auto suffixResultDict = cache[suffix];
          for (auto &itPrefixResult : prefixResultDict) {
            for (auto &itSuffixResult : suffixResultDict) {
              int prefixResultVal = itPrefixResult.first;
              int suffixResultVal = itSuffixResult.first;
              {
                for (auto &prefixExp : itPrefixResult.second) {
                  for (auto &suffixExp: itSuffixResult.second) {
                    string newExp = prefixExp + "+" + suffixExp;
                    int newResultVal = prefixResultVal + suffixResultVal;
                    cache[digitSeg][newResultVal].insert(newExp);
                  }
                }
              }
              {
                for (auto &prefixExp : itPrefixResult.second) {
                  for (auto &suffixExp: itSuffixResult.second) {
                    string newExp = prefixExp + "-" + suffixExp;
                    int newResultVal = prefixResultVal - suffixResultVal;
                    cache[digitSeg][newResultVal].insert(newExp);
                  }
                }
              }
              {
                for (auto &prefixExp : itPrefixResult.second) {
                  for (auto &suffixExp: itSuffixResult.second) {
                    auto lastMultiplyInPrefix = prefixExp.find_last_of("*");
                    auto lastPlusMinusInPrefix = prefixExp.find_last_of("+-");
                    if (lastPlusMinusInPrefix != string::npos 
                        &&
                        (
                         lastMultiplyInPrefix == string::npos
                         ||
                         lastPlusMinusInPrefix > lastMultiplyInPrefix
                        )
                       ) {
                      continue;
                    }
                    auto firstMultiplyInPrefix = prefixExp.find_first_of("*");
                    auto firstPlusMinusInPrefix = prefixExp.find_first_of("+-");
                    if (firstPlusMinusInPrefix != string::npos 
                        &&
                        (
                         firstMultiplyInPrefix == string::npos
                         ||
                         firstPlusMinusInPrefix < firstMultiplyInPrefix
                        )
                       ) {
                      continue;
                    }

                    string newExp = prefixExp + "*" + suffixExp;
                    int newResultVal = prefixResultVal * suffixResultVal;
                    cache[digitSeg][newResultVal].insert(newExp);
                  }
                }
              }
            }
          }
        } else {
          if (false == hasInvalidLeadingZero(digitSeg)) {
            int newResultVal = atoi(digitSeg.c_str());
            cache[digitSeg][newResultVal].insert(digitSeg); 
          }
        }
      }

      if (L == origLen) {
        auto resultDict = cache[digitSeg];
        for (auto &itResult : resultDict) {
          if (itResult.first != target) continue;
          for (auto &exp : itResult.second) {
            ans.push_back(exp); 
          }
        }
      }
    }

    vector<string> addOperators(string num, int target) {
      /*
         test case #1
         "105"
         5

         test case #2
         "3456237490"
         9191
       */

      // cache[digitSeg][resultVal] = {strings with operators that yield "resultVal"}
      unordered_map<string, unordered_map<int, unordered_set<string>>> cache;
      vector<string> ans;
      search(num, cache, num.length(), target, ans);
      return ans;
    }
};

