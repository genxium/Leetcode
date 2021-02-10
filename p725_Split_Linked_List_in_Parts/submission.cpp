/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */

/*
test cases
[1,2,3]
5
[1,2,3,4]
5
[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
3
[]
3
*/
class Solution {
public:
    vector<ListNode*> splitListToParts(ListNode* root, int k) {
        vector<ListNode*> ans;
        
        int len = 0;
        ListNode* cur = root;
        while (cur) {
            ++len;
            cur = cur->next;
        }
        
        cur = root;
        int commonPartLen = len/k, bigParts = len%k;
        while (k--) {
            int thisPartLen = (bigParts > 0 ? commonPartLen+1 : commonPartLen);
            if (0 == thisPartLen) {
                ans.push_back(NULL);
            } else {
                ListNode *head = cur, *prev = NULL;
                while (thisPartLen--) {
                    prev = cur;
                    cur = cur->next;    
                }
                prev->next = NULL;
                ans.push_back(head);
            }
            
            --bigParts;
        }
        
        return ans;
    }
};
