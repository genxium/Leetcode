bool const debug = false;
int const MOD = 1000000007;

/*
test cases
[1,1,1]
[1,2,2,2,5,0]
[3,2,1]
[0,0,0]
[0,0,0,0,0]
*/
class Solution {
public:
    int waysToSplit(vector<int>& nums) {
        int n = nums.size();
        vector<int> ps = {0}; // prefixSum
        for (int i = 0; i < n; ++i) {
            ps.push_back(ps.back() + nums[i]);
        }
        
        int overallSum = ps.back();
        
        int rightSplitMin = (int)ceil(overallSum*1.0/3.0);
        
        int preceding2SplitsSumMax = overallSum - rightSplitMin;
        
        int ans = 0;
        for (int i = 2; i < n && ps[i] <= preceding2SplitsSumMax; ++i) {
            // Note that "ps[i] == sum(nums[0], ..., nums[i-1])".
            // Right split is [i+1, n-1).
            int rightSplitSum = overallSum - ps[i];
            int midSplitMin = (int)ceil(ps[i]*1.0/2.0);
            int midSplitMax = rightSplitSum;
            
            // Left split is [1, j) and mid split if [j, i].
            int leftSplitMax = ps[i]-midSplitMin;
            auto leftSplitMaxPosIt = upper_bound(ps.begin(), ps.begin()+i, leftSplitMax); 
            if (leftSplitMaxPosIt == ps.begin()+i) {
                if (ps[i]-ps[i-1] < midSplitMin) {
                    continue;
                }
            }
            
            int leftSplitMin = ps[i]-midSplitMax;
            auto leftSplitMinPosIt = lower_bound(ps.begin()+1, ps.begin()+i, leftSplitMin); // The "leftSplit" should contain at least "nums[0]", i.e. "ps[1]".
            if (leftSplitMinPosIt == ps.begin()+i) continue;
            
            if (debug) printf("@i:%d, midSplitMin:%d, midSplitMax:%d, leftSplitMax:%d, leftSplitMin:%d, leftSplitMaxPosIt:%d, leftSplitMinPosIt:%d\n", i, midSplitMin, midSplitMax, leftSplitMax, leftSplitMin, leftSplitMaxPosIt-ps.begin(), leftSplitMinPosIt-ps.begin());
            ans += max((int)(leftSplitMaxPosIt-leftSplitMinPosIt), 0);
            ans %= MOD;
        }
        
        return ans;
    }
};
