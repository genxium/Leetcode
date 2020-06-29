class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        /*
        test case #1
        [0,0,1,1,1,1,2,3,3]
        
        test case #2
        [1,1,1,1,2,2,3]
        */
        int totalDedupLength = 0;
        int cur = 0;
        int n = nums.size();
        for (cur = 0; cur < n;) {
            int val = nums[cur], prevTotalDedupLength = totalDedupLength;
            //printf("nums[cur:%d] == %d\n", cur, val);
            nums[cur-prevTotalDedupLength] = val;
            int dupEnd = cur+1;
            int localDupLength = 0;
            while (dupEnd < n && nums[dupEnd] == val) {
                ++localDupLength;
                ++dupEnd;
                ++totalDedupLength;
            }
            //printf("\tdupEnd == %d, localDupLength == %d\n", dupEnd, localDupLength);

            if (localDupLength >= 1) {
                totalDedupLength -=1; // at most 2 duplicates are allowed
                if (cur+1 < n) {
                    nums[cur+1-prevTotalDedupLength] = val;
                }
            }
            
            // now that nums[dupEnd] != val
            cur = dupEnd;
        }
        
        int newTotalLength = (n-totalDedupLength);
        return newTotalLength;
    }
};
