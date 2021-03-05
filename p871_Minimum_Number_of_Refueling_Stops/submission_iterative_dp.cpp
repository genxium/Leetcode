bool const debug = false;
typedef int64_t LL;
int const MAXN = 500;
int const INVALID = -1;

/*
dp[i][c]: the maximum remained fuel when arriving at "stations[i]" and having refueled "c" times
*/
LL dp[MAXN+1][MAXN+2];

/*
test cases
1
1
[]
100
1
[[10,100]]
100
10
[[10,60],[20,30],[30,30],[60,40]]
1000000000
1000000000
[[5,1000000000],[1000,1000000000],[100000,1000000000]]
*/
class Solution {
public:
    int minRefuelStops(int target, int startFuel, vector<vector<int>>& stations) {
        vector<int> targetStation = {target, 0};
        stations.push_back(targetStation);
        int n = stations.size();

        memset(dp, INVALID, sizeof dp);
        if (startFuel >= stations[0][0]) {
            dp[0][0] = max(INVALID, startFuel-stations[0][0]);
            dp[0][1] = max(INVALID, startFuel-stations[0][0]+stations[0][1]);
        }
        
        if (debug) printf("n: %d\n", n);

        for (int i = 1; i < n; ++i) {
            for (int c = 0; c <= i+1; ++c) {
                LL disFromPrevStation = ((LL)stations[i][0] - stations[i-1][0]);
                // no refuel here
                int prevC = c;
                if (dp[i-1][prevC] >= disFromPrevStation) {
                    dp[i][c] = max(dp[i][c], dp[i-1][prevC]-disFromPrevStation);
                }

                if (c >= 1) {
                    // refuel here
                    prevC = c-1;
                    if (dp[i-1][prevC] >= disFromPrevStation) {
                        dp[i][c] = max(dp[i][c], dp[i-1][prevC]-disFromPrevStation+stations[i][1]);
                    }
                }
            }
        }

        for (int c = 0; c <= n; ++c) {
            if (dp[n-1][c] >= 0) return c;
        }

        return INVALID;
    }
};
