bool const debug = true;

int const MAXN = 10000;
int maxFromLeft[MAXN], minFromLeft[MAXN], maxFromRight[MAXN], minFromRight[MAXN];

/*
test cases
[1]
0
[0,10]
2
[1,3,6]
3
[7,8,8]
5
*/
class Solution {
public:
    int smallestRangeII(vector<int>& nums, int k) {
        int n = nums.size();
        if (1 == n) return 0;
        
        sort(nums.begin(), nums.end());
        maxFromLeft[0] = minFromLeft[0] = nums[0]+k; 
        for (int i = 1; i < n; ++i) {
            maxFromLeft[i] = nums[i]+k;
            minFromLeft[i] = minFromLeft[i-1]; 
        }
        
        maxFromRight[n-1] = minFromRight[n-1] = nums[n-1]-k; 
        for (int i = n-2; i >= 0; --i) {
            maxFromRight[i] = maxFromRight[i+1];
            minFromRight[i] = nums[i]-k;
        }
        
        int ans = nums[n-1]-nums[0];        
        for (int i = 1; i < n; ++i) {
            vector<int> tmp = {maxFromLeft[i-1], minFromLeft[i-1], maxFromRight[i], minFromRight[i]};
            int localMax = *max_element(tmp.begin(), tmp.end());
            int localMin = *min_element(tmp.begin(), tmp.end());
            
            int localDiff = localMax-localMin;
            ans = min(ans, localDiff);
        }
        
        return ans;
    }
};
