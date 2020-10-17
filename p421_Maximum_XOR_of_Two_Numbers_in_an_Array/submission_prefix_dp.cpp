class Solution {
public:
    int findMaximumXOR(vector<int>& nums) {
        /*
        We made use of a simple property of XOP as follows.

        - If "x == (a ^ b)" then "a == (x ^ b)"
        - If "x == (a ^ b) && x == (c ^ d)", then "a == (x ^ b) && c == (x ^ d)" are still valid even if "a != c && a != d"

        Another idea here is similar to exploiting "OptimalSubstructure". 
        Consider 4 numbers "a, b, c, d" of "bit-width == 8", if we know that the maximum XOR result of the first 7 bits is "p", then to obtain the final result we can try to assume in a greedy manner that "q = concat(p, 1)" is a candidate, then check for each "t in [a, b, c, d]" to see whether "(q ^ t)" also exists in [a, b, c, d].
        */

        int prefixMask = 0, ans = 0;
        for (int s = 31; s >= 0; --s) {
          prefixMask |= (1 << s);
          unordered_set<int> prefixes;
          for (auto num : nums) {
            int prefix = (num & prefixMask);
            prefixes.insert(prefix);
          }
          int candidateAns = (ans | (1 << s));
          for (auto prefix : prefixes) {
            if (prefixes.find(candidateAns ^ prefix) == prefixes.end()) continue;
            ans = candidateAns;
            break;
          }
        }

        return ans;
    }
};
