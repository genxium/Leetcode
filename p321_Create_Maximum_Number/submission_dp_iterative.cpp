class Solution {
private:
    void printMaxSubseqsOfLengths(vector<string> &subseqs) {
        for (int len = 1; len < subseqs.size(); ++len) {
            printf("len:%d, subseq: %s\n", len, subseqs[len].c_str());
        }
        printf("\n");
    }
    
public:
    vector<string> maxSubsequencesOfLengths(vector<int> &nums, int upperLength) {
        //printf("#checkpoint-a\n");
        int n = nums.size();
        upperLength = min(upperLength, n);
        
         // "dp[len]" the maximum subsequence of length "len"
        vector<string> dp;
        dp.assign(upperLength+1, "");
        
        for (int i = 0; i < n; ++i) {
            // update all "dp[len]"s for "nums[0, ..., i]" 
            int effectiveUpperLength = min(i+1, upperLength);
            for (int len = effectiveUpperLength; len >= 1; --len) {
                // traverse "len" in reverse-order to make sure that "dp[currentRound][len]" is calculated from "dp[currentRound-1][len-1]"(BUT NOT "dp[currentRound][len-1]"!)
                string candidateWithNewChar = dp[len-1] + to_string(nums[i]); 
                dp[len] = max(dp[len], candidateWithNewChar);
                
                //printf("i:%d, dp[len:%d] is updated to %s\n", i, len, dp[len].c_str());
            }
        }
        //printf("#checkpoint-b\n");
        return dp;
    }
        
    vector<int> maxNumber(vector<int>& nums1, vector<int>& nums2, int k) {
        vector<string> maxSubseqsOfLens1 = maxSubsequencesOfLengths(nums1, k);
        //printMaxSubseqsOfLengths(maxSubseqsOfLens1);
        
        vector<string> maxSubseqsOfLens2 = maxSubsequencesOfLengths(nums2, k);
        //printMaxSubseqsOfLengths(maxSubseqsOfLens2);
        
        string ansStr = "";
        int cnt1Upper = k < nums1.size() ? k : nums1.size();
        for (int cnt1 = 0; cnt1 <= cnt1Upper; ++cnt1) {
            int cnt2 = (k - cnt1);
            if (cnt2 > nums2.size()) continue;
            //printf("cnt1 == %d, cnt2 == %d\n", cnt1, cnt2);
            string maxSubseq1 = (0 == cnt1 ? "" : maxSubseqsOfLens1[cnt1]);
            string maxSubseq2 = (0 == cnt2 ? "" : maxSubseqsOfLens2[cnt2]);
            int ptr1 = 0, ptr2 = 0;
            string candidate = "";
            
            while (candidate.length() < k) {
                /*
                Note that "string-compare"(effectively "suffix-compare") is used here instead of "two-pointers", because we might have "maxSubseq1[ptr1] == maxSubseq2[ptr2]" which is a case where potential inspection of the whole suffix is necessary.
                */
                if (maxSubseq1 > maxSubseq2) {
                    candidate.push_back(maxSubseq1[0]);
                    maxSubseq1 = maxSubseq1.substr(1);
                } else {
                    candidate.push_back(maxSubseq2[0]);
                    maxSubseq2 = maxSubseq2.substr(1);
                }
            }
            
            if (candidate > ansStr) {
                ansStr = candidate;
            }
        }
        
        vector<int> ans;
        for (auto &ch : ansStr) {
            ans.push_back(ch-'0');
        }
        return ans;
    }
};
