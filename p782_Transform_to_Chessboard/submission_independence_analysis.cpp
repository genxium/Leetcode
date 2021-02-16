bool debug = false;

/*
test cases
[[0,1,1,0],[0,1,1,0],[1,0,0,1],[1,0,0,1]]
[[0, 1], [1, 0]]
[[1, 0], [1, 0]]
[[0,1,0],[1,0,1],[0,1,0]]
[[0,1,0],[1,0,1],[1,0,1]]
[[1,1,0],[0,0,1],[0,0,1]]
*/
class Solution {
public:
    int movesToChessboard(vector<vector<int>>& board) {
        /*
        [Lemma#1] ActionSequences "swapRows(ri, rj) -> swapCols(ci, cj)" and "swapRows(ci, cj) -> swapCols(ri, rj)" have the SAME result on a "01-board". Draw the 4 intersections (ri, ci), (ri, cj), (rj, ci), (rj, cj) and follow their moves to see why. Moreover, an ActionSequence of "swapRows" and "swapCols" can arbitrarily permute the actions to get the same result on a "01-board".
        
        [Lemma#2] If transformed from a ChessBoard, there'd be EXACTLY 2 types of rows in the input.
        */
        int N = board.size();
        int valsForLeadingDigits[2];
        valsForLeadingDigits[0] = 0; // a row or col "0,1,0,1,0,1,..."
        for (int i = 1; i < N; ++i) {
            valsForLeadingDigits[0] *= 2;
            valsForLeadingDigits[0] += (i & 1) ? 1 : 0;
        }
        
        valsForLeadingDigits[1] = 1; // a row or col "1,0,1,0,1,0,..."
        for (int i = 1; i < N; ++i) {
            valsForLeadingDigits[1] *= 2;
            valsForLeadingDigits[1] += (i & 1) ? 0 : 1;
        }
        
        map<int, int> rowRepVals; // row representatives
        for (int y = 0; y < N; ++y) {
            int rowVal = 0;
            vector<int> row;
            for (int x = 0; x < N; ++x) {
                rowVal *= 2;
                rowVal += board[y][x];
            }
            if (rowRepVals.size() == 2 && !rowRepVals.count(rowVal)) return -1;
            ++rowRepVals[rowVal];
        }
        
        map<int, int> colRepVals; // col representatives
        for (int x = 0; x < N; ++x) {
            int colVal = 0;
            vector<int> col;
            for (int y = 0; y < N; ++y) {
                colVal *= 2;
                colVal += board[y][x];
            }
            if (colRepVals.size() == 2 && !colRepVals.count(colVal)) return -1;
            ++colRepVals[colVal];
        }
        
        if (colRepVals.size() != 2 || rowRepVals.size() != 2) return -1;
        
        vector<int> rowRepValsList = {rowRepVals.begin()->first, rowRepVals.rbegin()->first};
        vector<int> colRepValsList = {colRepVals.begin()->first, colRepVals.rbegin()->first};
        
        int ans = INT_MAX;
        // Note that to transform a "01-row" into another "01-row" with the same 0's count & 1's count by "swapping any pair of digits", we just have to count how many of the digits are different and take the half.

        if (N%2 == 0) {
            int halfN = N/2;
            if (colRepVals.begin()->second != colRepVals.rbegin()->second) return -1;
            if (rowRepVals.begin()->second != rowRepVals.rbegin()->second) return -1;
            if (__builtin_popcount(colRepValsList[0]) != halfN || __builtin_popcount(colRepValsList[1]) != halfN) return -1;
            if (__builtin_popcount(rowRepValsList[0]) != halfN || __builtin_popcount(rowRepValsList[1]) != halfN) return -1;
            
            // Traverse the matching options, 
            for (int a = 0; a < 2; ++a) {
                for (int b = 0; b < 2; ++b) {
                    int rowCost = (__builtin_popcount(rowRepValsList[0]^valsForLeadingDigits[a]))/2; 
                    int colCost = (__builtin_popcount(colRepValsList[0]^valsForLeadingDigits[b]))/2; 
                    ans = min(ans, rowCost+colCost);
                }
            }
        } else {
            int majorityEle = -1;
            int majorityEleTotCount = (N*N+1)/2;
            int minorityEleTotCount = (N*N-1)/2;
            int oneTotCount = 0;
            for (auto &it : rowRepVals) oneTotCount += it.second*__builtin_popcount(it.first);
            
            if (oneTotCount == majorityEleTotCount) {
                majorityEle = 1;
            } else if (oneTotCount == minorityEleTotCount) {
                majorityEle = 0;
            } else {
                return -1;
            }
                        
            int localMajorityCount = (N+1)/2, localMinorityCount = (N-1)/2;
            int rowCost = INT_MAX, colCost = INT_MAX;
            if (__builtin_popcount(rowRepValsList[0]) == (1 == majorityEle ? localMajorityCount : localMinorityCount)) {
                if (debug) printf("rowRepVal:%d holds localMajorityCount:%d of the majorityEle:%d\n", rowRepValsList[0], localMajorityCount, majorityEle);
                rowCost = (__builtin_popcount(rowRepValsList[0]^valsForLeadingDigits[majorityEle]))/2;
            } else {
                if (debug) printf("rowRepVal:%d holds localMajorityCount:%d of the majorityEle:%d\n", rowRepValsList[1], localMajorityCount, majorityEle);
                rowCost = (__builtin_popcount(rowRepValsList[1]^valsForLeadingDigits[majorityEle]))/2;
            }

            if (__builtin_popcount(colRepValsList[0]) == (1 == majorityEle ? localMajorityCount : localMinorityCount)) {
                if (debug) printf("colRepVal:%d holds localMajorityCount:%d of the majorityEle:%d\n", colRepValsList[0], localMajorityCount, majorityEle);
                colCost = (__builtin_popcount(colRepValsList[0]^valsForLeadingDigits[majorityEle]))/2;
            } else {
                if (debug) printf("colRepVal:%d holds localMajorityCount:%d of the majorityEle:%d\n", colRepValsList[1], localMajorityCount, majorityEle);
                colCost = (__builtin_popcount(colRepValsList[1]^valsForLeadingDigits[majorityEle]))/2;
            }
            
            if (debug) printf("N:%d is even, majorityEle is %d, rowCost:%d, colCost:%d\n", N, majorityEle, rowCost, colCost);
            
            ans = min(ans, rowCost+colCost);
        }
        
        return (INT_MAX == ans ? -1 : ans);
    }
};
