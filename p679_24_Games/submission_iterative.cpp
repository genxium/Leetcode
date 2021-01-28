bool const debug = false;
int const INVALID = -1;
int const MAXN = 4;
double const EPS = 1e-12;
unordered_set<double> possibleValues[MAXN][MAXN]; // {i, j} -> {val1, val2, ...}, where nums[i, ..., j] is in a closure/parentheses

/*
test cases
[4,1,8,7]
[1,2,1,2]
[8,1,6,6]
[3,3,8,8]
*/
class Solution {
public: 
    bool judgePoint24(vector<int>& nums) {
        int n = nums.size();
        sort(nums.begin(), nums.end());
        do {
            if (debug) {
                printf("Checking new permutation\n");
                for (int i = 0; i < n; ++i) printf("%d ", nums[i]);
                puts("");
            }
            for (int i = 0; i < n; ++i) {
                for (int j = i; j < n; ++j) {
                    possibleValues[i][j].clear();
                    if (i == j) {
                        possibleValues[i][j].insert((double)nums[i]);
                    }
                }
            }

            for (int len = 2; len <= n; ++len) {
                int iUpper = n-len+1;
                for (int i = 0; i < iUpper; ++i) {
                    int j = i+len-1;
                    for (int k = i; k < j; ++k) {
                        for (auto lVal : possibleValues[i][k]) {
                            for (auto rVal : possibleValues[k+1][j]) {
                                auto v = lVal+rVal;
                                possibleValues[i][j].insert(v);
                                v = lVal-rVal;
                                possibleValues[i][j].insert(v);
                                v = lVal*rVal;
                                possibleValues[i][j].insert(v);
                                if (rVal != 0) {
                                    v = lVal/rVal;
                                    possibleValues[i][j].insert(v);                                    
                                }
                            }
                        }
                    }
                }
            }
            
            for (auto v : possibleValues[0][n-1]) {
                if (fabs(v-24) < EPS) return true;
            }
        } while (next_permutation(nums.begin(), nums.end()));

        return false;
    }
};
