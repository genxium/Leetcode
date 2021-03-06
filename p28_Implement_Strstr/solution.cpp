/*
test case#1
"aaa"
"aabab"

test case#2
"aaaaa"
"bba"

test case#3
"cuaabaababaabaaa"
"abaababaab"
*/
class Solution {
  public:
    void buildPatternKmpWidth(string& inPattern, vector<int>& outPatternPrefixWidth) {
      int patternLength = inPattern.length();
      if (0 == patternLength) {
        return;
      }

      // init
      outPatternPrefixWidth.assign(patternLength, 0);
      
      // loop
      for (int i = 1; i < patternLength; ++i) {
        int candidatePrefixWidth = outPatternPrefixWidth[i-1];
        //printf("@i:%d, trying candidatePrefixWidth == %d initially\n", i, candidatePrefixWidth);
        if (inPattern[i] == inPattern[candidatePrefixWidth]) {
          outPatternPrefixWidth[i] = candidatePrefixWidth + 1;
        } else {
          // inPattern[i] != inPattern[candidatePrefixWidth]
          
          /*
          * A case that we need the special treatment here. 
          * ```
          * pattern3    = "a a a b a a a a a a a c"
          * prefixWidth =  0 1 2 0 1 2 3 3 3 3 3 0 
          *                              ^
          * ```
          */

          /*
          * Another case that we need the special treatment here. 
          * ```
          * pattern3    = "a b a a b a b a a b"
          * prefixWidth =  0 0 1 1 2 3 2 3 4 5
          *                            ^
          * ```
          */
          while (0 < candidatePrefixWidth) {
            candidatePrefixWidth = outPatternPrefixWidth[candidatePrefixWidth-1];
            //printf("\ttrying candidatePrefixWidth == %d\n", candidatePrefixWidth);
            if (inPattern[i] == inPattern[candidatePrefixWidth]) {
              outPatternPrefixWidth[i] = (1 + candidatePrefixWidth);
              break;
            }
          }
          // or "outPatternPrefixWidth[i]" is left as "outPatternPrefixWidth[i] == 0"
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
