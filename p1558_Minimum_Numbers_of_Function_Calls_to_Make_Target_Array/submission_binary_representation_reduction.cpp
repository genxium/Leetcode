/*
The problem is equivalent to reducing "nums" to all zeros by subtracting 1 each or right shift the whole. 
*/

class Solution {
public:
    int minOperations(vector<int>& nums) {
        int ans = 0;
        bool hasNonZero = true;
        
        while (hasNonZero) {
            hasNonZero = false;
            for (auto &num : nums) {
                //printf("%d: ", num);
                if (num > 0) {
                    if ((num & 1) > 0) {
                        //printf("ans+1 \n");
                        ans += 1;
                        num -= 1; // in fact this operation can be omitted
                    }
                    num >>= 1;
                    if (num > 0) {
                        hasNonZero = true;
                    }
                }
            }
            
            if (hasNonZero) {
                ans += 1;  
            }
        }
        
        return ans;
    }
};
