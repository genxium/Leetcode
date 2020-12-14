bool const debug = false;
int const INVALID = -1;
int const MAXN = 16;
int const MAXACTIVESUBSETCNT = 12870; // C(16, 8)

/*
test cases
[1,2,1,4]
2
[6,3,8,1,3,1,2,2]
4
[5,3,3,6,3,3]
3
[5,3,3,6,3,3,1,2,3,4,5,6]
4
[5,3,3,6,3,3,1,2,3,4,5,6,7,8,9,10]
4
[7,3,16,15,1,13,1,2,14,5,3,10,6,2,7,15]
8
[13,13,2,2,4,6,3,4,3,12,13,8,3]
1
*/

class DcLinkNode {
public:
  int val, index; 
  DcLinkNode *l, *r;
  DcLinkNode(int aVal, int aIndex): l(NULL), r(NULL) {
    this->val = aVal;
    this->index = aIndex;
  }
};

void cover(DcLinkNode* c) {
    if (c->r) {
        c->r->l = c->l;            
    }
    if (c->l) {
        c->l->r = c->r;            
    }
} 

void uncover(DcLinkNode* c) {
    if (c->r) {
        c->r->l = c;            
    }
    if (c->l) {
        c->l->r = c;            
    }
}

class Solution {
public:
    string bitRepOf(int state, int n) {
        string toRet = "";
        for (int i = 0; i < n; ++i) {
            if (0 == (state & (1 << i))) toRet.push_back('0');
            else toRet.push_back('1');
        }
        return toRet;
    }
    
    void genPossibleSubSets(unordered_set<int> &activeSubsetOfIndices, unordered_set<int> &activeSubsetOfVals, int l, vector<int>& nums, DcLinkNode* grandHeadCandidates, unordered_map<int, int> &subsets) {
        if (l == activeSubsetOfIndices.size()) {
            int asMin = INT_MAX, asMax = INT_MIN;
            int activeSubsetBitmask = 0;
            for (auto i : activeSubsetOfIndices) {
                asMin = min(asMin, nums[i]);
                asMax = max(asMax, nums[i]);
                activeSubsetBitmask ^= (1 << i);
            }
            subsets[activeSubsetBitmask] = asMax - asMin;
            return;
        }
        
        DcLinkNode* curCandidate = grandHeadCandidates->r;
        if (!curCandidate) {
            return;
        }
        
        while (curCandidate) {
            if (activeSubsetOfVals.find(curCandidate->val) != activeSubsetOfVals.end()) {
                curCandidate = curCandidate->r;
                continue;
            }
            activeSubsetOfIndices.insert(curCandidate->index);
            activeSubsetOfVals.insert(curCandidate->val);
            cover(curCandidate);
            
            genPossibleSubSets(activeSubsetOfIndices, activeSubsetOfVals, l, nums, grandHeadCandidates, subsets);
            
            activeSubsetOfIndices.erase(curCandidate->index); // backtrace
            activeSubsetOfVals.erase(curCandidate->val); // backtrace 
            uncover(curCandidate); // backtrace
            curCandidate = curCandidate->r;
        }
    }
    
    int solve1(int remainingCandidates, DcLinkNode* grandHeadSubsets, unordered_map<int, int> &cache, int n, int level) {
        int indent = 2*level;
        if (debug) printf("%*s@remainingCandidates:%s\n", indent, "", bitRepOf(remainingCandidates, n).c_str());
        
        if (cache.find(remainingCandidates) != cache.end()) {
            return cache[remainingCandidates];
        }
        if (0 == remainingCandidates) {
            return (cache[remainingCandidates] = 0);
        }
        
        int candidate = INVALID;
        DcLinkNode* curSubset = grandHeadSubsets->r;
        while (curSubset) {
            int subsetBitRepresentation = curSubset->val; // re-purposed ".val" as ".subsetBitRepresentation" 
            int subsetIncompatibility = curSubset->index; // re-purposed ".index" as ".subsetIncompatibility" 
            cover(curSubset);
            bool contained = ((remainingCandidates & subsetBitRepresentation) == subsetBitRepresentation); 
            int subsequenceBest = INVALID;
            if (false == contained) {
                // WOULDN'T be available to further reduced "remainingCandidates" in the deeper recursions. 
                int subsequenceBest = solve1(remainingCandidates, grandHeadSubsets, cache, n, level+1);
                if (INVALID != subsequenceBest) {
                    if (INVALID == candidate) candidate = subsequenceBest; 
                    else candidate = min(candidate, subsequenceBest);    
                }
            } else {
                int subsequenceBest = solve1(remainingCandidates^subsetBitRepresentation, grandHeadSubsets, cache, n, level+1);
                if (INVALID != subsequenceBest) {
                    if (INVALID == candidate) candidate = subsetIncompatibility+subsequenceBest; 
                    else candidate = min(candidate, subsetIncompatibility+subsequenceBest);    
                }
            }
            
            uncover(curSubset); // backtrace
            
            curSubset = curSubset->r;
        }
        return (cache[remainingCandidates] = candidate);
    }
    
