#define INVALID -1

class Solution {
public:
    vector<int> findDuplicates(vector<int>& nums) {
        vector<int> ans;
        int n = nums.size();
        for (int i = 0; i < n; ++i) {
            int num = nums[i];
            int correspondingIndex = num-1;
            if (correspondingIndex == i) {
                continue;
            }
            while (INVALID < correspondingIndex && nums[correspondingIndex] != num) {
                swap(nums[i], nums[correspondingIndex]);
                num = nums[i];
                correspondingIndex = num-1;
            }
            
            // now that "nums[nums[i]-1] == nums[i]" || INVALID >= corresponding
            if (INVALID >= correspondingIndex || correspondingIndex == i) {
                continue;
            }
        
            //printf("@i:%d, pushing back num:%d\n", i, num);
            ans.push_back(num);
            nums[i] = INVALID;
        }
        return ans;
    }
