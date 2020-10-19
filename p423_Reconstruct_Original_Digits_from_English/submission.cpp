/*
zero
one 
two
three
four
five
six 
seven
eight
nine

grades of "uniqueness"
- 1st [z:0, w:2, u:4, x:6, g:8]
- 2nd [o:1, r:3, s:7]
  - would be unique after elimated 1st candidates
- 3rd [v:5]
- 4th [e:9]
*/

class Solution {
private:
  unordered_map<int, unordered_map<char, int>> gradeMapper;
  unordered_map<int, char const *> digitToStr;
  unordered_map<int, unordered_map<char, int>> digitToChCount;

public:
  Solution() {
    digitToStr[0] = "zero";
    digitToStr[1] = "one";
    digitToStr[2] = "two";
    digitToStr[3] = "three";
    digitToStr[4] = "four";
    digitToStr[5] = "five";
    digitToStr[6] = "six";
    digitToStr[7] = "seven";
    digitToStr[8] = "eight";
    digitToStr[9] = "nine";
    for (auto &it : digitToStr) {
      int digit = it.first;
      unordered_map<char, int> counter;
      char const * const  dStr = it.second;
      char const * p = &(dStr[0]);
      while (*p != '\0') {
        ++counter[*p];
        ++p;
      }
      digitToChCount[digit] = counter;
    }
    
    gradeMapper[1] = {
      {'z', 0},
      {'w', 2},
      {'u', 4},
      {'x', 6},
      {'g', 8}
    };
    
    gradeMapper[2] = {
      {'o', 1},
      {'r', 3},
      {'s', 7}
    };
    
    gradeMapper[3] = {
      {'v', 5}
    };
    
    gradeMapper[4] = {
      {'e', 9}
    };
    
  }
  
  void solve(int grade, unordered_multiset<char> &restChars, vector<int> &candidates) {
    if (5 == grade || restChars.empty()) {
      return;
    }
    
    // printf("At grade %d, restChars is now: ", grade);
    // for (auto c : restChars) {
    //   printf("%c ", c);
    // }
    // printf("\n");

    auto mapOfCurrentGrade = gradeMapper[grade];
    
    unordered_map<char, int> toRemoveCounter;
    for (auto c : restChars) {
      if (mapOfCurrentGrade.find(c) == mapOfCurrentGrade.end()) continue;
      int digit = mapOfCurrentGrade[c];
      auto &singleToRemove = digitToChCount[digit];
      for (auto &it : singleToRemove) {
        toRemoveCounter[it.first] += it.second;
      }
    }
    
    // printf("toRemoveCounter is:: ");
    // for (auto &it : toRemoveCounter) {
    //   printf("%c:%d ", it.first, it.second);
    // }
    // puts("");
    
    for (auto it = restChars.begin(); it != restChars.end();) {
      if (toRemoveCounter[*it] > 0) {
        if (mapOfCurrentGrade.find(*it) != mapOfCurrentGrade.end()) {
          int digit = mapOfCurrentGrade[*it];
          candidates.push_back(digit);
        }
        --toRemoveCounter[*it];
        it = restChars.erase(it);
      } else {
        ++it;
      }
    }
    
    solve(grade+1, restChars, candidates);
  }

  string originalDigits(string s) {
    unordered_multiset<char> restChars;
    for (auto ch : s) {
      restChars.insert(ch);
    }
    vector<int> candidates;
    solve(1, restChars, candidates);
    sort(candidates.begin(), candidates.end());

    string ans = "";
    for (auto d : candidates) {
      ans.push_back('0'+d);
    }
    return ans;
  }
};
