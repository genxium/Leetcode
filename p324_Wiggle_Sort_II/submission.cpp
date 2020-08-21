class Solution {
public:
    bool isOdd(int i) {
        return ((i & 1) > 0);    
    }
    
    void wiggleSort(vector<int>& nums) {
        /*
        Median finding, or in general "selectKthSmallest(int k, vector<int> &nums)" can be done in O(n) time-complexity for the worst cases, see https://www.bowdoin.edu/~ltoma/teaching/cs231/fall09/Lectures/7-selection/selection.pdf for more information.
        */
        int n = nums.size();
        int halfSize = (n >> 1);
        nth_element(nums.begin(), nums.begin()+halfSize, nums.end());
        int median = nums[halfSize];
        
        int oddSlot = 1, reverseEvenSlot = (isOdd(n-1)) ? (n-2) : (n-1);
        for (int i = 0; i < n;) {
            if (nums[i] > median) {
                if (isOdd(i) && i < oddSlot) {
                    ++i;
                } else {
                    swap(nums[i], nums[oddSlot]);
                    oddSlot += 2;
                }
            } else if (nums[i] < median) {
                if (!isOdd(i) && i > reverseEvenSlot) {
                    ++i;
                } else {
                    swap(nums[i], nums[reverseEvenSlot]);
                    reverseEvenSlot -= 2;
                }
            } else {
                ++i;
                // nums[i] == median, deliberately left blank
            } 
        }
    }
};
