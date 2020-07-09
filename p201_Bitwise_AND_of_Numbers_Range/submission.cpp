class Solution {
public:
    int rangeBitwiseAnd(int m, int n) {
      /*
      test case #1
      5
      7
      
      test case #2
      5
      8
      
      test case #3
      4
      16
      
      test case #4
      2147483646
      2147483647
      */
      int differentCount = 0;
      while (m != n) {
        m >>= 1;
        n >>= 1;
        ++differentCount;
      }
      return (m << differentCount);
    }
};
