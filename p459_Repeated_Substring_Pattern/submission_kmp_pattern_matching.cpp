/*
test case#1
"aabaaaabaaaabaaaabaaaabaaaabaaaabaaaabaaaabaaaabaa"

test case#2
"aabaaaabaaaabaaaabaaaabaaaabaaaabaaaabaaaabaaaabaac"

test case#3
"aabaaaabaaaabaaaabaaaabaaaabaaaabaaaabaaaabaaaabaaa"

test case#4
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
    
    bool repeatedSubstringPattern(string pattern) {
      int patternLength = pattern.length();
      vector<int> patternPrefixWidth(patternLength, 0);
      buildPatternKmpWidth(pattern, patternPrefixWidth);
      // printf("patternPrefixWidth[]\n");
      // for (int i = 0; i < patternLength; ++i) {
      //   printf("%d ", patternPrefixWidth[i]);
      // }
      // printf("\n");
      if (patternPrefixWidth.back() == 0) return false;
      int candidatePrefixLength = patternLength - patternPrefixWidth.back();
      return (patternPrefixWidth.back()%candidatePrefixLength == 0
             &&
             patternLength%candidatePrefixLength == 0);
    }
};
