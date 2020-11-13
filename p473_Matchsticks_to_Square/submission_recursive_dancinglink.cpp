int const INVALID = -1;

class DcLinkNode {
public:
  int val; 
  DcLinkNode *l, *r;
  DcLinkNode(): val(INVALID), l(NULL), r(NULL) {
  }
  DcLinkNode(int aVal): l(NULL), r(NULL) {
    this->val = aVal;
  }
};

void cover(DcLinkNode* c) {
    if (c->r) {
        c->r->l = c->l;            
    }
    if (c->l) {
        c->l->r = c->r;            
    }
} 

void uncover(DcLinkNode* c) {
    if (c->r) {
        c->r->l = c;            
    }
    if (c->l) {
        c->l->r = c;            
    }
}

/*
test case#1
[1,1,2,2,2]

test case#2
[3,3,3,3,4]

test case#3
[5,5,5,5,16,4,4,4,4,4,3,3,3,3,4]
*/

class Solution {
public:
    void solve(DcLinkNode* grandHead, int segCount, int stdRemainingLen, int remainingLen, bool &ans) {
        if (ans) return;

        if (5 <= segCount) return;
        
        if (4 == segCount) {
            if (!grandHead->r) {
                ans = true;
            }
            return;
        }
        
        DcLinkNode* cur = grandHead->r;
        while (NULL != cur) {
            if (cur->val == remainingLen) {
                cover(cur);  
                solve(grandHead, segCount+1, stdRemainingLen, stdRemainingLen, ans);
                uncover(cur);  
            } else if (cur->val < remainingLen) {
                cover(cur);  
                solve(grandHead, segCount, stdRemainingLen, remainingLen-cur->val, ans);
                uncover(cur);
            } else {
                // cur->val > remainingLen, skip 
            } 
            
            /*
            [TRICK to avoid TLE] After returning from deeper recursion and uncovered, don't try sticks with the same length.
            
            The "StateTuple" for this problem is actually (vector<pair<int, int>> remainingSticksWithCount, segCount, remainingLenOfCurrentSeg), and we'd like to AVOID re-entering the same "StateTuple" in future search.
            */
            DcLinkNode *next = cur;
            while (next && next->val == cur->val) {
                next = next->r;
            }
            cur = next;
        }
    }
    
    bool makesquare(vector<int>& nums) {
        sort(nums.begin(), nums.end(), greater<int>());       
        DcLinkNode *grandHead = new DcLinkNode();
        DcLinkNode *prev = grandHead;

        int totalLen = 0;
        for (auto num : nums) {
            DcLinkNode *single = new DcLinkNode(num);
            prev->r = single;
            single->l = prev;
            totalLen += num;
            prev = single;
        }

        if (totalLen % 4 != 0) return false;
        int stdRemainingLen = totalLen/4;
        bool ans = false;
        solve(grandHead, 0, stdRemainingLen, stdRemainingLen, ans);
        return ans;
    }
};
