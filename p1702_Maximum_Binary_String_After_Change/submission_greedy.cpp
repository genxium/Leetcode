/*
test cases
"000110"
"01"
"00000000001111000"
"11"
*/
class Solution {
public:
    string maximumBinaryString(string binary) {
        /*
        The greedy trick is that for "[0000...0000]1111..." where the number of consecutive 0s are larger than 3, we can always use only "Operation 1" to end with a replacement "[1111...1110]1111...". We should apply the first batch of consecutive 0s such that the 1s come as early as possible.
        
        For the second batch of consecutive 0s, e.g. "[1111...1110]1111...1111[0000...0000]", we should 
        - use "Operation 2" to shift the 1s to "[1111...1110]0111...1111[1000...0000]", then 
        - applying "Operation 1" will result in "[1111...1111]0111...1111[1000...0000]"
        , and repeat the two steps above till we don't have any trailing 0, we end with something like "1111...111111111111110111...1111". 
        
        That said, once we can apply "Operation 1" to the input, we can always get a result with only one '0' occurring at a position as late as possible.
        */
        
        int onlyZeroPos = -1;
        list<int> consecutiveZeroPosList;
        for (int i = 0; i < binary.length(); ++i) {
            if (binary[i] == '0') {
                consecutiveZeroPosList.push_back(i);
            } else {
                if (-1 == onlyZeroPos && consecutiveZeroPosList.size() > 0) {
                    onlyZeroPos = consecutiveZeroPosList.back();
                } else {
                    onlyZeroPos += consecutiveZeroPosList.size();
                }
                consecutiveZeroPosList.clear();
            }
        }      
        
        // wrap up
        if (-1 == onlyZeroPos && consecutiveZeroPosList.size() > 0) {
            onlyZeroPos = consecutiveZeroPosList.back();
        } else {
            onlyZeroPos += consecutiveZeroPosList.size();
        }

        if (onlyZeroPos == -1) return binary; // no "Operation 1" is applicable 
        string ans;
        for (int i = 0; i < binary.length(); ++i) {
            if (i == onlyZeroPos) ans.push_back('0');
            else ans.push_back('1');
        }
        return ans;
    }
};
