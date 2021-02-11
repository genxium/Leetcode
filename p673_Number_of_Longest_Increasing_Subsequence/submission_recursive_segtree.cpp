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
        return ((leftIndexClosed + rightIndexOpen) >> 1);
    }
    
public:
    int leftIndexClosed;
    int rightIndexOpen; 

    /*
    {lisLength, lisCount}, means that the "LongestIncreasingSubsequence"s ended at "nums[...]" which are valued(INSTEAD OF "indexed") within [leftIndexClosed, rightIndexOpen) has maximum length "lisLength" and there're "lisCount" of it.
    
    This definition is CRITICAL, because it makes any "root->lisResult" an aggregate of its childrens' "lisResult"s.
    */
    PII lisResult;

    SegmentTreeNode* lChild = NULL; // To be initialized upon construction, and would be NULL for a leaf, i.e. (rightIndexOpen == 1 + leftIndexClosed).
    SegmentTreeNode* rChild = NULL; // To be initialized upon construction, and would be NULL for a leaf, i.e. (rightIndexOpen == 1 + leftIndexClosed).
    
    SegmentTreeNode(int newSegLeftIndexClosed, int newSegRightIndexOpen) {
        this->leftIndexClosed = newSegLeftIndexClosed;
        this->rightIndexOpen = newSegRightIndexOpen;
        this->lisResult = INIT_LIS_RESULT;
    }
    
    SegmentTreeNode* getOrCreateLChild() {
        if (!lChild) lChild = new SegmentTreeNode(leftIndexClosed, calcMid());
        return lChild;
    }
    
    SegmentTreeNode* getOrCreateRChild() {
        if (!rChild) rChild = new SegmentTreeNode(calcMid(), rightIndexOpen);
        return rChild;
    }

    void RangeAdd(int newSegLeftIndexClosed, int newSegRightIndexOpen, PII newLisResult, int level) {
        // Reject invalid "[newSegLeftIndexClosed, newSegRightIndexOpen)"s. 
        if (newSegLeftIndexClosed >= newSegRightIndexOpen) return;
        if (newSegLeftIndexClosed >= rightIndexOpen) return;
        if (newSegRightIndexOpen <= leftIndexClosed) return;

        int indentSpaceCount = (level << 1);
        if (debug) printf("%*sRangeAdd, [newSegLeftIndexClosed:%d, newSegRightIndexOpen:%d), (newLisLength: %d, newLisCount: %d)\n", indentSpaceCount, "", newSegLeftIndexClosed, newSegRightIndexOpen, newLisResult.first, newLisResult.second);

        if (newSegLeftIndexClosed <= leftIndexClosed && rightIndexOpen <= newSegRightIndexOpen) {
            lisResult = aggregate(newLisResult, lisResult);
            if (debug) printf("%*sRangeAdd-END#1, updated (newLisLength: %d, newLisCount: %d)\n", indentSpaceCount, "", lisResult.first, lisResult.second);
            return;
        }

        getOrCreateLChild()->RangeAdd(newSegLeftIndexClosed, newSegRightIndexOpen, newLisResult, level+1);
        getOrCreateRChild()->RangeAdd(newSegLeftIndexClosed, newSegRightIndexOpen, newLisResult, level+1);        
        lisResult = aggregate(lChild->lisResult, rChild->lisResult);
        if (debug) printf("%*sRangeAdd-END#2, updated (newLisLength: %d, newLisCount: %d)\n", indentSpaceCount, "", lisResult.first, lisResult.second);
    }

    PII RangeMax(int targetLeftIndexClosed, int targetRightIndexOpen) {
        PII result = INIT_LIS_RESULT;
        
        // Reject invalid "[targetLeftIndexClosed, targetRightIndexOpen)"s. 
        if (targetLeftIndexClosed >= targetRightIndexOpen) return result;
        if (targetLeftIndexClosed >= rightIndexOpen) return result;
        if (targetRightIndexOpen <= leftIndexClosed) return result;

        if (targetLeftIndexClosed <= leftIndexClosed && rightIndexOpen <= targetRightIndexOpen) {
            return lisResult;
        }
                
        if (lChild) result = aggregate(result, lChild->RangeMax(targetLeftIndexClosed, targetRightIndexOpen));
        if (rChild) result = aggregate(result, rChild->RangeMax(targetLeftIndexClosed, targetRightIndexOpen));
        
        /*
        [WARNING]
        
        By now the "result" is for interval [targetLeftIndexClosed, targetRightIndexOpen), which DOESN'T FULLY COVER [this->leftIndexClosed, this->rightIndexOpen), hence DON'T UPDATE "this->lisResult" by "result"!
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
        // [WARNING] It's better to index "nums" first, but anyway it works for now.
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

