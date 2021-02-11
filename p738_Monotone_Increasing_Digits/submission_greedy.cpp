bool debug = false;

/*
test cases
10
1234
125552889
*/
class Solution {
public:
    int monotoneIncreasingDigits(int N) {
        vector<int> arr;
        int NN = N;
        while (NN > 0) {
            arr.push_back(NN%10);
            NN /= 10;
        }
        
        reverse(arr.begin(), arr.end());
        
        // Step#1, find the "LongestNonDecreasingSubarray" from the beginning
        int lastDigit = -1, i = -1;
        for (i = 0; i < arr.size(); ++i) {
            if (arr[i] < lastDigit) {
                break;
            }
            lastDigit = arr[i];
        }
        
        if (i == arr.size()) return N;
        
        // Step#2, find the first occurrence of the last digit in the "LongestNonDecreasingSubarray" 
        int firstEqualDigitPos = i-1;
        while (firstEqualDigitPos >= 0 && arr[firstEqualDigitPos] == lastDigit) --firstEqualDigitPos;
        
        ++firstEqualDigitPos;
        
        // Step#3, if "N" is "123455....52", the "lastDigit" is now "5" and "firstEqualDigitPos" points to its first occurred position.  
        arr[firstEqualDigitPos] -= 1;
        for (int j = firstEqualDigitPos+1; j < arr.size(); ++j) arr[j] = 9;
        int ans = 0;
        for (int j = 0; j < arr.size(); ++j) {
            ans *= 10;
            ans += arr[j];
        }
        
        return ans;
    }
};
