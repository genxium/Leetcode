class Pair {
public:
    int st;
    int ed;
    Pair(int aSt, int aEd) {
        this->st = aSt;
        this->ed = aEd;
    }
};

class PairCompare {
public:
    bool operator() (Pair const &a, Pair const &b) const {
        if (a.st != b.st) {
            return ( a.st < b.st );
        } else {
            return ( a.ed < b.ed );
        }
    }
};

bool closeOrExtendCurrentEnclosure(vector<vector<int>> &out, Pair* pCurrentEnclosure, Pair* pCandidate) {
    if (NULL == pCandidate) {
        //printf("closeOrExtendCurrentEnclosure #1, closing \n");
        //printf("\tcurrentEnclosure (%d, %d) by pCandidate == NULL\n", pCurrentEnclosure->st, pCurrentEnclosure->ed);
        out.push_back(vector<int>{pCurrentEnclosure->st, pCurrentEnclosure->ed});
        return true; // For the caller to set "pCurrentEnclosure = NULL" on its own stack.
    } else {
        //printf("closeOrExtendCurrentEnclosure #2, \n");
        //printf("\tby candidate (%d, %d)\n", pCandidate->st, pCandidate->ed);
        if (pCandidate->st > pCurrentEnclosure->ed) {
            //printf("\tclosing currentEnclosure (%d, %d)\n", pCurrentEnclosure->st, pCurrentEnclosure->ed);
            out.push_back(vector<int>{pCurrentEnclosure->st, pCurrentEnclosure->ed});
            return true; // For the caller to set "pCurrentEnclosure = NULL" on its own stack.
        } else {
            if (pCandidate->ed > pCurrentEnclosure->ed) {
                // Extends the right edge of (*pCurrentEnclosure).
                //printf("\textending currentEnclosure (%d, %d)\n", pCurrentEnclosure->st, pCurrentEnclosure->ed);
                pCurrentEnclosure->ed = pCandidate->ed; 
            }
        }
    }
    return false;
}

class Solution {
public:
    vector<vector<int>> insert(vector<vector<int>>& intervals, vector<int>& newInterval) {
        /*
        test case #1
        [[1,3],[6,9]]
        [2,5]
        
        test case #2
        [[1,3],[6,9]]
        [1,3]
        
        test case #3
        [[1,3],[6,9]]
        [1,9]
        
        test case #4
        [[1,3],[6,9]]
        [1,6]
        
        test case #5
        []
        [1,6]
        
        test case #6
        []
        []
        */
        vector<vector<int>> ans;
        int nIntervals = intervals.size();
        
        if (0 >= nIntervals) {
            ans.push_back(newInterval);
            return ans;
        }
  
        vector<Pair> arrOfPairs;
        Pair p(newInterval[0], newInterval[1]);
        arrOfPairs.push_back(p);
        for (int i = 0; i < nIntervals; ++i) {
            Pair ap(intervals[i][0], intervals[i][1]);
            arrOfPairs.push_back(ap); 
        }
        sort(arrOfPairs.begin(), arrOfPairs.end(), PairCompare());

        Pair currentEnclosure(0, 0);
        Pair *pCurrentEnclosure  = NULL;
        
        for (int i = 0; i < arrOfPairs.size(); ++i) {
            Pair singlePair = arrOfPairs[i];
            printf("arrOfPairs[i:%d] == [%d, %d]\n", i, singlePair.st, singlePair.ed);
            if (NULL != pCurrentEnclosure) {
                bool closed = closeOrExtendCurrentEnclosure(ans, pCurrentEnclosure, &singlePair);
                if (closed) {
                    pCurrentEnclosure = NULL;
                }
            }

            if (NULL == pCurrentEnclosure) {
                currentEnclosure.st = singlePair.st;
                currentEnclosure.ed = singlePair.ed;
                pCurrentEnclosure = &currentEnclosure;
            }
        }
        
        if (NULL != pCurrentEnclosure) {
            bool closed = closeOrExtendCurrentEnclosure(ans, pCurrentEnclosure, NULL);
            if (closed) {
                pCurrentEnclosure = NULL;
            }
        }

        return ans;
    }
};
