bool const debug = false;
int const INVALID = -1;

/*
test cases
[0,1,0]
1
[1,1,0]
2
[0,0,0,1,0,1,1,0]
3
*/
class Solution {
private: 
    bool headIsOne(vector<list<int>> &posList, int zeroPosListIdx, int onePosListIdx) {
        // It's guaranteed that "posList[zeroPosListIdx].size() + posList[onePosListIdx].size() == K".
        return (
            posList[zeroPosListIdx].empty() 
                || 
            (!posList[onePosListIdx].empty() && posList[zeroPosListIdx].front() > posList[onePosListIdx].front())
        );
    }
public:
    int dfsTLE(int i, vector<int>& A, int K) {
        if (i == A.size()) return 0;
        if (A[i] == 1) return dfsTLE(i+1, A, K);
        if (debug) printf("dfs(i:%d), A[i:%d] == %d\n", i, i, A[i]);
        // By now "A[i] == 0", always flip with the head aligned to a '0'. 
        int restItems = A.size()-i;
        if (restItems < K) return INVALID;
        int nextZeroPos = i+K;
        for (int j = i; j < i+K; ++j) {
            // Greedily make "prefix continuous 1's".
            A[j] = 1-A[j];
            if (0 == A[j]) {
                nextZeroPos = min(nextZeroPos, j);
            }; 
        }
        int restSteps = dfsTLE(nextZeroPos, A, K);
        if (INVALID == restSteps) return INVALID;
        return 1 + restSteps;
    }

    int minKBitFlips(vector<int>& A, int K) {
        // return dfsTLE(0, A, K);

        int zeroPosListIdx = 0, onePosListIdx = 1;
        vector<list<int>> posList(2, list<int>{});

        // init
        int l = 0, r = 0;
        while (r < K) {
            posList[A[r]].push_back(r);
            if (r == K-1) break;
            ++r;
        }

        int flipCnt = 0;

        // loop
        while (r < A.size()) {
            // Align the head(i.e. A[l]) to a '0' element.
            while (headIsOne(posList, zeroPosListIdx, onePosListIdx) && r < A.size()) {
                ++l;
                posList[onePosListIdx].pop_front();
                ++r;
                if (r < A.size()) {
                    if (debug) {
                        printf("Pushing A[r:%d] == %d into posList.\n", r, A[r]);
                    }
                    if (A[r] == 1) posList[onePosListIdx].push_back(r);
                    else posList[zeroPosListIdx].push_back(r);
                }
            }

            if (l+K <= A.size()) {
                // flip
                if (debug) printf("Flipping from l:%d, r:%d, posList[zeroIdx:%d].size(): %d, posList[oneIdx:%d].size(): %d\n", l, r, zeroPosListIdx, posList[zeroPosListIdx].size(), onePosListIdx, posList[onePosListIdx].size());
                swap(zeroPosListIdx, onePosListIdx);
                ++flipCnt;
            } else {
                break;
            }
        }

        if (posList[zeroPosListIdx].size() > 0) return INVALID;
        return flipCnt;
    }
};
