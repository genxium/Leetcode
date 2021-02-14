#define larger(x, y) x > y ? x : y
#define smaller(x, y) x < y ? x : y
/*
test cases
[7,2,5,10,8]
2
[1,2,3,4,5]
2
[1,4,4]
3
[7,2,5,10,8]
3
[7,2,103,0,5,10,0,8,999999,10]
3
[7,2,103,0,5,10,0,8,99,10]
5
[2,3,1,2,4,3]
5
*/
class Solution {
public:
    bool isAvailable(int n, int m, vector<int> &nums, int largestGroupSum) {
        int currentGroupSum = 0;
        for (int i = 0; i < n;) {
            if (nums[i] > largestGroupSum) {
                return false;
            }
            int tmp = currentGroupSum + nums[i];
            if (tmp > largestGroupSum) {
                // "nums[i]" MUST be the start of the next group
                --m;
                currentGroupSum = 0;
                continue;
            }   
            currentGroupSum = tmp;
            ++i;
        }
        
        // wrap up
        --m;

        if (m >= 0) {
            // In this case we can always cut the current grouping into even smaller groups.
            return true;
        }
        return false;
    }

    int splitArray(vector<int>& nums, int m) {
        int l = 0, r = 0;
        int n = nums.size();
        for (int i = 0; i < n; ++i) {
            r += nums[i];
        }

        int ans = INT_MAX;
        while (l <= r) {
            int cand = ((l+r) >> 1);
            if (isAvailable(n, m, nums, cand)) {
                ans = smaller(ans, cand);
                r = cand-1;
            } else {
                l = cand+1;
            }
        }

        return ans;
    }
};

