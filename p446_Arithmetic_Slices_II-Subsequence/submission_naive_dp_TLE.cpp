#define MAXN 1000
#define BIGINT int64_t
#define PI pair<int, BIGINT> // {endIdxClosed, diff}
#define PIM map<PI, int> 

/*
test case#1
[2,4,6,8,10]

test case#2
[2,4,6,8,11]

test case#3
[2,2,3,4]

test case#4
[0,2000000000,-294967296]

test case#5
[-8485,7255,10728,2357,-3226,5371,-9805,3758,-6762,-7133,-3962,10234,-9635,1348,5227,-3958,-5065,-101,-6556,3542,-6611,8223,4070,7199,10416,4645,-3409,-5357,-2529,9589,7887,-80,3150,3220,-11077,-5340,-6942,5305,6217,-237,-6681,8067,10977,4665,-2730,-8656,9720,5534,-2662,-5147,619,7609,-9054,-8731,-4125,-7204,-7987,-3656,153,-374,8655,2586,-1785,-11154,-8340,-9105,-3143,8669,8567,-5895,-282,-4288,-10199,-4238,-5841,4425,-4785,7588,9641,2333,4764,9709,2635,-10879,7268,-1208,-2536,-10036,8157,8350,9051,-9658,-448,814,-2632,2340,-3335,-10296,-10436,3631,-823,-3278,5664,5464,-4742,5964,781,691,4732,-9547,4042,5807,-5385,9646,-4238,-1607,546,-936,-1254,4228,-9458,-4150,4376,1691,-1388,-1320,-543,6684,7398,2568,3576,8802,-10162,8267,-9943,-8701,-6396,-3815,9928,-9037,-8733,-6517,-3407,-8212,-8067,-2668,243,-7173,1994,9169,-4620,6266,-6500,6414,-4974,-4616,-10643,9885,-1631,5576,-10302,4132,-3372,5218,3538,-1419,2539,-7139,-8112,-4357,9216,4262,11,7586,-3803,2183,5452,-8709,9341,-5837,-5124,6419,-10005,-9697,4406,922,1486,599,-2740,849,-5830,-329,-2567,3242,-4779,-2365,-6131,2846,3979,-2283]
*/
class Solution {
public:
    int numberOfArithmeticSlices(vector<int>& A) {
      int n = A.size(), ans = 0;
      vector<PIM> dp; // "dp[length][endIdxClosed][diff] == x" means that count of ArithmeticSubsequence"s that end exactly at "endIdxClosed" with "length" is "x"

      // Init
      for (int length = 0; length <= n; ++length) {
        PIM singleDict;
        dp.push_back(singleDict);
      }
      
      for (int endIdxClosed = 0; endIdxClosed < n; ++endIdxClosed) {
        dp[1][{endIdxClosed,0}] = 1; // For each "1-length ArithmeticSubsequence" that ends at each "A[i]", we have only 1 count and "diff == 0".  
      }

      // Loop
      for (int length = 2; length <= n; ++length) {
        int preLength = length-1;
        auto &preDict = dp[preLength];
        int endIdxClosedLower = (length-1); 
        for (int endIdxClosed = endIdxClosedLower; endIdxClosed < n; ++endIdxClosed) {
          for (auto it = preDict.begin(); it != preDict.end(); ++it) {
            auto prevKey = it->first;
            int prevEndIdxClosed = prevKey.first;
            if (prevEndIdxClosed >= endIdxClosed) continue;
            
            BIGINT prevDiff = prevKey.second;
            int prevCount = it->second;
            BIGINT diff = (BIGINT)A[endIdxClosed]-(BIGINT)A[prevEndIdxClosed];
            if (preLength != 1 && prevDiff != diff) continue;
            
            if (length >= 3) {
              //printf("@endIdxClosed:%d, adding prevCount==%d from dp[preLength:%d][{prevEndIdxClosed:%d, prevDiff:%d}]\n", endIdxClosed, prevCount, preLength, prevEndIdxClosed, prevDiff);
              ans += prevCount;
            }
            auto &curDict = dp[length];
            PI key = {endIdxClosed, diff};
            if (curDict.find(key) == curDict.end()) {
              curDict[key] = prevCount;
            } else {
              curDict[key] += prevCount;  
            }
          }
        }
      }
      
      return ans;
    }
};
