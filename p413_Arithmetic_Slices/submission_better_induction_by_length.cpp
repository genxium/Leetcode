class Solution {
public:
    int numberOfArithmeticSlices(vector<int>& A) {
      unordered_set<int> lengthBasedCandidates;
      
      int n = A.size(), ans = 0;
      // init 
      for (int i = 0; i < n-2; ++i) {
        int diff1 = A[i+1] - A[i];
        int diff2 = A[i+2] - A[i+1]; // i+2 < n
        if (diff1 != diff2) continue;
        lengthBasedCandidates.insert(i); // adds only the "startIdx" 
        ++ans;
      }
      
      // loop 
      int len = 3;
      while (len <= n) {
        for (auto it = lengthBasedCandidates.begin(); it != lengthBasedCandidates.end(); ) {
          int startIdx = *it;
          int startdiff = A[startIdx+1]-A[startIdx];
          int endIdxClosed = startIdx+len;
          if (endIdxClosed >= n) {
            ++it;
            continue;
          }
          int endDiff = A[endIdxClosed]-A[endIdxClosed-1]; 
          if (endDiff != startdiff) {
            it = lengthBasedCandidates.erase(it);
            continue;
          }
          ++it;
          ++ans;
        }
        ++len;
      }
      
      return ans;
    }
};
