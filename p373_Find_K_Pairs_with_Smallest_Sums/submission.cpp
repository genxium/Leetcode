#define VI vector<int>

class Solution {
public:
    vector<vector<int>> kSmallestPairs(vector<int>& nums1, vector<int>& nums2, int k) {
        priority_queue<VI, vector<VI>, greater<VI>> pq; // a MinHeap based on "nums1[i]+nums2[j]"
        
        /*
        TODO
        
        Is there a way to avoid using "NestedLoop"?
        */
        for (int i = 0; i < nums1.size(); ++i) {
            for (int j = 0; j < nums2.size(); ++j) {
                int sum = nums1[i]+nums2[j];
                pq.push({sum, nums1[i], nums2[j]});
            }
        }
        
        vector<VI> ans;
        while (k-- && !pq.empty()) {
            auto top = pq.top(); pq.pop();
            ans.push_back({top[1], top[2]});
        }
        
        return ans;
    }
};
