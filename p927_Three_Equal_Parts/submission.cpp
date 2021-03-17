/*
test cases
[1,0,1,0,1]
[1,1,0,1,1]
[1,1,0,0,1]
[0,0,0]
*/
class Solution {
public:
    vector<int> threeEqualParts(vector<int>& arr) {
        int n = arr.size();
        vector<int> onePos;
        for (int i = 0; i < arr.size(); ++i) {
            if (0 == arr[i]) continue;
            onePos.push_back(i);
        }

        int m = onePos.size();
        // For the 3 equal parts, the count of ones should be identical.
        if (0 < m && m%3 != 0) {
            return {-1, -1};
        }

        if (0 == m) {
            return {n/3-1, 2*n/3};
        }

        int oneCntInEachPart = onePos.size()/3;
        // For the 3 equal parts, the distance between each adjacent "onePos" should be identical.
        for (int i = 1; i < oneCntInEachPart; ++i) {
            int dis1 = onePos[i]-onePos[i-1];
            int dis2 = onePos[i+oneCntInEachPart] - onePos[i+oneCntInEachPart-1];
            int dis3 = onePos[i+oneCntInEachPart+oneCntInEachPart] - onePos[i+oneCntInEachPart+oneCntInEachPart-1];
            if (dis1 != dis2) return {-1, -1};
            if (dis1 != dis3) return {-1, -1};
        }

        int trailingZero1 = onePos[oneCntInEachPart] - onePos[oneCntInEachPart-1];
        int trailingZero2 = onePos[oneCntInEachPart+oneCntInEachPart] - onePos[oneCntInEachPart+oneCntInEachPart-1];
        int trailingZero3 = n - onePos.back(); // Cannot be trimmed by cutting into "leading zeros", thus must be smallest

        if (trailingZero3 > trailingZero1 || trailingZero3 > trailingZero2) return {-1, -1};

        // Now that we can definitely partition into 3 equal parts respecting "trailingZero3"
        int cut1 = onePos[oneCntInEachPart-1] + trailingZero3-1;
        int cut2 = onePos[oneCntInEachPart+oneCntInEachPart-1]+ trailingZero3;

        return {cut1, cut2};
    }
};
