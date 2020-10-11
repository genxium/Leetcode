#define MAXN 1000
#define BIGINT int64_t
#define IM unordered_map<BIGINT, int>

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
      vector<IM> dp; // "dp[endIdxClosed][diff] == x" means that count of ArithmeticSubsequence"s that end exactly at "endIdxClosed" is "x", REGARDLESS OF the "length" of the subsequences (YET only "length >= 2" will be accounted in dp[][])
      
      // Init
      for (int endIdxClosed = 0; endIdxClosed < n; ++endIdxClosed) {
        IM singleDIct;
        dp.push_back(singleDIct);
      }
      int lengthOneCount = 0, lengthTwoCount = 0; // will remove the count of all the "length < 3" subsequences at the end
      /*
      [WARNING]
      
      It's by design that we DON'T initialize "dp[endIdxClosed:0][diff:0] = 1" here, which will contaminate the checkvalve logic "dp[prevEndIdxClosed].find(targetDiff) != dp[prevEndIdxClosed].end()" below!
      */
      
      // Loop
      for (int endIdxClosed = 0; endIdxClosed < n; ++endIdxClosed) {
        ++ans; // a new "length == 1" subsequence can be formed by "A[endIdxClosed]" alone
        ++lengthOneCount;
        for (int prevEndIdxClosed = 0; prevEndIdxClosed < endIdxClosed; ++prevEndIdxClosed) {
          // a new "length == 2" subsequence can be formed by [A[prevEndIdxClosed], A[endIdxClosed]]
          ++ans; 
          ++lengthTwoCount;
          BIGINT targetDiff = (BIGINT)A[endIdxClosed]-(BIGINT)A[prevEndIdxClosed];
          ++dp[endIdxClosed][targetDiff];
          
          // form the "length >= 3" subsequences that end at "endIdxClosed"
          int prevCount = 0;
          if (dp[prevEndIdxClosed].find(targetDiff) != dp[prevEndIdxClosed].end()) {
            prevCount = dp[prevEndIdxClosed][targetDiff];
          }
          
          ans += prevCount;
          dp[endIdxClosed][targetDiff] += prevCount;
        }
      }
      
      ans -= (lengthOneCount + lengthTwoCount);
      return ans;
    }
};
