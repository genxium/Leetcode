bool debug = false;

class Solution {
public:
    int minOperations(vector<int>& nums1, vector<int>& nums2) {
        int s1 = nums1.size(), s2 = nums2.size();
        int min1 = s1, max1 = s1*6, min2 = s2, max2 = s2*6;
        if (min2 > max1 || min1 > max2) return -1;
        
        int l = max(min1, min2), r = min(max1, max2);
        
        int ans = INT_MAX;
        sort(nums1.begin(), nums1.end());
        sort(nums2.begin(), nums2.end());
        
        int sum1 = 0;
        vector<int> upDiffPs1 = {0}; // prefixSum of (6-nums1[i])
        for (auto num : nums1) {
            sum1 += num;
            int inc = 6-num;
            upDiffPs1.push_back(upDiffPs1.back()+inc);
        }
        
        vector<int> downDiffPs1 = {0}; // prefixSum of (nums1[i]-1)
        for (auto it = nums1.rbegin(); it != nums1.rend(); ++it) {
            int dec = *it-1;
            downDiffPs1.push_back(downDiffPs1.back()+dec);
        }
        
        int sum2 = 0;
        vector<int> upDiffPs2 = {0};
        for (auto num : nums2) {
            sum2 += num;
            int inc = 6-num;
            upDiffPs2.push_back(upDiffPs2.back()+inc);
        }
        
        vector<int> downDiffPs2 = {0};
        for (auto it = nums2.rbegin(); it != nums2.rend(); ++it) {
            int dec = *it-1;
            downDiffPs2.push_back(downDiffPs2.back()+dec);
        }
        
        for (int val = l; val <= r; ++val) {
            int cand1 = 0;
            if (val > sum1) {
                int diff = val-sum1;
                auto it = lower_bound(upDiffPs1.begin(), upDiffPs1.end(), diff);
                cand1 = it-upDiffPs1.begin();
            } else if (val < sum1) {
                int diff = sum1-val;
                auto it = lower_bound(downDiffPs1.begin(), downDiffPs1.end(), diff);
                cand1 = it-downDiffPs1.begin();
            }
            
            int cand2 = 0;
            if (val > sum2) {
                int diff = val-sum2;
                auto it = lower_bound(upDiffPs2.begin(), upDiffPs2.end(), diff);
                cand2 = it-upDiffPs2.begin();
            } else if (val < sum2) {
                int diff = sum2-val;
                auto it = lower_bound(downDiffPs2.begin(), downDiffPs2.end(), diff);
                cand2 = it-downDiffPs2.begin();
            }
            
            int cand = cand1+cand2;
            if (debug) printf("val: %d, cand1:%d, cand2:%d\n", val, cand1, cand2);
            if (-1 == ans || cand < ans) ans = cand;
        }
        
        return ans;
    }
};
