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
    
    ListNode* reverseKGroup(ListNode* head, int k) {
        if (NULL == head) return NULL;
        ListNode *toRet = NULL; 
            
        ListNode *preGroupTail = NULL;
        ListNode *pre = NULL, *cur = head;
        
        int groupCount = 0;
        while (NULL != cur) {
            printf("Start processing group %d\n", groupCount);
            ListNode *curGroupHead = cur;
            ListNode *curGroupTail = cur;
            ListNode *cachedNext = cur->next;
            int kk = k, i;
            for (i = 1; i < kk; ++i) {
                curGroupTail = curGroupTail->next;
                if (NULL == curGroupTail) break;
            }

            if (i == k) {
                //printf("\tFound current group tail with value %d\n", curGroupTail->val);

                bool groupAssembled = false;
                while (cur != curGroupTail->next) {
                    if (NULL != pre) {
                        if (pre != preGroupTail) {
                            cur->next = pre;
                        } else {
                            // (pre == preGroupTail) != NULL
                            pre->next = NULL;
                            cur->next = NULL;
                        }
                    } 
                    if (cur == curGroupTail) {
                        ListNode *temp = curGroupTail;
                        curGroupTail = curGroupHead;
                        curGroupHead = temp;
                        
                        if (NULL == toRet) {
                            toRet = curGroupHead;  
                        }
                        
                        if (NULL != preGroupTail) {
                            preGroupTail->next = curGroupHead;
                        }
                        curGroupTail->next = cachedNext;
                        
                        // move across group border
                        preGroupTail = curGroupTail;
                        // curGroupHead, curGroupTail will be re-assigned to "cur" in the next "big loop"
                        groupAssembled = true;
                    }
                    // move in current group or cross group border
                    pre = cur;
                    cur = cachedNext;
                    if (NULL != cur) {
                        cachedNext = cur->next;                        
                    }
                    
                    if (groupAssembled) {
                        break;
                    }
                }
            } else {
                // last residue group with <k elements
                break;
            }        
            ++groupCount;
        }
        if (NULL == toRet) {
            toRet = head;  
        }
        return toRet;
    }
};
