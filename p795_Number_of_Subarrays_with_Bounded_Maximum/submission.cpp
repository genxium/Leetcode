/*
test cases
[2,1,4,3]
2
3
[2,9,2,5,6]
2
8
[2, 1, 1, 2, 2]
2
3
*/
class Solution {    
public:
    int numSubarrayBoundedMax(vector<int>& A, int L, int R) {
        /*
        The intuition is that for each "A[i]", if it is the last element in a "valid subarray", then "A[i] <= R" must hold.

        - case L <= A[i] <= R
          Walk backwards to find the first "j < i && A[j] > R", now every "A[u >= j, i]" is a valid subarray.

        - case A[i] < L
          Walk backwards to find the first "t < i && L <= A[t] <= R", then continue walking back to find the first "j < i && A[j] > R", now every "A[u >= j, i]" is a valid subarray if "t" exists.
        */
        int ans = 0;
        int lessThanLCnt = 0, lessThanOrEqualToRCnt = 0;
        for (int i = 0; i < A.size(); ++i) {
            /*
            Consider 
            ```
            [2, 1, 1, 2, 2]
            2
            3
            ```
            */
            if (A[i] < L) ++lessThanLCnt;
            else lessThanLCnt = 0;
            
            if (A[i] <= R) ++lessThanOrEqualToRCnt;
            else lessThanOrEqualToRCnt = 0;
            
            /*
            By now
            - "A[i-lessThanLCnt+1, i]" are all "< L", and
            - "A[i-lessThanOrEqualToRCnt+1, i]" are all "<= R"
            , thus if (lessThanOrEqualToRCnt > lessThanLCnt), denote "u = i-lessThanOrEqualToRCnt+1, v = i-lessThanLCnt", then every subarray "A[t >= u, v]" is a "valid & new subarray" to count.
            */
            ans += (lessThanOrEqualToRCnt - lessThanLCnt);
        }
            
        return ans;
    }
};
