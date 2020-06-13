#define MAXN 20001

class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        int res = 0;
        vector<int> partialSum;
        map<int, int> partialSumCount;
        partialSum.push_back(nums[0]);
        partialSumCount[partialSum.back()] = 1;
        if (nums[0] == k) {
            ++res;
        }
        
        for (int i = 1; i < nums.size(); ++i) {
            int newPartialSum = partialSum.back() + nums[i];
            if (newPartialSum == k) {
                ++res;
            }
            int target = newPartialSum - k;   
            //printf("For partialSum[%d] == %d, target == %d\n", i, newPartialSum, target);
            
            // By far, each element in "partialSumCount" has a position "j < i". 
            map<int, int>::iterator counterpart = partialSumCount.find(target);
            if (counterpart != partialSumCount.end()) {
                res += counterpart->second;
            }
            
            partialSum.push_back(newPartialSum); 
            if (partialSumCount.find(newPartialSum) != partialSumCount.end()) {
                partialSumCount[newPartialSum] += 1;
            } else {
                partialSumCount[newPartialSum] = 1;
            }
        }
       
        return res;
    }
};
