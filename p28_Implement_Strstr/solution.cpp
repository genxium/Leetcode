#include <stdio.h>
#include <string>
#include <vector>
using namespace std;

class Solution {
  public:
    void buildPatternKmpWidth(string& inPattern, vector<int>& outPatternPrefixWidth) {
      int patternLength = inPattern.length();
      if (0 == patternLength) {
        return;
      }
      outPatternPrefixWidth.assign(patternLength, 0);
      outPatternPrefixWidth[0] = 0;
      for (int i = 1; i < patternLength; ++i) {
        int candidatePrefixWidth = outPatternPrefixWidth[i-1];
        /*
        * A case that we need the traversal of "candidatePrefixWidth" is as follows. 
        * ```
        * pattern3    = "a a a b a a a a a a a c"
        * prefixWidth =  0 1 2 0 1 2 3 3 3 3 3 0 
        * ```
        */ 
        while (candidatePrefixWidth >= 0) {
          if (inPattern[i] == inPattern[candidatePrefixWidth]) {
            outPatternPrefixWidth[i] = candidatePrefixWidth + 1;
            break;
          } else {
            --candidatePrefixWidth; 
          }
        }
      } 
    }

    int strStr(string s, string pattern) {
      int patternLength = pattern.length();
      if (0 == patternLength) {
        return 0;
      }
      int sLen = s.length();
      if (sLen < patternLength) {
        return -1;
      }
      vector<int> patternPrefixWidth;
      buildPatternKmpWidth(pattern, patternPrefixWidth);
      
      // printf("patternPrefixWidth[]\n");
      // for (int i = 0; i < patternLength; ++i) {
      //   printf("%d ", patternPrefixWidth[i]);
      // }
      // printf("\n");
          
      int offsetInPattern = 0;
      int offsetInS = 0;
      while (offsetInS < sLen) {
        // printf("start matching aligned at s[%d] == %c, pattern[%d] == %c.\n", offsetInS + offsetInPattern, s[offsetInS + offsetInPattern], offsetInPattern, pattern[offsetInPattern]);
        while (offsetInPattern < patternLength && s[offsetInS + offsetInPattern] == pattern[offsetInPattern]) {
          // printf("s[%d] == pattern[%d], moving on\n", offsetInS + offsetInPattern, offsetInPattern);

          ++offsetInPattern;
        }
        if (offsetInPattern >= patternLength) {
          // matched
          return offsetInS;
        }

        int magicOffset;
        if (0 >= offsetInPattern) {
          magicOffset = 1;
          offsetInS += magicOffset;
          offsetInPattern = 0; // unchanged
        } else {
          magicOffset = (offsetInPattern - patternPrefixWidth[offsetInPattern - 1]);
          offsetInS += magicOffset;
          offsetInPattern = patternPrefixWidth[offsetInPattern - 1];
        }

        // printf("resetting magicOffset == %d, offsetInPattern == %d, offsetInS == %d.\n", magicOffset, offsetInPattern, offsetInS);
      }

      // not matched till the end of "s"
      return -1;
    }
};

int main() {
  string haystack = "aabaaabaaac";
  string needle = "aabaaac";

  // string haystack = "abcxxxabcxxxabcyyy";
  // string needle = "abcxxxabcyyy";

  // string haystack = "hello";
  // string needle = "ll";

  Solution s;
  int res = s.strStr(haystack, needle);
  printf("Found %s in %s at %d of the latter.\n", needle.c_str(), haystack.c_str(), res);
  return 0;
}
