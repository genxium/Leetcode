/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
#include <queue>
using namespace std;

class HeapNode {
public:
    int val;
    ListNode* curOfList;
};

// The "priority_queue" is by default a max heap and I want to pop the smallest element.
class HeapNodePtrCmp 
{ 
public: 
    int operator() (const HeapNode* p1, const HeapNode* p2) 
    { 
        return p1->val > p2->val; 
    } 
}; 

class Solution {
public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        ListNode *toRet = NULL;
        ListNode *toRetTail = NULL;
        int const cntLists = lists.size();
        priority_queue<HeapNode*, vector<HeapNode*>, HeapNodePtrCmp> pq; 

        for (int i = 0; i < cntLists; ++i) {
            ListNode* curOfList = lists[i];
            if (NULL == curOfList) continue;
            HeapNode* hn = new HeapNode();
            hn->val = curOfList->val;
            hn->curOfList = curOfList;
            pq.push(hn);
        }
        while (0 < pq.size()) {
            // The "minHn" is reused of each list, or freed when iterated to the end of the corresponding list.
            HeapNode* minHn = pq.top();

            pq.pop();
            // printf("pq.size == %d, after popping minHn->val == %d.\n", pq.size(), minHn->val);

            ListNode* thatListHead = minHn->curOfList;            
            // To avoid pointer contamination of "thatListHead->next", use it to update the heap first.
            ListNode* nextOfThatListNode = thatListHead->next;
            if (NULL == nextOfThatListNode) {
                // printf("nextOfThatListNode is NULL.\n");
                delete minHn;
            } else {
                // printf("nextOfThatListNode has nextOfThatListNode->val == %d.\n", nextOfThatListNode->val);
                minHn->val = nextOfThatListNode->val;
                minHn->curOfList = nextOfThatListNode;
                pq.push(minHn);
            }
            
            if (NULL == toRet) {
                toRet = thatListHead;
                toRetTail = thatListHead;
            } else {
                toRetTail->next = thatListHead;
                toRetTail = toRetTail->next;
                toRetTail->next = NULL;
            }
        }
        return toRet;
    }
};