    int solve2(int remainingCandidates, DcLinkNode* activeSubset, DcLinkNode* grandHeadSubsets, unordered_map<int, unordered_map<int, int>> &cache, int n, int level) {
        int indent = 2*level;
        if (debug) printf("%*s@remainingCandidates:%s, activeSubset->bitRep:%s\n", indent, "", bitRepOf(remainingCandidates, n).c_str(), bitRepOf(activeSubset->val, n).c_str());

        // re-purposed ".val" as ".subsetBitRepresentation" 
        // re-purposed ".index" as ".subsetIncompatibility" 
        if (cache.find(remainingCandidates) != cache.end() && cache[remainingCandidates].find(activeSubset->val) != cache[remainingCandidates].end()) {
            return cache[remainingCandidates][activeSubset->val];
        }
        
        bool contained = ((remainingCandidates & activeSubset->val) == activeSubset->val);
        int newRemainCandidates = contained ? remainingCandidates^(activeSubset->val) : remainingCandidates;
        if (0 == newRemainCandidates) {
            int candidate = activeSubset->index;
            if (debug) printf("%*s@remainingCandidates:%s, activeSubset->bitRep:%s, the to be cached value is %d\n", indent, "", bitRepOf(remainingCandidates, n).c_str(), bitRepOf(activeSubset->val, n).c_str(), candidate);
            return cache[remainingCandidates][activeSubset->val] = candidate;
        }
        
        int incIncompatibility = contained ? activeSubset->index : 0;
        
        cover(activeSubset); // Such that "nextActiveSubset" won't hit "activeSubset".
        int candidate = INVALID;
        DcLinkNode* nextActiveSubset = grandHeadSubsets->r;
        while (nextActiveSubset) {
            int subsequenceBest = solve2(newRemainCandidates, nextActiveSubset, grandHeadSubsets, cache, n, level+1);
            if (INVALID != subsequenceBest) {
                if (INVALID == candidate) candidate = incIncompatibility+subsequenceBest; 
                else candidate = min(candidate, incIncompatibility+subsequenceBest);    
            }
            
            nextActiveSubset = nextActiveSubset->r;
        }
        uncover(activeSubset); // backtrace    
        if (debug) printf("%*s@remainingCandidates:%s, activeSubset->bitRep:%s, the to be cached value is %d\n", indent, "", bitRepOf(remainingCandidates, n).c_str(), bitRepOf(activeSubset->val, n).c_str(), candidate);
        
        return (cache[remainingCandidates][activeSubset->val] = candidate);
    }
    
    int solve3(int remainingCandidates, DcLinkNode* grandHeadSubsets, unordered_map<int, int> &cache, int n, int level) {
        int indent = 2*level;
        if (cache.find(remainingCandidates) != cache.end()) {
            return cache[remainingCandidates];
        }
        if (0 == remainingCandidates) {
            return (cache[remainingCandidates] = 0);
        }
        
        int candidate = INVALID;
        vector<DcLinkNode*> notContainedList; notContainedList.reserve(MAXACTIVESUBSETCNT);
        DcLinkNode* curSubset = grandHeadSubsets->r;
        while (curSubset) {
            int subsetBitRepresentation = curSubset->val; // re-purposed ".val" as ".subsetBitRepresentation" 
            int subsetIncompatibility = curSubset->index; // re-purposed ".index" as ".subsetIncompatibility" 
            if ((remainingCandidates & subsetBitRepresentation) != subsetBitRepresentation) {
                // WOULDN'T be available to further reduced "remainingCandidates" in the deeper recursions. 
                cover(curSubset);
                notContainedList.push_back(curSubset);
                curSubset = curSubset->r;
                continue;
            }
            cover(curSubset);
            int subsequenceBest = solve1(remainingCandidates^subsetBitRepresentation, grandHeadSubsets, cache, n, level+1);
            if (INVALID != subsequenceBest) {
                if (INVALID == candidate) candidate = subsetIncompatibility+subsequenceBest; 
                else candidate = min(candidate, subsetIncompatibility+subsequenceBest);    
            }
            uncover(curSubset); // backtrace
            curSubset = curSubset->r;
        }
        
        for (DcLinkNode* notContained : notContainedList) {
            uncover(notContained); // backtrace
        }
        
        return (cache[remainingCandidates] = candidate);
    }
    
