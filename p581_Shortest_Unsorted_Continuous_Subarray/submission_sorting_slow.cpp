typedef pair<int, int> PII;
typedef vector<int> VI;

/*
test cases
[2,6,4,8,10,9,15]
[1,2,3,4]
[1,2,2,3,4]
[1,2,3,2,4,3,4]
[1]
*/
class Solution {
public:
    int findUnsortedSubarray(vector<int>& nums) {
        // The key idea is that for each nums[r], find "the leftmost position l such that nums[l] > nums[r]" to form an interval [l, r], then the answer is the leftmost & rightmost of all intervals.
        
        int n = nums.size();
        vector<VI> items;
        for (int i = 0; i < n; ++i) {
            items.push_back({nums[i], i, 0});
        }
        sort(items.begin(), items.end(), less<VI>());
        
        unordered_map<int, int> ranks;
        
        for (int i = 0; i < n; ++i) {
            items[i][2] = i;
            ranks[items[i][1]] = items[i][2];
        }
        
        int l = -1, r = -1;
        for (int i = 0; i < n; ++i) {
            if (ranks[i] != i) {
                if (-1 == l)  l = i; 
                if (i > r) r = i;
            }
        }
        
        if (r == l) return 0;
        return r-l+1;
    }
};
