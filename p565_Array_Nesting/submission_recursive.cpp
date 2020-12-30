bool const debug = false;
int const MAXN = 20000;
int cache[MAXN];
bool visited[MAXN];

/*
test cases
[5,4,0,3,1,6,2]
*/
class Solution {
public:
    int length(int i, vector<int>& nums) {
        visited[i] = true; // It's important to mark "visited[i]" in a pre-order.
        if (0 < cache[i]) return cache[i];
        int nextI = nums[i]; 
        if (nums[nextI] == nums[i]) return cache[i] = 1;
        if (visited[nextI]) return cache[i] = 1;
        return 1+length(nextI, nums);
    }
    
    int arrayNesting(vector<int>& nums) {
        memset(cache, 0, sizeof cache);
        memset(visited, false, sizeof visited); 
        int ans = 0;
        int n = nums.size();
        for (int i = 0; i < n; ++i) {
            int cand = length(i, nums);
            ans = max(ans, cand);
        }
        return ans;
    }
};
