#include <stdio.h>
#include <vector>
using namespace std;

class Solution {
public:
    int trap(vector<int>& height) {
        int n = height.size();
        if (2 >= n) return 0;

        int sum = 0;
        
        vector<int> highestPilarToLeft(n, 0); // Yet not shorter than the current pilar height[i].
        vector<int> highestPilarToRight(n, 0); // Yet not shorter than the current pilar height[i].
        
        highestPilarToLeft[0] = height[0];
        for (int i = 1; i < n; ++i) {
            if (highestPilarToLeft[i-1] >= height[i]) {
                highestPilarToLeft[i] = highestPilarToLeft[i-1];
            } else {
                highestPilarToLeft[i] = height[i];
            }
        }
        
        highestPilarToRight[n-1] = height[n-1];
        for (int i = n-2; i >= 0; --i) {
            if (highestPilarToRight[i+1] >= height[i]) {
                highestPilarToRight[i] = highestPilarToRight[i+1];
            } else {
                highestPilarToRight[i] = height[i];
            }
        }
        
        for (int i = 0; i < n; ++i) {
            int shorterEdgePilar = (highestPilarToLeft[i] < highestPilarToRight[i] ? highestPilarToLeft[i] : highestPilarToRight[i]); 
            sum += (shorterEdgePilar - height[i]); 
        }
        return sum;
    }
};

int main() {
  int input[] = {0,1,0,2,1,0,1,3,2,1,2,1}; 
  vector<int> inputV(input, input+sizeof(input)/sizeof(int));
  Solution s;
  int output = s.trap(inputV);
  printf("%d\n", output);
  return 0;
}
