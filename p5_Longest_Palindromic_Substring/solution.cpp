#define MAXLEN 1000
#include <stdio.h>
#include <string>
#include <vector>
using namespace std;

class Solution {
public:
    string longestPalindrome(string s) {
      int slen = s.length();
      if (slen <= 1) return s;

      string ss = "#"; // definitely of length (2*slen + 1) and of which any “palindromic substring” must be of ODD length
      for (int i = 0; i < slen; ++i) {
        ss.push_back(s[i]);
        ss.push_back('#');
      }
      int sslen = (slen << 1) + 1;
      /*
       * By now it's fairly easy to calculate the longest palindrome of "ss" by a "nested loop" like
       * ```
       * for (int i = 0; i < sslen; ++i) {
       *   for (int radius = 0; radius < slen; ++radius) {
       *     ...
       *   }
       * }
       * ```
       * , which is quite inefficient.
       */

      /*
       * To make use of the "historically calculated palindrome lengths", here comes the first naive approach.
       * ```
       * Denote that "dp[i] = n" means that ss[i-(n-1), …, i] is the longest palindrome ended at ss[i].
       * for (i = 1; i < sslen; ++i) {
       *   int prevPalindromeLen = dp[i - 1]; // must be ODD
       *   int leftIdxOfPrevPalindrome = (i - 1) - (prevPalindromeLen - 1); 

       *   int optimalLeftIdxCandidateOfCurrentPalindrome = (leftIdxOfPrevPalindrome - 1); 
       *   if (optimalLeftIdxCandidateOfCurrentPalindrome >= 0) {
       *     char leftCandidate = ss[optimalLeftIdxCandidateOfCurrentPalindrome];
       *     if (leftCandidate == ss[i]) {
       *       dp[i] = (prevPalindromeLen + 2);    
       *     } else {
       *       // Here comes the trap.
       *       // It's possible that there exists a "u > optimalLeftIdxCandidateOfCurrentPalindrome" making "ss[u, ..., i]" a palindrome. 
       *       // 
       *       // This "sample ss"
       *       // ```
       *       //   ___________________________
       *       // #x#c#t#c#y#b#a#a#a#b#y#c#t#c#y#
       *       //                      ---------
       *       //                              ^
       *       //                              i
       *       // ```
       *       // has "dp[i-1] == 27 (i.e. the length of the upper line)", 
       *       // and is expected to have "dp[i] == 9 (i.e. the length of the underlying dash)"
       *       // by the current definition.
       *       // 
       *       // The KEY ISSUE is that there're possibly multiple palindromes ended at each "i".
       *     }
       *   } else {
       *     dp[i] = 1;
       *   }
       * }
       * ```
       *
       */
      
      // init
      vector< vector<int> > palindromeLenCount; // "palindromeLenCount[i][j] == n" means that "the j-th palindrome ended at ss[i] has length n" 
      vector<int> palindromeLenCount0; 
      palindromeLenCount0.push_back(1); // A single character itself is of course a palindrome.
      palindromeLenCount.push_back(palindromeLenCount0);

      int maxPalindromeLen = 1;
      int maxPalindromeLenAtIdx = 0;
      
      // loop
      for (int i = 1; i < sslen; ++i) {
        vector<int> currentPalindromeLenCount;
        currentPalindromeLenCount.push_back(1); // A single character itself is of course a palindrome.
        vector<int> prevPalindromeLenCount = palindromeLenCount[i - 1];
        for (int j = 0; j < prevPalindromeLenCount.size(); ++j) {
          int prevPalindromeLen = prevPalindromeLenCount[j]; // must be ODD
          int leftIdxOfPrevPalindrome = (i - 1) - (prevPalindromeLen - 1); 

          int leftIdxCandidateOfCurrentPalindrome = (leftIdxOfPrevPalindrome - 1); 
          if (leftIdxCandidateOfCurrentPalindrome < 0) continue;
          char leftCandidate = ss[leftIdxCandidateOfCurrentPalindrome];
          if (leftCandidate != ss[i]) continue;
          int newPalindromeLen = (prevPalindromeLen + 2);
          currentPalindromeLenCount.push_back(newPalindromeLen);
          if (newPalindromeLen > maxPalindromeLen) {
            maxPalindromeLen = newPalindromeLen; 
            maxPalindromeLenAtIdx = i;
          }
        }  
        palindromeLenCount.push_back(currentPalindromeLenCount);
      }  

      int stInSs = maxPalindromeLenAtIdx - (maxPalindromeLen - 1);
      string toRet = s.substr((stInSs >> 1), (maxPalindromeLen >> 1));
      return toRet;
    }
};

int main() {
  string input = "xctcybaaabyctcy";
  Solution ins;
  string output = ins.longestPalindrome(input);
  printf("%s\n", output.c_str());
  return 0;
}
