int const MAXNUM = 1000000000;

class Solution {
public:
    int minimumSize(vector<int>& nums, int maxOperations) {
        int l = 1, r = MAXNUM;
        int ans = MAXNUM;
        
        while (l <= r) {
            int m = (l+r)/2;
            int opsRequired = 0;
            for (auto num : nums) {
                if (num <= m) continue;
                opsRequired += (num-1)/m; // each time break into (num-m) + m
            }
            
            if (opsRequired <= maxOperations) {
                r = m-1;
                ans = min(ans, m);
            } else {
                l = m+1;
            }
        }
        
        return ans;
    }
};
