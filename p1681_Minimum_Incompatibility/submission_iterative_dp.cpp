bool const debug = false;
int const INVALID = -1;
int const MAXN = 16;
int dp[(1 << MAXN)]; // That "dp[usedCandidates] == x" means using the candidates represented by bitmask "usedCandidates", we can obtain "minimum total incompatibility == x".
int subsets[(1 << MAXN)];
/*
test cases
[1,2,1,4]
2
[6,3,8,1,3,1,2,2]
4
[5,3,3,6,3,3]
3
[5,3,3,6,3,3,1,2,3,4,5,6]
4
[5,3,3,6,3,3,1,2,3,4,5,6,7,8,9,10]
4
[7,3,16,15,1,13,1,2,14,5,3,10,6,2,7,15]
8
[13,13,2,2,4,6,3,4,3,12,13,8,3]
1
*/
class Solution {
public:
    void genPossibleSubSets(int l, vector<int>& nums, int n, int bitRepUpper) {
        for (int activeSubsetBitmask = 0; activeSubsetBitmask < bitRepUpper; ++activeSubsetBitmask) {
            if (__builtin_popcount(activeSubsetBitmask) != l) continue;
            bool hasDupVal = false;
            unordered_set<int> vals;
            int asMin = INT_MAX, asMax = INT_MIN;
            for (int i = 0; i < n; ++i) {
                if (!((1 << i) & activeSubsetBitmask)) continue;
                if (vals.find(nums[i]) != vals.end()) {
                    hasDupVal = true;
                    break;
                }
                vals.insert(nums[i]);
                asMin = min(asMin, nums[i]);
                asMax = max(asMax, nums[i]);
            }
            if (hasDupVal) continue;
            subsets[activeSubsetBitmask] = asMax - asMin;
        }
    }
        
    int minimumIncompatibility(vector<int>& nums, int k) {
        int n = nums.size(), l = n/k, bitRepUpper = (1 << n);

        // init
        memset(subsets, INVALID, sizeof(subsets));
        memset(dp, INVALID, sizeof(dp));
        dp[0] = 0;
        
        genPossibleSubSets(l, nums, n, bitRepUpper);
                
        // solve
        for (int usedCandidates = 0; usedCandidates < bitRepUpper; ++usedCandidates) {
            if (__builtin_popcount(usedCandidates) % l != 0) continue; // Definitely not a worth investigating state of "usedCandidates"
            // traverse all the submasks of "usedCandidates"
            for (int subsetBitRep = usedCandidates; subsetBitRep > 0; subsetBitRep=((subsetBitRep-1)&usedCandidates)) {
                if (INVALID == subsets[subsetBitRep]) continue;
                int prevUsedCandidates = (usedCandidates^subsetBitRep);
                if (INVALID == dp[prevUsedCandidates]) continue;
                int candidate = dp[prevUsedCandidates] + subsets[subsetBitRep];
                if (INVALID == dp[usedCandidates]) dp[usedCandidates] = candidate;
                else dp[usedCandidates] = min(dp[usedCandidates], candidate);
            }

            // alternative version if "subsets: unordered_map<int, int>"
            /*
            for (auto it : subsets) {
                int subsetBitRep = it.first;
                bool contained = ((usedCandidates & subsetBitRep) == subsetBitRep);
                if (!contained) continue;
                int prevUsedCandidates = (usedCandidates^subsetBitRep);
                if (INVALID == dp[prevUsedCandidates]) continue;
                int candidate = dp[prevUsedCandidates] + subsets[subsetBitRep];
                if (INVALID == dp[usedCandidates]) dp[usedCandidates] = candidate;
                else dp[usedCandidates] = min(dp[usedCandidates], candidate);
            }
            */
        }
        
        return dp[bitRepUpper-1];
    }
};

