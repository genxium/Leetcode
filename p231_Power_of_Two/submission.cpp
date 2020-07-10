class Solution {
public:
    bool isPowerOfTwo(int n) {
      if (0 == n) return false;
      if (0 < n) {
        int k = (n&(n-1)); // Wipes out the lowest '1' in the binary representation of "n".
        return (0 == k);  
      } else {
        return false;
      }
    }
};
