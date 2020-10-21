#define INVALID -1

typedef class RepeatingInterval {
public:
  char ch;
  int st = INVALID, ed = INVALID; // st is closed, ed is open
  int prefixCost = 0; // note that "prefixCost" always starts with "prefixCost == 0" for "mapper[ch][0]" of each "ch"
  int prefixLength = 0;
  
  void print() {
    printf("ch:%c, [st:%d, ed:%d), prefixCost:%d, prefixLength:%d\n", ch, st, ed, prefixCost, prefixLength);
  }
} RIT;

/*
test case#0
""
0

test case#1
"ABAB"
2

test case#2
"AABABBA"
1

test case#3 (this is a case where some greedy strategy will fail, if the middle substring "XAAX" was chosen to be replaced with 'B' for the first step)
"AAAABBBBXAAXBBBBAAAA"
10

test case#4
"ABBB"
2
*/
class RITCompare {
public:
  bool operator() (RIT* const lhs, int const rhs) {
    return lhs->prefixCost < rhs;
  }
};

class Solution {
public:
    int characterReplacement(string s, int k) {
      int n = s.length();
      if (0 == n) return 0;
      
      unordered_map<char, vector<RIT*>> mapper;           
      RIT* activeRit = new RIT();
      activeRit->st = 0;
      activeRit->ch = s[0];
      
      for (int i = 1; i <= n; ++i) {
        if (i < n && s[i] == activeRit->ch) continue;
        // s[i] != activeRit->ch
        activeRit->ed = i;
        
        auto vecOfCurrentChIt = mapper.find(activeRit->ch);
        if (vecOfCurrentChIt == mapper.end()) {
          RIT* assigner = activeRit;
          assigner->prefixLength = (assigner->ed - assigner->st);
          vector<RIT*> vecOfCurrentCh{assigner};
          mapper[activeRit->ch] = vecOfCurrentCh;
        } else {
          vector<RIT*>& vecOfCurrentCh = vecOfCurrentChIt->second;
          auto prevRit = vecOfCurrentCh.back();
          RIT* assigner = activeRit;
          int chCountToReplace = assigner->st - prevRit->ed;
          int chCountOfInterval = assigner->ed - assigner->st;
          assigner->prefixLength = prevRit->prefixLength + chCountToReplace + chCountOfInterval;
          assigner->prefixCost = prevRit->prefixCost + chCountToReplace;
          vecOfCurrentCh.push_back(assigner);
        }
        
        if (i < n) {
          activeRit = new RIT();
          activeRit->st = i;
          activeRit->ch = s[i];          
        }
      }
      
      int ans = 0;
      for (auto vecOfCurrentChIt = mapper.begin(); vecOfCurrentChIt != mapper.end(); ++vecOfCurrentChIt) {     
        vector<RIT*>& vecOfCurrentCh = vecOfCurrentChIt->second;
        int m = vecOfCurrentCh.size();
        for (int j = 0; j < m; ++j) {
          // printf("Checking interval");
          // vecOfCurrentCh[j]->print();
          if (vecOfCurrentCh[j]->prefixCost <= k) {
            int residualSteps = (k-vecOfCurrentCh[j]->prefixCost);
            int candidate = vecOfCurrentCh[j]->prefixLength + residualSteps;
            candidate = min(candidate, n);
            ans = max(ans, candidate);
          } else {
            int minDeduction = (vecOfCurrentCh[j]->prefixCost - k);
            auto thatIntervalIt = lower_bound(vecOfCurrentCh.begin(), vecOfCurrentCh.end(), minDeduction, RITCompare());
            if (thatIntervalIt == vecOfCurrentCh.end()) continue;
            
            RIT* thatInterval = *thatIntervalIt; 
            int residualSteps = (k-(vecOfCurrentCh[j]->prefixCost-thatInterval->prefixCost)); // We can use "residualSteps" to add same characters on the left-side of "thatInterval", or the right-side of "vecOfCurrentCh[j]".
            int candidate = (thatInterval->ed - thatInterval->st) + (vecOfCurrentCh[j]->prefixLength - thatInterval->prefixLength) + residualSteps;
            candidate = min(candidate, n);
            
            ans = max(ans, candidate);
          }
        }
      }
      
      return ans;
    }
};
