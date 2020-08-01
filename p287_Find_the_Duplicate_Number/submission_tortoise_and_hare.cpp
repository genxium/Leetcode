class Solution {
public:
    int findDuplicate(vector<int>& nums) {
      // the "nums" in this problem is actually a "LinkedList" with a node that begins a cycle
      int slow = nums[0], fast = nums[0];
      do {
        slow = nums[slow];
        fast = nums[fast];
        fast = nums[fast];
      } while(slow != fast);
      
      // now that "slow" meets "fast" somewhere within the cycle
      int somewhereInCycle = fast;
      slow = nums[0];
      while (slow != fast) {
        slow = nums[slow];
        fast = nums[fast];
      }
      return slow;
    }
};
