bool const debug = false;
int const MAXN = 2000;
int const INVALID = -1;
int cache[MAXN][4]; // -1: unknown, 0: impossible, 1: possible

class Solution {
public:
    void evalPalindromeLenCount(string &s, vector< vector<int> > &palindromeLenCount) {
      int slen = s.length();

      // init
      palindromeLenCount.push_back({1}); // A single character itself is of course a palindrome.

      if (slen <= 1) return;

      // loop
      for (int i = 1; i < slen; ++i) {
        vector<int> currentPalindromeLenCount = {1}; // A single character itself is of course a palindrome.
        vector<int> prevPalindromeLenCount = palindromeLenCount[i - 1];
        for (int j = 0; j < prevPalindromeLenCount.size(); ++j) {
          int prevPalindromeLen = prevPalindromeLenCount[j]; // could be ODD or EVEN
          int leftIdxCandidateOfCurrentPalindrome = -1, newPalindromeLen = 0;
          do {
            if (0 < (prevPalindromeLen & 1)) {
              // ODD
              if (0 == j) {
                if (1 < prevPalindromeLen) {
                  // A "repeating character palindrome", otherwise "prevPalindromeLenCount[0]" should be 1.
                  if (s[i] == s[i-1]) {
                    currentPalindromeLenCount[0] = (prevPalindromeLen+1);
                    leftIdxCandidateOfCurrentPalindrome = i-currentPalindromeLenCount[0]+1;
                    newPalindromeLen = currentPalindromeLenCount[0];
                    break;             
                  }
                } else {
                  // 1 == prevPalindromeLen
                  if (s[i] == s[i-1]) {
                    currentPalindromeLenCount[0] = (prevPalindromeLen+1);
                    leftIdxCandidateOfCurrentPalindrome = i-currentPalindromeLenCount[0]+1;
                    newPalindromeLen = currentPalindromeLenCount[0];
                    break;                  
                  }
                }
              }
            } else {
              // EVEN
              if (0 == j) {
                // A "repeating character palindrome", otherwise "prevPalindromeLenCount[0]" should be 1 which is ODD.
                if (s[i] == s[i-1]) {
                  /* 
                     Joins the "repeating character palindrome", it's IMPOSSIBLE that 
                     ```
                     s[(i-1)-(prevPalindromeLen - 1)-1] == s[i]
                     ```
                     , otherwise "prevPalindromeLen" should be ODD instead and bigger.
                   */
                  currentPalindromeLenCount[0] = (prevPalindromeLen+1);
                  leftIdxCandidateOfCurrentPalindrome = i-currentPalindromeLenCount[0]+1;
                  newPalindromeLen = currentPalindromeLenCount[0];
                  break;
                }
              }
            }

            int leftIdxOfPrevPalindrome = (i - 1) - (prevPalindromeLen - 1); 
            leftIdxCandidateOfCurrentPalindrome = (leftIdxOfPrevPalindrome - 1); 
            if (0 > leftIdxCandidateOfCurrentPalindrome) break;
            char leftCandidate = s[leftIdxCandidateOfCurrentPalindrome];
            if (leftCandidate != s[i]) break;
            newPalindromeLen = (prevPalindromeLen + 2);
            currentPalindromeLenCount.push_back(newPalindromeLen); 
          } while (false);

          if (0 == newPalindromeLen) {
            continue;
          }
        }  
        palindromeLenCount.push_back(currentPalindromeLenCount);
      }
    }

    int dfs(int i, int c, vector< vector<int> > &palindromeLenCount, int level) {
        int indent = 2*level;
        // Can we partition s[0, ..., i] to c palindromic substrings?
        if (debug) {
            printf("%*si:%d, c:%d\n", indent, "", i, c);   
        }
        
        if (i == -1) {
            return (c == 0 ? true : false);
        }
        
        if (c == 0) {
            if (i == -1) return true;
            return cache[i][c] = false;
        }
        
        if (INVALID != cache[i][c]) return cache[i][c];
        
        int result = false;
        for (int x = 1; x <= palindromeLenCount[i][0]; ++x) {
            int t = i-x+1; // s[t, i] is a palindrome
            int cand = dfs(t-1, c-1, palindromeLenCount, level+1);
            if (cand) {
                result = true;
                break;
            }
        }
        
        if (false == result) {
            for (int j = 1; j < palindromeLenCount[i].size(); ++j) {
                int x = palindromeLenCount[i][j];
                int t = i-x+1; // s[t, i] is a palindrome
                int cand = dfs(t-1, c-1, palindromeLenCount, level+1);
                if (cand) {
                    result = true;
                    break;
                }
            }         
        }

        return cache[i][c] = result;
    }
    
    bool checkPartitioning(string s) {
        vector< vector<int> > palindromeLenCount; // "palindromeLenCount[i][j] == x" means that "the j-th palindrome ended at s[i] has length x" 
        evalPalindromeLenCount(s, palindromeLenCount);
        memset(cache, INVALID, sizeof cache);
        
        return dfs(s.length()-1, 3, palindromeLenCount, 0);
    }
};
