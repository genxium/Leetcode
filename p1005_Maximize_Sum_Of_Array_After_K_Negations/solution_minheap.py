class Solution:
    def largestSumAfterKNegations(self, nums: List[int], k: int) -> int:
        heapq.heapify(nums)
        while k > 0:
            smallest = heapq.heappop(nums)
            flipped = -smallest # doesn't matter whether "smallest" is negative, positive or zero
            heapq.heappush(nums, flipped)
            k -= 1
            
        return sum(nums)
