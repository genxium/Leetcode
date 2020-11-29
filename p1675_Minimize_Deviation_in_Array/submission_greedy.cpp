/*
test cases
[1,2,3,4]
[4,1,5,20,3]
[2,10,8]
*/
class Solution {
private:
    bool isEven(int x) {
        return ((x & 1) == 0);
    }
public:
    int minimumDeviation(vector<int>& nums) {
        set<int> candidates;
        for (auto num : nums) {
            /*
            [1st trick] 
            The answer will be the same if all the "originally odd nums" were accounted as doubled the value.
            */
            if (isEven(num)) candidates.insert(num);
            else candidates.insert((num << 1));
        }
        
        int ans = *candidates.rbegin() - *candidates.begin();
        while (isEven(*candidates.rbegin())) {
            int localMax = (*candidates.rbegin());
            int trs = (localMax >> 1);
            candidates.erase(localMax);
            candidates.insert(trs);
            int tmp = *candidates.rbegin() - *candidates.begin();
            ans = min(ans, tmp);
        }
        /*
        [2nd trick]
        Now that the max of "candidates" is an odd number which couldn't be reduced anymore.
        */
        return ans;
    }
};