    int solve4(int remainingCandidates, DcLinkNode* activeSubset, DcLinkNode* grandHeadSubsets, unordered_map<int, unordered_map<int, int>> &cache, int n, int level) {
        int indent = 2*level;
        if (debug) printf("%*s@remainingCandidates:%s, activeSubset->bitRep:%s\n", indent, "", bitRepOf(remainingCandidates, n).c_str(), bitRepOf(activeSubset->val, n).c_str());

        // re-purposed ".val" as ".subsetBitRepresentation" 
        // re-purposed ".index" as ".subsetIncompatibility" 
        if (cache.find(remainingCandidates) != cache.end() && cache[remainingCandidates].find(activeSubset->val) != cache[remainingCandidates].end()) {
            return cache[remainingCandidates][activeSubset->val];
        }
        
        int newRemainCandidates = remainingCandidates^(activeSubset->val);
        if (0 == newRemainCandidates) {
            int candidate = activeSubset->index;
            if (debug) printf("%*s@remainingCandidates:%s, activeSubset->bitRep:%s, the to be cached value is %d\n", indent, "", bitRepOf(remainingCandidates, n).c_str(), bitRepOf(activeSubset->val, n).c_str(), candidate);
            return cache[remainingCandidates][activeSubset->val] = candidate;
        }
        vector<DcLinkNode*> notContainedList; notContainedList.reserve(MAXACTIVESUBSETCNT);
        
        int incIncompatibility = activeSubset->index;
        
        cover(activeSubset); // Such that "nextActiveSubset" won't hit "activeSubset".
        int candidate = INVALID;
        DcLinkNode* nextActiveSubset = grandHeadSubsets->r;
        while (nextActiveSubset) {
            bool contained = ((newRemainCandidates & nextActiveSubset->val) == nextActiveSubset->val);
            if (!contained) {
                cover(nextActiveSubset);
                notContainedList.push_back(nextActiveSubset);
            } else {
                int subsequenceBest = solve2(newRemainCandidates, nextActiveSubset, grandHeadSubsets, cache, n, level+1);
                if (INVALID != subsequenceBest) {
                    if (INVALID == candidate) candidate = incIncompatibility+subsequenceBest; 
                    else candidate = min(candidate, incIncompatibility+subsequenceBest);    
                }                
            }
            
            nextActiveSubset = nextActiveSubset->r;
        }
        uncover(activeSubset); // backtrace    
        for (DcLinkNode* notContained : notContainedList) {
            uncover(notContained); // backtrace
        }
        if (debug) printf("%*s@remainingCandidates:%s, activeSubset->bitRep:%s, the to be cached value is %d\n", indent, "", bitRepOf(remainingCandidates, n).c_str(), bitRepOf(activeSubset->val, n).c_str(), candidate);
        
        return (cache[remainingCandidates][activeSubset->val] = candidate);
    }
    
    int minimumIncompatibility(vector<int>& nums, int k) {
        int n = nums.size(), l = n/k;
        unordered_set<int> activeSubsetOfIndices, activeSubsetOfVals;
        
        // init
        DcLinkNode* grandHeadCandidates = new DcLinkNode(INVALID, INVALID);
        DcLinkNode* curCandidates = grandHeadCandidates;
        for (int i = 0; i < n; ++i) {
            int num = nums[i];
            DcLinkNode* newNode = new DcLinkNode(num, i);
            curCandidates->r = newNode;
            newNode->l = curCandidates;
            curCandidates = newNode;
        }
        
        unordered_map<int, int> subsets;
        genPossibleSubSets(activeSubsetOfIndices, activeSubsetOfVals, l, nums, grandHeadCandidates, subsets);
        
        if (debug) printf("There're %d subsets of length l:%d.\n", subsets.size(), l);
            
        DcLinkNode* grandHeadSubsets = new DcLinkNode(INVALID, INVALID); // [WARNING] Re-purposed to {subsetBitRepresentation, subsetIncompatibility} of each DcLinkNode.
        DcLinkNode* curSubset = grandHeadSubsets;
        for (auto &it : subsets) {
            DcLinkNode* newNode = new DcLinkNode(it.first, it.second);
            curSubset->r = newNode;
            newNode->l = curSubset;
            curSubset = newNode;
        }

        int remainingCandidates = (1 << n) - 1; // Initially all, this is the realtime bitmask representation of "grandHeadCandidates".

        // solve
        int level = 0;
        unordered_map<int, int> cache1;
        int ans = solve3(remainingCandidates, grandHeadSubsets, cache1, n, level);
        
        // unordered_map<int, unordered_map<int, int>> cache2;
        // int ans = INVALID;
        // curSubset = grandHeadSubsets->r;
        // while (curSubset) {
        //     int subsequentBest = solve4(remainingCandidates, curSubset, grandHeadSubsets, cache2, n, level);
        //     if (INVALID != subsequentBest) {
        //         if (INVALID == ans) ans = subsequentBest;
        //         else ans = min(ans , subsequentBest);
        //     }
        //     curSubset = curSubset->r;
        // }
        return ans;
    }
};
