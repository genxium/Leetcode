/*
test cases
1
3
2
4
5
*/
class Solution {
public:
    int reachNumber(int target) {
        target = abs(target);
        
        int lv = 1;
        int position = 0; // prefixSum 
        for (lv = 1; ; ++lv) {
            int newPosition = position+lv;
            if (newPosition == target) {
                return lv;
            }
            if (newPosition > target) {
                break;
            }
            position = newPosition;
        }
        
        // Now that "position < target" but "position+lv > target"
        int overDistance = (position+lv - target);
        /*
        1. This "overDistance" is in range [0, lv), due to "position < target".
        2. If "overDistance" is even, we know that it's also in range [2, 2*lv) which is the same as [2, 2*(lv-1)], thus we can choose EXACTLY 1 PREVIOUS STEP to reverse the sign and make "overDistance" zero.
        */
        if (overDistance % 2 == 0) return lv;
        
        /*
        3. If "overDistance" is odd, we move on adding "lv" till it becomes even such that #2 applies.
        */
        while (overDistance % 2 == 1) {
            ++lv;
            overDistance += lv;
        }
        
        return lv;
    }
};
