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
      
      // find the "LowestSignificantBit" of "xorRes"
      int pow2 = 1;
      for (int lsb = 0; lsb < 32; ++lsb) {
        if ((xorRes & pow2) > 0) break;
        pow2 <<= 1;
      }
      
      // get "a" and "b"
      int a = 0, b = 0;
      for (int i = 0; i < n; ++i) {
        if ((nums[i] & pow2) > 0) {
          a ^= nums[i];
        } else {
          b ^= nums[i];
        }
      }
      
      return {a, b};
    }
};
