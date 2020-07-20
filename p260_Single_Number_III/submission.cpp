class Solution {
public:
    vector<int> singleNumber(vector<int>& nums) {
      int n = nums.size();
      if (0 == n) return {};
      
      int xorRes = 0;
      for (int i = 0; i < n; ++i) {
        xorRes ^= nums[i];
      }
      
      // now that xorRes contains (a^b)
      
      // find the "LowestSignificantBit" of "xorRes", it's actually OK to find any bit "1" in "xorRes", and move on to the following steps
      int chosenShift = 1;
      for (int lsb = 0; lsb < 32; ++lsb) {
        if ((xorRes & chosenShift) > 0) break;
        chosenShift <<= 1;
      }

      /* 
      now that all elements in "nums[]" are deemed one type or another
      - those having the same chosen bit "1" as "xorRes", i.e. "chosenShift"
      - those not having the same chosen bit "1" as "xorRes"
      */
      
      // get "a" and "b"
      int a = 0, b = 0;
      for (int i = 0; i < n; ++i) {
        if ((nums[i] & chosenShift) > 0) {
          a ^= nums[i];
        } else {
          b ^= nums[i];
        }
      }
      
      return {a, b};
    }
};
