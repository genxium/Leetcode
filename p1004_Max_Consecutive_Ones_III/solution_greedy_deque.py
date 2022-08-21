debug = False

class Solution:
    def longestOnes(self, nums: List[int], k: int) -> int:
        n = len(nums)
        if k >= n: return n
        
        usingFlips = deque() # positions where 0s are flipped by far
        l, r = 0, 0 # positions containing consecutive 1s by far
        
        ans = 0
        for r in range(n):
            if 0 == nums[r]:
                usingFlips.append(r) # just flip nums[r]
                if len(usingFlips) > k: 
                    # need pop from front 
                    f = usingFlips.popleft()
                    l = f+1 # update the left accordingly
            if debug: print(f'r={r}, l={l}, usingFlips={usingFlips}')
            ans = max(ans, r-l+1)
            
        return ans
        
