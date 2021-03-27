class Solution {
public:
    bool isPossible(int k, int h, vector<int>& piles) {
        int timeNeeded = 0;
        for (auto &pile : piles) {
            timeNeeded += (int)ceil((double)pile/k);
            if (timeNeeded > h) return false;
        }
        
        return true;
    }
    
    int minEatingSpeed(vector<int>& piles, int h) {
        int l = 1, r = 1000000000;
        int ans = INT_MAX;
        while (l <= r) {
            int k = ((l+r) >> 1);
            if (isPossible(k, h, piles)) {
                ans = min(ans, k);
                r = k-1;
            } else {
                // k is too small
                l = k+1;
            }
        }
        
        return ans;
    }
};
