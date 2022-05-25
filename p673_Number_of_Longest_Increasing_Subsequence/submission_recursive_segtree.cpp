bool const debug = false;
typedef pair<int, int> PII;
PII const INIT_LIS_RESULT = {0, 1};

/*
test cases
[1]
[1,3,5,4,7]
[2,2,2,2,2]
[3,2,1]
[1,2,4,3,4]
[1,2,3,2,3]
[1,2,3,2,3,1,2,3,2,3]
*/
PII aggregate(PII lhs, PII rhs) {
    if (lhs.first == rhs.first) {
        if (0 == lhs.first) return INIT_LIS_RESULT; // Edge case here, don't aggregate the count of "lisLength == 0".
        return {lhs.first, lhs.second+rhs.second};
    }  

    return (lhs.first > rhs.first ? lhs : rhs);
}

class SegmentTreeNode {
private:
    int calcMid() {
        return ((leftClosed + rightOpen) >> 1);
    }
    
public:
    int leftClosed;
    int rightOpen; 

    /*
    {lisLength, lisCount}, means that the "LongestIncreasingSubsequence"s ended at "nums[...]" which are valued(INSTEAD OF "indexed") within [leftClosed, rightOpen) has maximum length "lisLength" and there're "lisCount" of it.
    
    This definition is CRITICAL, because it makes any "root->lisResult" an aggregate of its childrens' "lisResult"s.
    */
    PII lisResult;

    SegmentTreeNode* lChild = NULL; // To be initialized upon construction, and would be NULL for a leaf, i.e. (rightOpen == 1 + leftClosed).
    SegmentTreeNode* rChild = NULL; // To be initialized upon construction, and would be NULL for a leaf, i.e. (rightOpen == 1 + leftClosed).
    
    SegmentTreeNode(int newSegLeftClosed, int newSegRightOpen) {
        this->leftClosed = newSegLeftClosed;
        this->rightOpen = newSegRightOpen;
        this->lisResult = INIT_LIS_RESULT;
    }
    
    SegmentTreeNode* getOrCreateLChild() {
        if (!lChild) lChild = new SegmentTreeNode(leftClosed, calcMid());
        return lChild;
    }
    
    SegmentTreeNode* getOrCreateRChild() {
        if (!rChild) rChild = new SegmentTreeNode(calcMid(), rightOpen);
        return rChild;
    }

    void RangeAdd(int newSegLeftClosed, int newSegRightOpen, PII newLisResult, int level) {
        // Reject invalid "[newSegLeftClosed, newSegRightOpen)"s. 
        if (newSegLeftClosed >= newSegRightOpen) return;
        if (newSegLeftClosed >= rightOpen) return;
        if (newSegRightOpen <= leftClosed) return;

        int indentSpaceCount = (level << 1);
        if (debug) printf("%*sRangeAdd, [newSegLeftClosed:%d, newSegRightOpen:%d), (newLisLength: %d, newLisCount: %d)\n", indentSpaceCount, "", newSegLeftClosed, newSegRightOpen, newLisResult.first, newLisResult.second);

        if (newSegLeftClosed <= leftClosed && rightOpen <= newSegRightOpen) {
            lisResult = aggregate(newLisResult, lisResult);
            if (debug) printf("%*sRangeAdd-END#1, updated (newLisLength: %d, newLisCount: %d)\n", indentSpaceCount, "", lisResult.first, lisResult.second);
            return;
        }

        getOrCreateLChild()->RangeAdd(newSegLeftClosed, newSegRightOpen, newLisResult, level+1);
        getOrCreateRChild()->RangeAdd(newSegLeftClosed, newSegRightOpen, newLisResult, level+1);        
        lisResult = aggregate(lChild->lisResult, rChild->lisResult);
        if (debug) printf("%*sRangeAdd-END#2, updated (newLisLength: %d, newLisCount: %d)\n", indentSpaceCount, "", lisResult.first, lisResult.second);
    }

    PII RangeMax(int targetLeftClosed, int targetRightOpen) {
        PII result = INIT_LIS_RESULT;
        
        // Reject invalid "[targetLeftClosed, targetRightOpen)"s. 
        if (targetLeftClosed >= targetRightOpen) return result;
        if (targetLeftClosed >= rightOpen) return result;
        if (targetRightOpen <= leftClosed) return result;

        if (targetLeftClosed <= leftClosed && rightOpen <= targetRightOpen) {
            return lisResult;
        }
                
        if (lChild) result = aggregate(result, lChild->RangeMax(targetLeftClosed, targetRightOpen));
        if (rChild) result = aggregate(result, rChild->RangeMax(targetLeftClosed, targetRightOpen));
        
        /*
        [WARNING]
        
        By now the "result" is for interval [targetLeftClosed, targetRightOpen), which DOESN'T FULLY COVER [this->leftClosed, this->rightOpen), hence DON'T UPDATE "this->lisResult" by "result"!
        */
        return result;
    }
};

class Solution {
public:
    int findNumberOfLIS(vector<int>& nums) {
        int minNum = INT_MAX, maxNum = INT_MIN;
        for (auto num : nums) {
            minNum = min(minNum, num);
            maxNum = max(maxNum, num);
        }
        SegmentTreeNode* root = new SegmentTreeNode(minNum, maxNum+1);
        
        for (int i = 0; i < nums.size(); ++i) {
            PII cand = root->RangeMax(minNum, nums[i]); // For every "nums[j < i] < nums[i]" 
            /*
            Deliberately using "RangeAdd(...)" instead of "RangeWrite(...)" here, consider "[1, 2, 4, 3, 4]" as an example, the value "4" will be visited twice.

            See https://www.yinxiang.com/everhub/note/b904af18-03fd-4dbc-a3d2-67a0daa1518e for an introduction to SegmentTree properties.
            */
            root->RangeAdd(nums[i], nums[i]+1, {cand.first+1, cand.second}, 0);
        }
        
        PII ans = root->RangeMax(minNum, maxNum+1);
        if (debug) printf("the lisLength is %d\n", ans.first);
        return ans.second;
    }
};

