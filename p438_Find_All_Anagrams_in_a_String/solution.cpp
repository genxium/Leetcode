#include <stdio.h>
#include <string>
#include <vector>
#include <map>
using namespace std;

class Solution {
  public:
    map<char, int> stringToCharCount(string &str, int st, int ed) {
      map<char, int> toRet;
      for (int i = st; i < ed; ++i) {
        map<char, int>::iterator it = toRet.find(str[i]);
        if (it != toRet.end()) {
          toRet[str[i]] += 1;
        } else {
          toRet[str[i]] = 1;
        }
      }
      return toRet;
    }

    bool isEqual(map<char, int> &left, map<char, int> &right) {
      /*
      * Time complexity of this function is O(size of charset).
      */
      if (left.size() != right.size()) {
        return false;
      }

      for (map<char, int>::iterator it = left.begin(); it != left.end(); ++it) {
        char key = it->first;
        map<char, int>::iterator matchingIt = right.find(key);
        if (matchingIt == right.end()) {
          return false;
        }
        if (matchingIt->second != it->second) {
          return false;
        }
      }
      return true;
    }

    vector<int> findAnagrams(string s, string p) {
      vector<int> toRet;

      if (p.size() > s.size()) {
        return toRet;
      }

      map<char, int> eleInP = this->stringToCharCount(p, 0, p.size());

      int offset = 0;
      map<char, int> eleInS = this->stringToCharCount(s, offset, offset+p.size());

      while(true) {
        if (isEqual(eleInS, eleInP)) {
          toRet.push_back(offset);
        }

        char toEvict = s[offset];
        --eleInS[toEvict];
        if (0 == eleInS[toEvict]) {
          eleInS.erase(toEvict);
        } 

        ++offset;

        if (offset + p.size() - 1 >= s.size()) {
          break;
        }
        char toInsert = s[offset + p.size() - 1];

        map<char, int>::iterator existingToInsertIt = eleInS.find(toInsert);  
        if (existingToInsertIt == eleInS.end()) {
          eleInS[toInsert] = 1;
        } else {
          eleInS[toInsert] += 1;
        }

        // printf("Moved from preOffset == %d to offset == %d, toEvict == %c, toInsert == %c\n", offset-1, offset, toEvict, toInsert);
      }

      return toRet;
    }
};

int main() {
  string t1 = "cbaebabacd";
  string t2 = "abc";
  Solution s;
  vector<int> res = s.findAnagrams(t1, t2);
  for (int i = 0; i < res.size(); ++i) {
    printf("%d ", res[i]); 
  }
  printf("\n"); 
  return 0;
}
