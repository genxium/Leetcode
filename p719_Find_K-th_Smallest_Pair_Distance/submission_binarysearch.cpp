bool debug = false;

/*
test cases
[1,3,1]
1
[1,2,3,3,4,4,5,5,5,5]
7
[1,2,3,3,4,4,5,5,5,5]
18
*/
class Solution {
private:
    bool enough(vector<int> &nums, int x, int k) {
        int count = 0, n = nums.size();
        /*
               1   2     3     4     5     6 
            1    (1,2) (1,3) (1,4) (1,5) (1,6)
            2          (2,3) (2,4) (2,5) (2,6)
            3                (3,4) (3,5) (3,6) 
            4                      (4,5) (4,6)
            5                            (5,6) 
            6 
            
            Each (i,j) represents "abs(nums[j]-nums[i])" where "nums[]" is sorted. 
         */
        for (int i = 0; i < n; ++i) {
            /*
            In the table above, for each row , consisting of {(i,i+1), (i,i+2), ..., (i*n)}, the abs diff candidate "x" can be larger than several elements.
            */
            // Find out how many elements in "rows[i]" are not greater than "i+x".
            int target = nums[i]+x;
            auto it = upper_bound(nums.begin()+i+1, nums.end(), target);
            int contributionThisRow = (int)(it-(nums.begin()+i+1));
            count += contributionThisRow;
        }
        
        if (debug) printf("For abs diff x:%d, there're %d elements in the table which are not greater than it\n", x, count);
        return count >= k;
    }
public:
    int smallestDistancePair(vector<int>& nums, int k) {
        sort(nums.begin(), nums.end());
        int l = 0, r = *nums.rbegin()-*nums.begin();
        while (l < r) {
            int mid = ((l + r) >> 1);
            if (!enough(nums, mid, k)) l = mid+1;
            else r = mid;
        }
        return l;
    }
};
