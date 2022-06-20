"""
test cases
[4,6,15,35]
[20,50,9,63]
[2,3,6,7,4,12,21,39]
[2,3,6,7,4,12,21,39,100000]
"""
from collections import defaultdict
from bisect import bisect_right

debug = False
class Solution:
    def dfs(self, x, adjBuckets, bucketNums, memo, pseen, lv):
        if x in memo:
            return 0
        if debug:
            prefix = ' '*(lv << 1)
            print(f'{prefix}checking x: {x}, adjBuckets[{x}]: {adjBuckets[x]}')
        cnt = 1
        memo[x] = True
        for p in adjBuckets[x]:
            if p in pseen:
                continue
            pseen[p] = True # no need to check in the future, all nums having primefac "p" will be visited below, an even faster way is to maintain a linked-list by "cover/uncover ops" like that of DancingLink https://www.yinxiang.com/everhub/note/e71e7e87-ca06-40e5-a880-c9517a9afd33
            for i in range(0, len(bucketNums[p])):
                nb = bucketNums[p][i]
                cnt += self.dfs(nb, adjBuckets, bucketNums, memo, pseen, lv+1)

        return cnt

    def largestComponentSize(self, nums: List[int]) -> int:
        numsSet = set(nums)
        mx = max(nums)
        adjBuckets = defaultdict(lambda: set())
        bucketNums = defaultdict(lambda: list())
        isprime = [True for i in range(mx+1)]
        cntPrimes = 0
        for p in range(2, mx+1):
            if not isprime[p]:
                continue
            j = 1
            cntPrimes += 1
            while j*p <= mx:
                v = j*p
                if v in numsSet:
                    adjBuckets[v].add(p)
                    bucketNums[p].append(v)
                isprime[v] = False
                j += 1

        for p in bucketNums.keys():
            bucketNums[p] = sorted(set(bucketNums[p]))

        if debug: 
            print(f'adj inited, there\'re {len(bucketNums)} primes in range')

        ans = 1
        memo = dict()
        pseen = dict()
        for x in sorted(nums):
            ans = max(ans, self.dfs(x, adjBuckets, bucketNums, memo, pseen, 0))
        return ans
