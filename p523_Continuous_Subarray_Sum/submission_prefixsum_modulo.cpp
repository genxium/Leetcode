/*
test cases
[23,2,4,6,7]
6
[23,2,6,4,7]
6
[23,2,6,4,7]
1
[23,2,6,4,7]
101
[23,2,6,4,7]
0
[0,1,0]
0
[0,0]
0
*/
class Solution {
public:
    bool checkSubarraySum(vector<int>& nums, int k) {
        vector<int> ps = {0}; // prefixSum
        unordered_map<int, int> firstPos;
        firstPos[0] = -1;
        for (int i = 0; i < nums.size(); ++i) {
            int num = nums[i];
            if (0 == k) {
                int sum = (ps.back()+num);
                if (firstPos.find(sum) != firstPos.end() && firstPos[sum] != i-1) {
                    return true;
                }
                if (firstPos.find(sum) == firstPos.end()) {
                    firstPos[sum] = i;   
                }
                ps.push_back(sum);
            } else {
                int modSum = (ps.back()+num)%k;
                if (firstPos.find(modSum) != firstPos.end() && firstPos[modSum] != i-1) {
                    return true;
                }
                if (firstPos.find(modSum) == firstPos.end()) {
                    firstPos[modSum] = i;   
                }
                ps.push_back(modSum);    
            }
        }
        return false;
    }
};
