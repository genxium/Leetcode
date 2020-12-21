bool const debug = false;
typedef vector<int> VI;
int const INVALID = -1;
int const MAXN = 100;
int dp[6*MAXN]; // "dp[k] == x" means that using some of "rotatedCuboids[0, ..., k]" where "rotatedCuboids[k]" MUST BE INCLUDED, we can have at most "x" total height  

vector<VI> ROTATIONS = {
    {0, 1, 2},
    {0, 2, 1},
    {1, 0, 2},
    {1, 2, 0},
    {2, 0, 1},
    {2, 1, 0}
};

/*
test cases
[[50,45,20],[95,37,53],[45,23,12]]
[[38,25,45],[76,35,3]]
[[7,11,17],[7,17,11],[11,7,17],[11,17,7],[17,7,11],[17,11,7]]
[[74,7,80],[7,52,61],[62,41,37],[91,58,26],[88,98,5],[72,93,23],[56,58,94],[88,8,64],[32,55,5]]
[[88,45,69],[88,15,96],[49,78,68],[96,72,80],[80,86,13],[9,99,73],[94,42,39],[30,19,58],[10,32,21],[20,52,85]]
*/
class Solution {
public:
    bool isCompatible(VI const &lhs, VI const &rhs) {
        if (lhs[3] == rhs[3]) return false;

        bool allSatisfied = true;
        for (int i = 0; i < 3; ++i) {
            if (lhs[i] < rhs[i]) {
                allSatisfied = false;
                break;
            }
        }
        return allSatisfied;
    }

    int maxHeight(vector<vector<int>>& cuboids) {
        int n = cuboids.size();
        memset(dp, INVALID, sizeof dp);
        vector<VI> rotatedCuboids;
        for (int i = 0; i < n; ++i) {
            VI &cuboid = cuboids[i];
            for (int j = 0; j < 6; ++j) {
              VI &rotationVec = ROTATIONS[j];
              rotatedCuboids.push_back({cuboid[rotationVec[0]], cuboid[rotationVec[1]], cuboid[rotationVec[2]], i});
            }
        }
        sort(rotatedCuboids.begin(), rotatedCuboids.end(), greater<VI>());

        int m = rotatedCuboids.size(); 
        
        if (debug) {
            for (int i = 0; i < m; ++i) {
                printf("rotatedCuboids[i:%d] == {%d, %d, %d, idx:%d}\n", i, rotatedCuboids[i][0], rotatedCuboids[i][1], rotatedCuboids[i][2], rotatedCuboids[i][3]);
            }
        }
        int ans = 0;
        
        // init
        dp[0] = rotatedCuboids[0][2];
        
        // loop 
        for (int k = 1; k < m; ++k) {
            auto &anchorItem = rotatedCuboids[k]; 
            int heightInc = anchorItem[2];
            dp[k] = heightInc; // using only "anchorItem" itself
            for (int i = 0; i < k; ++i) {
                if (debug) printf("Checking if rotatedCuboids[k:%d] {%d, %d, %d, idx:%d} can be put on rotatedCuboids[i:%d] {%d, %d, %d, idx:%d}\n", k, rotatedCuboids[k][0], rotatedCuboids[k][1], rotatedCuboids[k][2], rotatedCuboids[k][3], i, rotatedCuboids[i][0], rotatedCuboids[i][1], rotatedCuboids[i][2], rotatedCuboids[i][3]);
                
                if (false == isCompatible(rotatedCuboids[i], anchorItem)) {
                    if (debug) printf("\tno\n");
                    continue;
                }
                if (debug) printf("\tyes\n");
                dp[k] = max(dp[k], dp[i] + heightInc);      
            }
            ans = max(ans, dp[k]);
        }
        
        return ans;
    }
};

