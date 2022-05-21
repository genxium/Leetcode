"""
test cases
"00110"
"010110"
"00011000"
"11011"
"""
class Solution:
    def minFlipsMonoIncr(self, s: str) -> int:
        count0 = []
        totCnt0 = 0
        for c in s:
            if c == '0': 
                count0.append(1 if not count0 else count0[-1] + 1)
                totCnt0 += 1
            else:
                count0.append(0 if not count0 else count0[-1])
            
        ans = min(
            len(s) - totCnt0, # all 0
            totCnt0 # all 1
            )
        for i in range(len(s)):
            suffixLen = len(s)-i
            suffixCnt0 = totCnt0 - count0[i]
            prefixLen = i+1
            ans = min(ans, (prefixLen-count0[i]) + suffixCnt0)
            
        return ans
        
