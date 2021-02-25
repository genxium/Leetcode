/*
test cases
[1,1,2]
[1,2,3]
*/
class Solution {
public:
    bool xorGame(vector<int>& nums) {
        int xorVal = 0;
        for (auto num : nums) xorVal ^= num;
        /*
        If "xorVal > 0 && nums.size() is even", it can be proved by contradiction that there must be some nums[i] remained such that "(xorVal^nums[i]) > 0". 
        
        Assume that in this case every "(xorVal^nums[i]) == 0", then "XOR{(xorVal^nums[i])} == 0", which means "XOR(xorVal | even times)^XOR{nums[i]} == 0", which is CONTRADICTING "xorVal > 0".
        
        Therefore "xorVal > 0 && nums.size() is even" is also a "WinState" as "xorVal == 0".
        */
        return (xorVal == 0 || nums.size()%2 == 0);
    }
};
