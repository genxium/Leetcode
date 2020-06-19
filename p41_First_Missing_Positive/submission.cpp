#define MARKED INT_MIN

class Solution {
public:
    void swap(int *left, int *right) {
        int tmp = *left;
        *left = *right;
        *right = tmp;
    }
    
    int firstMissingPositive(vector<int>& nums) {
        // The soluttion here is to turn "nums" into "marks = [MARKED MARKED ... MARKED _ MARKED ...]"
        //                                                                              ^
        // in O(n) time, such that we can traverse "marks" to find the "first missing positive".
        
        nums.push_back(INT_MAX); // To make sure that if "nums = [1,2,3,...,k]" we could afford "marks[k]" in space.
        
        int n = nums.size();
        for (int i = 0; i < n; ++i) {
            int value = nums[i];
            if (value == MARKED) {
                continue;
            }
            if (value <= 0) {
                continue;
            }
            // value > 0
            if (value >= n) {
                // No need to mark "value >= n", because it's irrelevant for finding "first missing positive".
                continue;
            }
            while (nums[value] != MARKED) {
                swap(&(nums[i]), &(nums[value]));
                nums[value] = MARKED;
                value = nums[i];
                if (value <= 0) break;
                if (value >= n) break;
            }
        }
        
        // By now, "nums" is fully turned into "marks".
        int i = 0;
        for (i = 1; i < n; ++i) {
            if (nums[i] != MARKED) return i;
        }
        return i;
    }
};
