class Solution {
public:
    int hIndex(vector<int>& citations) {
      /*
      test case #1
      [1,1]
      
      test case #2
      [100]
      */
      /*
      It's easy to see that the resultant "h" is less than or equal to "citations.size()".
      */
      int n = citations.size();
      int l = 0, r = n;
      int ans = 0;
      while (l < r) {
        int mid = ((l+r) >> 1);
        // by now there're "n-mid" elements which are >= "citations[mid]"
        int count = (n-mid);
        int hCandidate = citations[mid];
        //printf("l:%d, r:%d, mid:%d, thus\n\tcount:%d, hCandidate:%d\n", l, r, mid, count, hCandidate);
        if (count >= hCandidate) {
          if (hCandidate > ans)  ans = hCandidate;
          l = mid+1;
        } else {
          if (count > ans) ans = count;
          r = mid;
        }
      }
      return ans;
    }
};
