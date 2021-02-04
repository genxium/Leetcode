bool const debug = false;
int const INVALID = -1, MAXN = 16, MAXK = 16;
int const MAXBM = (1 << MAXN); // "bm" is "bitmask" 
int cache[MAXBM][MAXK+1];
int subsetSumCache[MAXBM];

/*
test cases
[4,3,2,3,5,2,1]
4
[3,5,3,2,5,5,5,2,5,5,5,5,5,5,5,5]
14
*/
class Solution {
public:
    int minOfMaxsumForEach(vector<int>& nums, int remainedBm, int remainedCount) {
        if (INVALID != cache[remainedBm][remainedCount]) return cache[remainedBm][remainedCount]; 
        if (0 == remainedBm) return cache[0][remainedCount] = 0;
        
        // 0 != remainedBm
        if (0 == remainedCount) return (cache[remainedBm][remainedCount] = INT_MAX);
        
        // traverse all the submasks of "remainedBm"
        int toRet = INT_MAX;
        for (int subsetBitRep = remainedBm; subsetBitRep > 0; subsetBitRep=((subsetBitRep-1)&remainedBm)) {
            int currentSum = 0;
            if (INVALID != subsetSumCache[subsetBitRep]) currentSum = subsetSumCache[subsetBitRep];
            else {
                for (int j = 0; (1 << j) <= subsetBitRep; ++j) {
                    if ((1 << j) & subsetBitRep) currentSum += nums[j];
                }
                subsetSumCache[subsetBitRep] = currentSum;
            }
            
            int restMinOfMaxsumForEach = minOfMaxsumForEach(nums, remainedBm^subsetBitRep, remainedCount-1);
            int tmp = max(currentSum, restMinOfMaxsumForEach);
            toRet = min(toRet, tmp);
        }
        
        return cache[remainedBm][remainedCount] = toRet;
    }
    
    bool dfs(vector<int>& nums, int remainedBm, int remainedCount, int targetGroupSum) {
        if (INVALID != cache[remainedBm][remainedCount]) return cache[remainedBm][remainedCount]; 
        if (0 == remainedBm) return (0 == remainedCount);
        if (0 == remainedCount) return (0 == remainedBm);
        
        // traverse all the submasks of "remainedBm"
        int toRet = INT_MAX;
        for (int subsetBitRep = remainedBm; subsetBitRep > 0; subsetBitRep=((subsetBitRep-1)&remainedBm)) {
            int currentSum = 0;
            if (INVALID != subsetSumCache[subsetBitRep]) currentSum = subsetSumCache[subsetBitRep];
            else {
                for (int j = 0; (1 << j) <= subsetBitRep; ++j) {
                    if ((1 << j) & subsetBitRep) currentSum += nums[j];
                }
                subsetSumCache[subsetBitRep] = currentSum;
            }
            
            if (currentSum != targetGroupSum) continue;
            
            bool canSuffice = dfs(nums, remainedBm^subsetBitRep, remainedCount-1, targetGroupSum);
            if (canSuffice) return true;
        }
        
        return false;
    }
    
    bool canPartitionKSubsets(vector<int>& nums, int k) {
        int sum = 0;
        for (auto num : nums) sum += num;
        if ((sum % k) != 0) return false;
        
        memset(cache, INVALID, sizeof cache);
        memset(subsetSumCache, INVALID, sizeof(subsetSumCache));
        
        int n = nums.size();
        int remainedBm = (1 << n)-1; // initially all nums are available
        /*
        int minOfMax = minOfMaxsumForEach(nums, remainedBm, k); // Will result in TLE.
        return (minOfMax == sum/k);   
        */
        int targetGroupSum = sum/k;
        return dfs(nums, remainedBm, k, targetGroupSum);
    }
};
