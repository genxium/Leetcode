from collections import defaultdict

"""
test cases
[1,2,2]
[3,2,1,2,1,7]
[0,2,2]
"""
class Solution:
    def minIncrementForUnique(self, nums: List[int]) -> int:
        counter = defaultdict(lambda: 0)
        shouldSkip = set()
        for num in sorted(nums):
            counter[num] += 1 
            shouldSkip.add(num) 
            
        deduped = sorted(shouldSkip)
        ans, targetVal = 0, min(deduped)
        for num in deduped:
            c = counter[num]
            if c == 1: 
                continue
            if targetVal < num+1:
                targetVal = num+1
            
            while c > 1:
                while targetVal in shouldSkip:
                    targetVal += 1
                
                shouldSkip.add(targetVal)
                ans += (targetVal - num)
                c -= 1
                
        return ans
