typedef uint64_t ULL;

/*
test cases
12
21
1771891
177189133
1999999999
2147483647
*/
class Solution {
public:
    int nextGreaterElement(int n) {
      if (0 == n) {
        return -1;
      }
      int origN = n;
      vector<int> holder;
      while (n > 0) {
        holder.push_back(n%10);
        n /= 10;
      }
      reverse(holder.begin(), holder.end());
      next_permutation(holder.begin(), holder.end());
      ULL val = 0, tens = 1;
      for (int i = holder.size()-1; i >= 0; --i) {
        val += tens*holder[i];
        tens *= 10;
      }
      if (val > (ULL)(UINT_MAX)) return -1;
      int ans = (int)val;
      if (ans <= origN) return -1; 
      return ans;
    }
};
