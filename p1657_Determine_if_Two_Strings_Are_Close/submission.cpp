/*
test case#1
"auuxxxbbbb"
"xaabbbuuuu"
true
```
"auuxxxbbbb" -> "auubbbxxxx"
"xaabbbuuuu" -> "axxbbbuuuu" -> "auubbbxxxx"
```

test case#2
"hhhhhhhhhhh"
"hhhhhhhhhhh"
*/
class Solution {
public:
    bool closeStrings(string word1, string word2) {
      if (word1.length() != word2.length()) return false;
      unordered_map<char, int> counter1, counter2;
      for (char &ch : word1) {
        ++counter1[ch];
      }
      for (char &ch : word2) {
        ++counter2[ch];
      }
      vector<pair<int, char>> counts1;
      for (auto &it : counter1) {
        counts1.push_back({it.second, it.first});
      }
      
      vector<pair<int, char>> counts2;
      for (auto &it : counter2) {
        counts2.push_back({it.second, it.first});
      }
      if (counts1.size() != counts2.size()) return false;
      sort(counts1.begin(), counts1.end(), less<pair<int, char>>());
      sort(counts2.begin(), counts2.end(), less<pair<int, char>>());
      for (int i = 0; i < counts1.size(); ++i) {
        //printf("i:%d\n", i);
        if (counts1[i].first != counts2[i].first) return false;
        // counts1[i].first == counts2[i].first
        
        if (counts1[i].second != counts2[i].second) {
          // printf("counter1[counts2[i:%d].second:%c] == %d\n", i, counts2[i].second, counter1[counts2[i].second]);
          // printf("counter2[counts1[i:%d].second:%c] == %d\n", i, counts1[i].second, counter2[counts1[i].second]);
          
          if (counter1[counts2[i].second] == 0) return false; // not swappable
          if (counter2[counts1[i].second] == 0) return false; // not swappable
        }
      }
      
      return true;
    }
};
