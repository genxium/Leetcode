class Solution {
public:
    void swap(int* lhs, int* rhs) {
        int tmp = *lhs;
        *lhs = *rhs;
        *rhs = tmp;
    }
    
    void sortColors(vector<int>& nums) {
        /*
        test case #1
        [1,0,2]
        
        test case #2
        [1,1,1,1,1,1,1,1,2,2,2,2,2,0,2,2,0,1,1,0,0,0]
        
        test case #3
        [2,0,2,1,1,0]
        */
        // Using the "Lomuto partition" for both "left->right" and "left<-right". See https://www.geeksforgeeks.org/hoares-vs-lomuto-partition-scheme-quicksort/ for some information.
        int n = nums.size();
        
        int zerosIdx = 0;
        int twosIdx = n-1;
        
        int pivotVal = 1;
        for (int i = 0; i < n;) {
            if (nums[i] < pivotVal && i > zerosIdx) {
                swap(&nums[i], &nums[zerosIdx]);
                ++zerosIdx;
                continue;
            } else if (nums[i] > pivotVal && i < twosIdx) {
                swap(&nums[i], &nums[twosIdx]);
                --twosIdx;
                continue;
            } else {
                ++i;
            }
        }
    }
};
