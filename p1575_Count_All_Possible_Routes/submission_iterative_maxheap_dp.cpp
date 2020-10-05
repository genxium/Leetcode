#define MAXN 100
#define MAXF 200
#define ULL uint64_t
#define MOD 1000000007
#define PI pair<int, int>
ULL dp[MAXF+1][MAXN]; // "dp[f][k] == x" means that from "city:start", there're "x" paths to arrive at "city:k" with "f" fuel remaining
bool closedSet[MAXF+1][MAXN];
bool openSet[MAXF+1][MAXN]; // a.k.a. "isInQueue"

int fuelCost(int cityLhs, int cityRhs, vector<int>& locations) {
  return abs(locations[cityLhs] - locations[cityRhs]);
}

/*
test case#1
[2,3,6,8,4]
1
3
5

test case#2
[5,2,1]
0
2
3

test case#3
[2,1,5]
0
0
3

test case#4
[1,2,3]
0
2
40
*/
class Solution {
public:
    int countRoutes(vector<int>& locations, int start, int finish, int fuel) {
      int n = locations.size();
      memset(dp, 0, sizeof(dp));
      memset(closedSet, false, sizeof(closedSet));
      memset(openSet, false, sizeof(openSet));

      // init
      dp[fuel][start] = 1;
      
      priority_queue<PI, vector<PI>, less<PI>> q; // by default a "MaxHeap"
      q.push({fuel, start});
      openSet[fuel][start] = true;
      
      // loop
      while (!q.empty()) {
        auto expander = q.top(); q.pop();
        int expanderFuel = expander.first, expanderCity = expander.second;
        
        closedSet[expanderFuel][expanderCity] = true; // the "MaxHeap" makes sure that when a "StateTuple" {expanderFuel, expanderCity} enters the "closedSet", all "StateTuple"s with more "fuel" values are already visited
        openSet[expanderFuel][expanderCity] = false;
        
        //printf("dp[expanderFuel:%d][expanderCity:%d] == %d -> \n", expanderFuel, expanderCity, dp[expanderFuel][expanderCity]);
        for (int i = 0; i < n; ++i) {
          if (i == expanderCity) continue;
          int fc = fuelCost(expander.second, i, locations);
          if (fc > expanderFuel) continue;
          int newFuel = expanderFuel - fc;
          dp[newFuel][i] += dp[expanderFuel][expanderCity];
          dp[newFuel][i] %= MOD;
          //printf("\tdp[newFuel:%d][i:%d] == %d\n", newFuel, i, dp[newFuel][i]);
          if (closedSet[newFuel][i] || openSet[newFuel][i]) continue;
          q.push({newFuel, i});
          openSet[newFuel][i] = true;
        }
      }
      
      ULL ans = 0;
      for (int f = 0; f <= fuel; ++f) {
        ans += dp[f][finish];
        ans %= MOD;
      }
      
      return ans;
    }
};
