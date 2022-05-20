"""
["amazon","apple","facebook","google","leetcode"]
["e","o"]
["amazon","apple","facebook","google","leetcode"]
["l","e"]
["amazon","apple","facebook","google","leetcode"]
["lo","eo"]
"""
class Solution:
    def wordSubsets(self, words1: List[str], words2: List[str]) -> List[str]:
        universalDict = defaultdict(lambda: 0)
        for s in words2:
            localDict = defaultdict(lambda: 0)
            for i in range(len(s)):
                localDict[s[i]] += 1
            for i in range(len(s)):
                universalDict[s[i]] = max(universalDict[s[i]], localDict[s[i]])
        
        ans = []
        for s in words1:
            localDict = defaultdict(lambda: 0)
            for i in range(len(s)):
                localDict[s[i]] += 1
            contains = True
            for k,v in universalDict.items():
                if k not in localDict or localDict[k] < v:
                    contains = False
                    break
            if contains:
                ans.append(s)
                
        return ans
