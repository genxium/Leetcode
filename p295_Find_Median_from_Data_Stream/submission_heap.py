"""
test cases
["MedianFinder","addNum","addNum","findMedian","addNum","findMedian"]
[[],[1],[2],[],[3],[]]
["MedianFinder","addNum","addNum","addNum","addNum","addNum","addNum","addNum","addNum","findMedian"]
[[],[5],[5],[8],[1],[4],[7],[1],[3],[]]
"""
"""
[WARNING] 

This solution is concise, but unlike the ordered_map solution, this one is NOT GENERALIZABLE to https://github.com/genxium/Leetcode/tree/master/p480_Sliding_Window_Median.
"""
class MedianFinder:

    def __init__(self):
        self.smallerH = [] # max-heap
        self.biggerH = [] # min-heap
        self.median = None

    def addNum(self, num: int) -> None:
        if not self.median or float(num) >= self.median: 
            heappush(self.biggerH, num)
            if len(self.biggerH) - len(self.smallerH) > 1:
                t = heappop(self.biggerH) # min-heap
                heappush(self.smallerH, -t) # max-heap
        else:
            heappush(self.smallerH, -num)
            if len(self.smallerH) - len(self.biggerH) >= 1:
                t = -heappop(self.smallerH) 
                heappush(self.biggerH, t)

        if len(self.biggerH) == len(self.smallerH):
            t1 = self.biggerH[0]
            t2 = -self.smallerH[0]
            self.median = (t1+t2)/2
        else:
            self.median = self.biggerH[0]

    def findMedian(self) -> float:
        return self.median
        



# Your MedianFinder object will be instantiated and called as such:
# obj = MedianFinder()
# obj.addNum(num)
# param_2 = obj.findMedian()
