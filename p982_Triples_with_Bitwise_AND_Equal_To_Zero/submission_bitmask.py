debug = False
class Solution:
    def countTriplets(self, nums: List[int]) -> int:
        zs = defaultdict(int) # (zs[i] & (1 << k)) > 0 means that nums[k] has a 0 at the i-th position  
        for k,u in enumerate(nums):
            for i in range(16):
                if (u & (1 << i)) == 0:
                    zs[i] |= (1 << k) # python3 int is automatically bigint 
                    
        if debug:
            for i in range(16):
                print(f'zs[{i:02d}] = {zs[i]:016b}')
                
        ans = 0
        for v in nums:
            for u in nums:
                x = (u&v)
                satisfyingNums = (1 << len(nums)) - 1
                for i in range(16):
                    if (x & (1 << i)) > 0:
                        satisfyingNums &= zs[i]    
                if debug:
                    print(f'for x = {x:016b}, satisfyingNums are {satisfyingNums:016b}')
                ans += satisfyingNums.bit_count()
        
        return ans
