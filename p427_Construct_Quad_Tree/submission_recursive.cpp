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
test case#1
[[1,1,1,1,0,0,0,0],[1,1,1,1,0,0,0,0],[1,1,1,1,1,1,1,1],[1,1,1,1,1,1,1,1],[1,1,1,1,0,0,0,0],[1,1,1,1,0,0,0,0],[1,1,1,1,0,0,0,0],[1,1,1,1,0,0,0,0]]

test case#2
[[1,1,0,0],[0,0,1,1],[1,1,0,0],[0,0,1,1]]
*/
class Solution {
public:
    Node* buildTree(vector<vector<int>>& grid, int yLow, int yHigh, int xLow, int xHigh) {
        if (yHigh-yLow == 1 && xHigh-xLow == 1) {
            Node *ret = new Node(grid[yLow][xLow], true);
            return ret;
        }
        
        int yMid = ((yLow + yHigh) >> 1);
        int xMid = ((xLow + xHigh) >> 1);

        Node* tL = buildTree(grid, yLow, yMid, xLow, xMid);
        Node* tR = buildTree(grid, yLow, yMid, xMid, xHigh);
        Node* bL = buildTree(grid, yMid, yHigh, xLow, xMid);
        Node* bR = buildTree(grid, yMid, yHigh, xMid, xHigh);
        
        if (
            tL->isLeaf && tR->isLeaf && bL->isLeaf && bR->isLeaf
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
            
            Node* ret = new Node(unifiedVal, true);           
            return ret;
        }
        
        Node* ret = new Node(false, false, tL, tR, bL, bR);
        return ret;
    }
    
    Node* construct(vector<vector<int>>& grid) {
        int yHigh = grid.size();
        int xHigh = grid[0].size();
        Node* ret = buildTree(grid, 0, yHigh, 0, xHigh);
        return ret;
    }
};
