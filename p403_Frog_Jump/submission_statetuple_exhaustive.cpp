#define StateTuple pair<int, int> // {preStepLength, stoneIdx}
#define MAXSTEPLENGTH 1101
#define MAXN 1100
bool dp[MAXSTEPLENGTH+1][MAXN]; 
bool closedSet[MAXSTEPLENGTH+1][MAXN];
bool openSet[MAXSTEPLENGTH+1][MAXN]; // a.k.a. "isInQueue"
 
class Solution {
public:
    bool canCross(vector<int>& stones) {
      memset(closedSet, false, sizeof(closedSet));
      memset(openSet, false, sizeof(openSet));
      unordered_map<int, int> stoneLocations;
      int n = stones.size();
      // init
      for (int i = 0; i < n; ++i) {
        int stoneLocation = stones[i];
        stoneLocations[stoneLocation] = i;  
      }
      
      queue<StateTuple> q;
      q.push({0, 0});
      openSet[0][0] = true;
      
      while (!q.empty()) {
        auto expander = q.front(); q.pop();
        
        int expanderPreStepLen = expander.first;
        int expanderStoneIdx = expander.second;
        
        closedSet[expanderPreStepLen][expanderStoneIdx] = true;
        dp[expanderPreStepLen][expanderStoneIdx] = true;
        openSet[expanderPreStepLen][expanderStoneIdx] = false;
          
        if (n-1 == expanderStoneIdx) {
          return true;
        }
        
        int expanderStoneLocation = stones[expanderStoneIdx];
        for (int nextStepLen = expanderPreStepLen-1; nextStepLen <= expanderPreStepLen+1; ++nextStepLen) {
          if (0 >= nextStepLen) continue;
          int nextLocation = expanderStoneLocation+nextStepLen;
          if (stoneLocations.find(nextLocation) == stoneLocations.end()) continue;
          int nextStoneIdx = stoneLocations[nextLocation];
          if (openSet[nextStepLen][nextStoneIdx] || closedSet[nextStepLen][nextStoneIdx]) continue;
          q.push({nextStepLen, nextStoneIdx});
          openSet[nextStepLen][nextStoneIdx] = true;
        }
      }
      
      return false;
    }
};
