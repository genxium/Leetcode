class Solution {
public:
    void genSolution(vector<string>& words, vector<int> &prefixLengthSums, int prefixLengthSumDeduction, int &maxWidth, int offsetInWords, vector<string> &ans) {
        if (offsetInWords == words.size()) {
            return;
        }
            
        //printf("checking from words[offsetInWords:%d] == %s, prefixLengthSumDeduction: %d\n", offsetInWords, words[offsetInWords].c_str(), prefixLengthSumDeduction);
            
        int cur = offsetInWords, minSpaceCount = 0; // we need at least 1 space between words
        while (cur < words.size() && prefixLengthSums[cur] - prefixLengthSumDeduction + minSpaceCount <= maxWidth) {
            ++cur;
            ++minSpaceCount;
        }
        
        bool isLastLine = (cur == words.size());
        /*
        Now that if (false == isLastLine), we've got
        - "prefixLengthSums[cur-1] - prefixLengthSumDeduction + (minSpaceCount-1) <= maxWidth", and
        - "prefixLengthSums[cur] - prefixLengthSumDeduction + minSpaceCount > maxWidth".
        */ 
        int wordsCountThisRow = (cur - offsetInWords);
        int totalSpaces = maxWidth - (prefixLengthSums[cur-1] - prefixLengthSumDeduction);
        int floored = 0; // abbreviated for "floored inter-word space count"
        int residueCount = totalSpaces;
        if (true == isLastLine) {
            /* 
            From "problem description"
            ```
            For the last line of text, it should be left justified 
            and no extra space is inserted between words.
            ```
            
            , but my preference is actually as follows.
            ```
            floored = totalSpaces/wordsCountThisRow;
            residueCount = totalSpaces - (floored*wordsCountThisRow);
            ```
            */
            floored = 1;
            residueCount = 0; // not used when "true == isLastLine"
        } else {
            if (wordsCountThisRow-1 > 0) {
                floored = totalSpaces/(wordsCountThisRow-1);
                residueCount = totalSpaces - (floored*(wordsCountThisRow-1));
            }
        }
        
        //printf("\tisLastLine: %d, wordsCountThisRow:%d, totalSpaces:%d, floored:%d, residueCount:%d\n", isLastLine, wordsCountThisRow, totalSpaces, floored, residueCount);

        string thisRow = "";
        for (int i = offsetInWords; i < cur; ++i) {
            thisRow.append(words[i]);
            if (i < cur-1) {
                for (int j = 0; j < floored; ++j) {
                    thisRow.push_back(' ');
                }
                if (residueCount > 0) {
                    thisRow.push_back(' ');
                    --residueCount;
                }    
            }
        }
        while (thisRow.length() < maxWidth) {
            // most likely only works for "true == isLastLine"
            thisRow.push_back(' ');
        }
    
        //printf("\tthisRow== %s\n", thisRow.c_str());
        ans.push_back(thisRow);
        genSolution(words, prefixLengthSums, prefixLengthSums[cur-1], maxWidth, cur, ans);
    }
    
    vector<string> fullJustify(vector<string>& words, int maxWidth) {
        /*
        test case #1
        ["ask","not","what","your","country","can","do","for","you","ask","what","you","can","do","for","your","country"]
        16
        */
        vector<int> prefixLengthSums;
        for (auto word : words) {
            int prevPrefixLengthSumSize = prefixLengthSums.size();
            if (0 == prevPrefixLengthSumSize) {
                prefixLengthSums.push_back(word.length());                
            } else {
                int prevSum = prefixLengthSums[prevPrefixLengthSumSize - 1];
                prefixLengthSums.push_back(prevSum + word.length());        
            }
        }
        
        vector<string> ans;
        genSolution(words, prefixLengthSums, 0, maxWidth, 0, ans);
        return ans;
    }
};
