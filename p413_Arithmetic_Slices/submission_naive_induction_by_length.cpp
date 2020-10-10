class Solution {
public:
    int numberOfArithmeticSlices(vector<int>& A) {
      int CUR = 0, NEXT = 1;
      set<int> lengthBasedCandidates[2];
      
      int n = A.size(), ans = 0;
      // init 
      for (int i = 0; i < n-2; ++i) {
        int diff1 = A[i+1] - A[i];
        int diff2 = A[i+2] - A[i+1]; // i+2 < n
        if (diff1 != diff2) continue;
        lengthBasedCandidates[CUR].insert(i); // adds only the "startIdx" 
        ++ans;
      }
      
      // loop 
      int len = 3;
      while (len <= n) {
        lengthBasedCandidates[NEXT].clear();
        for (auto startIdx : lengthBasedCandidates[CUR]) {
          int startdiff = A[startIdx+1]-A[startIdx];
          int endIdxClosed = startIdx+len;
          if (endIdxClosed >= n) continue;
          int endDiff = A[endIdxClosed]-A[endIdxClosed-1]; 
          if (endDiff != startdiff) continue;
          lengthBasedCandidates[NEXT].insert(startIdx); // adds only the "startIdx" 
          ++ans;
        }
        swap(CUR, NEXT);
        ++len;
      }
      
      return ans;
    }
};
