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
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    TreeNode* sortedListToBST(ListNode* head) {
        if (NULL == head) return NULL;
        //printf("handling head->val:%d\n", head->val);
        ListNode *preStepper=NULL, *stepper=head, *doubleStepper = head;
        while (NULL != stepper && NULL != doubleStepper) {
            doubleStepper = doubleStepper->next;
            if (NULL != doubleStepper) {
                doubleStepper = doubleStepper->next;
            } else {
                break;
            }
            preStepper = stepper;
            stepper = stepper->next;
        }
        // now that "NULL == doubleStepper"
        TreeNode* single = new TreeNode(stepper->val);
        //printf("\tchose stepper->val:%d\n", stepper->val);

        ListNode* cachedNext = stepper->next;
        if (stepper != head) {
            preStepper->next = NULL; // avoid "stepper" being reused
            delete stepper;
            single->left = sortedListToBST(head);
        }
        if (NULL != cachedNext) {
            single->right = sortedListToBST(cachedNext);
        }
        return single;
    }
};
