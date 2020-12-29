/*
// Definition for a QuadTree node.
class Node {
public:
    bool val;
    bool isLeaf;
    Node* topLeft;
    Node* topRight;
    Node* bottomLeft;
    Node* bottomRight;
    
    Node() {
        val = false;
        isLeaf = false;
        topLeft = NULL;
        topRight = NULL;
        bottomLeft = NULL;
        bottomRight = NULL;
    }
    
    Node(bool _val, bool _isLeaf) {
        val = _val;
        isLeaf = _isLeaf;
        topLeft = NULL;
        topRight = NULL;
        bottomLeft = NULL;
        bottomRight = NULL;
    }
    
    Node(bool _val, bool _isLeaf, Node* _topLeft, Node* _topRight, Node* _bottomLeft, Node* _bottomRight) {
        val = _val;
        isLeaf = _isLeaf;
        topLeft = _topLeft;
        topRight = _topRight;
        bottomLeft = _bottomLeft;
        bottomRight = _bottomRight;
    }
};
*/

/*
test cases
[[0,1],[1,1],[1,1],[1,0],[1,0]]
[[0,1],[1,1],[0,1],[1,1],[1,0],null,null,null,null,[1,0],[1,0],[1,1],[1,1]]
[[1,0]]
[[1,0]]
[[0,0],[1,0],[1,0],[1,1],[1,1]]
[[0,0],[1,1],[1,1],[1,0],[1,1]]
[[0,0],[1,1],[1,0],[1,1],[1,1]]
[[0,0],[1,1],[0,1],[1,1],[1,1],null,null,null,null,[1,1],[1,0],[1,0],[1,1]]
[[0,1],[1,0],[0,1],[1,1],[1,0],null,null,null,null,[1,0],[1,0],[1,1],[1,1]]
[[0,1],[0,1],[1,0],[1,1],[1,0],[1,0],[1,0],[1,1],[1,1]]
*/
class Solution {
public:
    Node* intersect(Node* quadTree1, Node* quadTree2) {
        if (!quadTree1 || !quadTree2) {
            // At each recursion, it's guaranteed that "quadTree1->(anyChildBoundary) == quadTree2->(anyChildBoundary)".
            return NULL;
        }
        if (quadTree1->isLeaf && quadTree2->isLeaf) {
            Node* toRet = new Node(quadTree1->val | quadTree2->val, true);
            return toRet;
        }
        
        Node *tL = NULL, *tR = NULL, *bL = NULL, *bR = NULL;
        if (!quadTree1->isLeaf && !quadTree2->isLeaf) {
            tL = intersect(quadTree1->topLeft, quadTree2->topLeft);
            tR = intersect(quadTree1->topRight, quadTree2->topRight);
            bL = intersect(quadTree1->bottomLeft, quadTree2->bottomLeft);
            bR = intersect(quadTree1->bottomRight, quadTree2->bottomRight);
        }
        
        if (!quadTree1->isLeaf && quadTree2->isLeaf) {
            tL = intersect(quadTree1->topLeft, quadTree2);
            tR = intersect(quadTree1->topRight, quadTree2);
            bL = intersect(quadTree1->bottomLeft, quadTree2);
            bR = intersect(quadTree1->bottomRight, quadTree2);
        }
        
        if (quadTree1->isLeaf && !quadTree2->isLeaf) {
            tL = intersect(quadTree1, quadTree2->topLeft);
            tR = intersect(quadTree1, quadTree2->topRight);
            bL = intersect(quadTree1, quadTree2->bottomLeft);
            bR = intersect(quadTree1, quadTree2->bottomRight);
        }
        
        if (
           tL && tR && bL && bR
           && tL->isLeaf && tR->isLeaf && bL->isLeaf && bR->isLeaf
           && tL->val == tR->val
           && tL->val == bL->val
           && tL->val == bR->val
        ) {
            // deallocate the heapmem spaces
            int unifiedVal = tL->val;
            delete tL;
            delete tR;
            delete bL;
            delete bR;
            
            return new Node(unifiedVal, true);           
        } else {
            return new Node(false /*just arbitrarily chosen*/, false, tL, tR, bL, bR);
        }
    }
};
