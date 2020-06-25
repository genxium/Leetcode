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
class Solution {
public:
    ListNode* extractCurFromList(ListNode* pre, ListNode** pCur, ListNode** pHead) {
        ListNode* cur = *pCur;
        if (NULL != pre) {
            pre->next = cur->next;
            cur->next = NULL;
            *pCur = pre->next; // Updates the "on-stack variable of caller"
        } else {
            *pHead = cur->next; // Updates the "on-stack variable of caller"
            cur->next = NULL;
            *pCur = *pHead; // Updates the "on-stack variable of caller".
        }
        return cur;
    }
    
    ListNode* partition(ListNode* head, int x) {  
        if (NULL == head) {
            return NULL;
        }
        ListNode *gteListHead = NULL, *gteListTail = NULL;
        ListNode *lessListHead = NULL, *lessListTail = NULL;
        
        ListNode *pre = NULL, *cur = head; 
        while (NULL != cur) {
            ListNode* extracted = extractCurFromList(pre, &cur, &head);
            if (extracted->val < x) {
                if (NULL == lessListHead) {
                    lessListHead = extracted;
                    lessListTail = extracted;
                } else {
                    lessListTail->next = extracted;
                    lessListTail = lessListTail->next;
                }
            } else {
                if (NULL == gteListHead) {
                    gteListHead = extracted;
                    gteListTail = extracted;
                } else {
                    gteListTail->next = extracted;
                    gteListTail = gteListTail->next;
                }
            }
        }
        if (NULL != lessListTail) {
            lessListTail->next = gteListHead;
            return lessListHead;
        } else {
            return gteListHead;
        }
    }
};
