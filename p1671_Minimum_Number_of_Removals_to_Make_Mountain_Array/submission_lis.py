"""
test cases
[1,3,1]
[2,1,1,5,6,2,3,1]
[9,8,1,7,6,5,4,3,2,1]
"""
debug = False
class Solution:
    def minimumMountainRemovals(self, nums: List[int]) -> int:
        """
        [WARNING]

        This is NOT the traditional LIS solution, see https://github.com/genxium/Leetcode/tree/master/p354_Russian_Doll_Envelopes for more information
        """
        # TODO: any speed-up approach?
        n = len(nums)
        dp, rdp = [None]*n, [None]*n # dp[i] = length of LIS(LongestIncreasingSubsequence) endint EXACTLY at nums[i], and rdp the reverse(but decreasing)
        for i in range(n):
            if i == 0: 
                dp[i] = rdp[n-1-i] = 1
            else:
                dp[i] = max([dp[j]+1 if nums[j] < nums[i] else 1 for j in range(i)])
                rdp[n-1-i] = max([rdp[n-1-j]+1 if nums[n-1-i] > nums[n-1-j] else 1 for j in range(i)])
            
        ans = sys.maxsize
        for i in range(n):
            if dp[i] < 2 or rdp[i] < 2: continue
            effLen = dp[i] + rdp[i] - 1
            ans = min(ans, n-effLen)
            if debug: 
                print('at nums[i:%d]:%d, dp[i:%d]:%d, rdp[i:%d]:%d, thus ans updated to %d' % (i, nums[i], i, dp[i], i, rdp[i], ans))

        return ans
