typedef int64_t LL;
LL const MOD = 1000000007;

class Solution {
public:
    int minAbsoluteSumDiff(vector<int>& nums1, vector<int>& nums2) {
        set<int> n1Vals;
        for (auto num : nums1) n1Vals.insert(num);
        int n = nums1.size();
        
        int maxReduction = 0;
        
        for (int i = 0; i < n; ++i) {
            if (nums1[i] == nums2[i]) continue;
            auto it = n1Vals.lower_bound(nums2[i]);
            if (it != n1Vals.end()) {
                int cand = abs(nums1[i]-nums2[i]) - abs(*it-nums2[i]);
                maxReduction = max(maxReduction, cand);
            } 
            if (it != n1Vals.begin()) {
                --it;
                int cand = abs(nums1[i]-nums2[i]) - abs(*it-nums2[i]);
                maxReduction = max(maxReduction, cand);
            }
        }
        
        LL ans = -(LL)maxReduction;
        for (int i = 0; i < n; ++i) {
            ans += (LL)abs(nums1[i]-nums2[i]);
            ans %= MOD;
        }
        
        return ans;
    }
};
