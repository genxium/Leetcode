debug = False

"""
["TopVotedCandidate","q","q","q","q","q","q"]
[[[0,1,1,0,0,1,0],[0,5,10,15,20,25,30]],[3],[12],[25],[15],[24],[8]]
["TopVotedCandidate","q","q","q","q","q","q","q","q","q","q"]
[[[0,0,0,0,1],[0,6,39,52,75]],[45],[49],[59],[68],[42],[37],[99],[26],[78],[43]]
"""
class TopVotedCandidate:
    def __init__(self, persons: List[int], times: List[int]):
        self.leadings = []
        self.votes = defaultdict(lambda: 0) # latest votes
        self.votesH = [] # the heap for historical votes
        for i in range(len(persons)):
            self.votes[persons[i]] += 1
            # update votesH and leadings
            heappush(self.votesH, (-self.votes[persons[i]], -times[i], persons[i])) # a min-heap by default, make it a max-heap by the first 2 elements (reverseVote, reverseTime) where the "reverseTime" part is for tie-resolving
            foundLeading = False
            (rv, rt, p) = (None, None, None)
            while not foundLeading:
                (rv, rt, p) = heappop(self.votesH)
                if -rv != self.votes[p]:
                    continue
                heappush(self.votesH, (rv, rt, p)) # put-back
                foundLeading = True
                break
                
            if debug: print('At time:%d, person %d is leading for having %d votes' % (times[i], p, -rv))
            self.leadings.append((-rt, p, -rv))
            

    def q(self, t: int) -> int:
        i = bisect.bisect_left(self.leadings, t, key=lambda ele: ele[0])
        if debug: print('At time:%d, index %d is leading' % (t, i))
        if i == len(self.leadings):
            return self.leadings[-1][1] 
        if self.leadings[i][0] == t:
            return self.leadings[i][1]
        if i == 0:
            return None
        return self.leadings[i-1][1]


# Your TopVotedCandidate object will be instantiated and called as such:
# obj = TopVotedCandidate(persons, times)
# param_1 = obj.q(t)
