typedef uint64_t ULL;
ULL const MOD = 1000000007;
bool const debug = false;

/*
test cases
[3,1,2,4]
[11,81,94,43,3]
[71,55,82,55]
*/
class Solution {
public:
    int sumSubarrayMins(vector<int>& heights) {
        int n = heights.size();
        
        vector<int> l(n, n); // l[i] is the leftmost index such that anyOf(heights[l[i]], heights[l[i]+1], ..., heights[i-1]) >= heights[i]
        vector<int> r(n, n); // r[i] is the rightmost index such that heights[i] < anyOf(heights[i+1], ..., heights[r[i]-1], heights[r[i]]), note that we demand a strict inequality here to avoid duplicated counts
        
        /*
        It's NOT INTUITIVE why this approach can count every subarray. 
        
        Think about a affiliate array "rSmaller[i]", which marks the location of the first location to the right of the strictly smaller element than "heights[i]". It's obvious that "r[i] < rSmaller[i]", and moreover between "[i, rSmaller[i])" there could be multiple "heights[j] == heights[i]" -- which we're actually making use of to ensure that subarrays containing "[i, rSmaller[i])" and using "heights[i]" as the min is not missed in counting.
        */
        
        l[0] = 0;    
        for (int i = 1; i < n; ++i) {
            if (heights[i-1] < heights[i]) {
                l[i] = i;
            } else {
                int temp = l[i-1];
                while(temp-1 >= 0 && heights[temp-1] >= heights[i]) {
                    temp = l[temp-1];
                }
                l[i] = temp;
            }
        }
            
        r[n-1] = n-1;
        for (int i = n-2; i >= 0; --i) {
            if (heights[i+1] <= heights[i]) {
                r[i] = i;
            } else {
                int temp = r[i+1];
                while(temp+1 < n && heights[temp+1] > heights[i]) {
                    temp = r[temp+1];
                }
                r[i] = temp;
            }
        }
        
        ULL ans = 0;
        for (int i = 0; i < n; ++i) {
            ULL tmp = (ULL)(i-l[i]+1)*(ULL)(r[i]-i+1);
            tmp %= MOD;
            tmp *= (ULL)heights[i];
            tmp %= MOD;
            if (debug) printf("l[i:%d] == %d, r[i:%d] == %d, for arr[i:%d] == %d\n", i, l[i], i, r[i], i, heights[i]);
            ans += tmp;
            ans %= MOD;
        }
        
        return ans;
    }
};
