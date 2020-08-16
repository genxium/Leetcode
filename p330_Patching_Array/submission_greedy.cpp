#define ULL uint64_t

class Solution {
public:
    int minPatches(vector<int>& nums, int n) {
        /*
        Assume that by "nums[]" it's able to form [1, ..., x). It can be proved that at this point we should just patch "nums[]" with "x", thus extending the coverage to [1, ..., 2*x).
        
        Yet how to find "x" of a given "nums[]"?
        
        For "x == 1", it's obvious that "nums[0]" should be "1", otherwise we patch it with "1".
        For "x == 2 & 3", it's obvious that "nums[1]" should be "2", otherwise we patch it with "2".
        
        Consider that for a given "nums[] = [1, 2, ...]" and we're checking "x == 4", we'd proceed with 
        - if (nums[2] <= 4) then update "x = x+nums[2]", i.e. new coverage is [1, x+nums[2]), and move on to compare with "nums[3]"
        - else patch "nums[]" with "4" and just update "x = 2*x = 8" but still compare with "nums[2]".
        */
        
        ULL x = 1, nn = n;
        int curIdx = 0, patchCnt = 0;
        while (x <= nn) {
            if (curIdx < nums.size() && (ULL)nums[curIdx] <= x) {
                x += (ULL)nums[curIdx];
                ++curIdx;
            } else {
                ++patchCnt;
                x = (ULL)2*x;
            }
        }
        return patchCnt;
    }
};
