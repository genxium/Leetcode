#define MAXN 100
#define MAX_LAST_CH_CONSECUTIVE_COUNT 10
#define MAXQUOTA 100
#define MAXCH 27
#define INVALID -1
#define INVALID_CH 'a'+26

class Solution {
private:
    int cache[MAXQUOTA][MAXN][MAX_LAST_CH_CONSECUTIVE_COUNT+1][MAXCH];
  
    int runLengthOf(int count) {
      if (count <= 1) {
        return 0;
      }
      if (count < 10) {
        return 1;
      }
      if (count < 100) {
        return 2;
      }
      return 3;
    }
public:
    int suffixRunLengthAdditionWrtRepeatingCharacterPrefix(string &s, int delQuota, int offset, int lastChConsecutiveCount, char lastCh) {
      int lastChInt = (lastCh - 'a');
      
      //printf("suffixRunLengthAdditionWrtRepeatingCharacterPrefix, {delQuota:%d, offset:%d, lastChConsecutiveCount:%d, lastCh:%c}\n", delQuota, offset, lastChConsecutiveCount, lastCh);
      if (offset == s.length()) {
        return 0;
      }
      
      if (INVALID != cache[delQuota][offset][lastChConsecutiveCount][lastChInt]) {
        /*
        The 1st trick here is the focus on {delQuota, offset, lastChConsecutiveCount, lastCh} as a state variable for the search, hence the search results are cachable.
        */
        //printf("\tcache[delQuota:%d][offset:%d][lastChConsecutiveCount:%d][lastCh:%c]:%d\n", delQuota, offset, lastChConsecutiveCount, lastCh, cache[delQuota][offset][lastChConsecutiveCount][lastChInt]);

        return cache[delQuota][offset][lastChConsecutiveCount][lastChInt];
      }
      
      /*
        The 2nd trick here is that the recurring nature allows to ALWAYS compute "suffix run-length" w.r.t. a "repeating character prefix".
      */
      int toRet = INT_MAX;
      if (lastCh == s[offset]) {
        int newLastChConsecutiveCount = min(lastChConsecutiveCount+1, MAX_LAST_CH_CONSECUTIVE_COUNT);
        int candidateAddition = ((1 == lastChConsecutiveCount || 9 == lastChConsecutiveCount) ? 1 : 0); // just became 2 or 10
        int candidate = candidateAddition + suffixRunLengthAdditionWrtRepeatingCharacterPrefix(s, delQuota, offset+1, newLastChConsecutiveCount, lastCh);  
        if (candidate < toRet) toRet = candidate;
      } else {
        int candidateAddition = 1; // just became visible
        int candidate = candidateAddition + suffixRunLengthAdditionWrtRepeatingCharacterPrefix(s, delQuota, offset+1, 1, s[offset]);  
        if (candidate < toRet) toRet = candidate;
      }
      
      if (delQuota > 0) {
        int candidate = suffixRunLengthAdditionWrtRepeatingCharacterPrefix(s, delQuota-1, offset+1, lastChConsecutiveCount, lastCh);
        if (candidate < toRet) toRet = candidate;
      }
      
      cache[delQuota][offset][lastChConsecutiveCount][lastChInt] = toRet;
      return toRet;
    }
  
    int getLengthOfOptimalCompression(string s, int k) {
      /*
      test case#1
      "aabbaa"
      2
      
      test case #2
      "bbcbbbaccc"
      1
      
      test case #3
      "aabaabbcbbbaccc"
      1
      
      test case #4
      "aabaabbcbbbaccc"
      6
      
      test case #5
      "aaaaaaaaaaa"
      0
      */
      // init
      memset(cache, INVALID, sizeof(cache));
      
      // special case, other than this case we don't need consider counting "lastChConsecutiveCount > 10"
      if (100 == s.length()) {
        bool allTheSame = false;
        char firstCh = s[0];
        for (int i = 1; i < s.length(); ++i) {
          if (s[i] != firstCh) {
            allTheSame = false;
            break;
          }
        }
        if (allTheSame) {
          return 4;
        }  
      }
      
      // search
      int ans = suffixRunLengthAdditionWrtRepeatingCharacterPrefix(s, k, 0, 0, INVALID_CH);
      return ans;
    }
};
