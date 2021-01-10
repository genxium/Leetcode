bool const debug = false;
int const INVALID = -1, MAXN = 12, MAXK = 12;
int const MAXBM = (1 << MAXN); // "bm" is "bitmask" 
int cache[MAXBM][MAXK+1];
int subsetTimeSumCache[MAXBM];

class Solution {
public:
    int minOfMaxsumForEach(vector<int>& jobs, int remainedBm, int remainedWorkerCount) {
        if (INVALID != cache[remainedBm][remainedWorkerCount]) return cache[remainedBm][remainedWorkerCount]; 
        if (0 == remainedBm) return cache[0][remainedWorkerCount] = 0;
        
        // 0 != remainedBm
        if (0 == remainedWorkerCount) return (cache[remainedBm][remainedWorkerCount] = INT_MAX);
        
        // traverse all the submasks of "remainedBm"
        int toRet = INT_MAX;
        for (int subsetBitRep = remainedBm; subsetBitRep > 0; subsetBitRep=((subsetBitRep-1)&remainedBm)) {
            int currentWorkerSum = 0;
            if (INVALID != subsetTimeSumCache[subsetBitRep]) currentWorkerSum = subsetTimeSumCache[subsetBitRep];
            else {
                for (int j = 0; (1 << j) <= subsetBitRep; ++j) {
                    if ((1 << j) & subsetBitRep) currentWorkerSum += jobs[j];
                }
                subsetTimeSumCache[subsetBitRep] = currentWorkerSum;
            }
            
            int restMinOfMaxsumForEach = minOfMaxsumForEach(jobs, remainedBm^subsetBitRep, remainedWorkerCount-1);
            int tmp = max(currentWorkerSum, restMinOfMaxsumForEach);
            toRet = min(toRet, tmp);
        }
        
        return cache[remainedBm][remainedWorkerCount] = toRet;
    }
        
    int minimumTimeRequired(vector<int>& jobs, int k) {
        memset(cache, INVALID, sizeof cache);
        memset(subsetTimeSumCache, INVALID, sizeof(subsetTimeSumCache));
        
        int n = jobs.size();
        int remainedBm = (1 << n)-1; // initially all jobs are available
        int ans = minOfMaxsumForEach(jobs, remainedBm, k);
        return ans;
    }
};
