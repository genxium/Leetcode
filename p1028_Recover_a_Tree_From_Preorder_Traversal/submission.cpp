class Solution {
public:
    TreeNode* recoverFromPreorderExtra(string &s, int level, int sSt, int sEd) {
        //printf("At level == %d\n", level);
        int dashCount = level+1;
        string rootValStr = "";
        while(s[sSt] == '-') {
            ++sSt;
        }
        while(isdigit(s[sSt])) {
            rootValStr.push_back(s[sSt]);
            ++sSt;
        }
        
        int leftChildSt = sSt, leftChildEd = sSt;
        int rightChildSt = sEd, rightChildEd = sEd;

        int cur = leftChildEd, lastConsecutiveDashSt = leftChildSt, lastConsecutiveDashEd = leftChildEd;
        int foundEquilengthDashCounter = 0;
        while (cur < sEd) {
            if (s[cur] == '-') {
                if (lastConsecutiveDashEd == lastConsecutiveDashSt) {
                    lastConsecutiveDashSt = cur;
                    lastConsecutiveDashEd = cur;
                }
                ++lastConsecutiveDashEd;
            }
            if (isdigit(s[cur])) {
                int lastConsecutiveDashLength = lastConsecutiveDashEd - lastConsecutiveDashSt;
                if (lastConsecutiveDashLength > 0) {
                    if (lastConsecutiveDashLength == dashCount) {
                        ++foundEquilengthDashCounter;
                        if (foundEquilengthDashCounter == 2) {
                            leftChildEd = cur - (dashCount);
                            rightChildSt = leftChildEd;
                        }
                    }
                    lastConsecutiveDashSt = lastConsecutiveDashEd;
                }
            }
            ++cur;
        }
        if (foundEquilengthDashCounter == 1) {
            // The right child should be NULL in this case.
            leftChildEd = cur;
        }
        
        int rootVal = atoi(rootValStr.c_str());
        //printf("\trootVal: %d, [leftChildSt: %d, leftChildEd: %d), [rightChildSt: %d, rightChildEd: %d)\n", rootVal, leftChildSt, leftChildEd, rightChildSt, rightChildEd);
        TreeNode *toRet = new TreeNode(rootVal);
        if (leftChildSt < leftChildEd) {
            TreeNode* leftChild = recoverFromPreorderExtra(s, level+1, leftChildSt, leftChildEd);
            toRet->left = leftChild;
        } 
        if (rightChildSt < rightChildEd) {
            TreeNode* rightChild = recoverFromPreorderExtra(s, level+1, rightChildSt, rightChildEd);
            toRet->right = rightChild;
        }
        return toRet;
    }

    TreeNode* recoverFromPreorder(string s) {
        return recoverFromPreorderExtra(s, 0, 0, s.length());
    }
};
