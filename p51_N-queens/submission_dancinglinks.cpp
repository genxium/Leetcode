#define DEBUGGING false // set it to false to disable debug logging
#define INVALID -1

class DcLinkNode {
public:
  int origY, origX;
  DcLinkNode *l, *r, *u, *d;
  DcLinkNode(): origY(INVALID), origX(INVALID), l(NULL), r(NULL), u(NULL), d(NULL) {
  }
  DcLinkNode(int aY, int aX): l(NULL), r(NULL), u(NULL), d(NULL) {
    this->origY = aY;
    this->origX = aX;
  }
};

DcLinkNode* buildLinks(vector<vector<int>> &adj, map<int, DcLinkNode*> &colHeads) {
  int nOrigAdjYUpper = adj.size();
  unordered_map<int, DcLinkNode*> colTails;

  DcLinkNode* grandHead = new DcLinkNode(INVALID, INVALID);
  
  for (int y = 0; y < nOrigAdjYUpper; ++y) {
    vector<int> xOfOnesAtThisY = adj[y];  
    DcLinkNode* rowHeadAtThisY = NULL;
    DcLinkNode* prevAtThisY = NULL;

    for (int j = 0; j < xOfOnesAtThisY.size(); ++j) {
      DcLinkNode* curColHead = NULL;
      int xOfOne = xOfOnesAtThisY[j];
    
      if (colHeads.find(xOfOne) == colHeads.end()) {
        // lazily create "colHead" respectively
        DcLinkNode* newColHead = new DcLinkNode(INVALID, xOfOne);
        colHeads[xOfOne] = newColHead;
      }
      curColHead = colHeads[xOfOne]; 

      DcLinkNode* newNode= new DcLinkNode(y, xOfOne); 
      if (NULL != prevAtThisY) {
        prevAtThisY->r = newNode;
        newNode->l = prevAtThisY;          
      } else {
        rowHeadAtThisY = newNode;
      }
      prevAtThisY = newNode;

      if (NULL == curColHead->d) {
        // connect with "curColHead" (vertically)
        curColHead->d = newNode;  
        newNode->u = curColHead;   
      }

      if (j == xOfOnesAtThisY.size()-1) {
        // connect cycle edges (horizontally)
        newNode->r = rowHeadAtThisY;  
        rowHeadAtThisY->l = newNode;
      }

      if (colTails.find(xOfOne) == colTails.end()) {
        colTails[xOfOne] = newNode;
      } else {
        DcLinkNode* prevInThisCol = colTails[xOfOne];
        prevInThisCol->d = newNode;
        newNode->u = prevInThisCol; 
        colTails[xOfOne] = newNode;
      }  
    }  
  }

  /* 
  By now,
  - for a single column all "DcLinkNode"s within it are inter-connected(vertically, including the respective "colHead" but not the cyclic edges), and 
  - for a single non-head row all "DcLinkNode"s within it are inter-connected(horizontally, including the cyclic edges), but 
  - "colHeads" are NOT inter-connected(horizontally, including "grandHead").
  */

  DcLinkNode* prevAtThisY = NULL;
  for (auto it : colHeads) {
    auto colHead = it.second; 
    auto colTail = colTails[it.first];

    // connect cycle edges (vertically)
    colTail->d = colHead;
    colHead->u = colTail;

    if (NULL != prevAtThisY) {
      prevAtThisY->r = colHead; 
      colHead->l = prevAtThisY;
    } 
    prevAtThisY = colHead;
  }
  grandHead->r = colHeads.begin()->second;
  colHeads.begin()->second->l = grandHead;
  
  // connect cyclic edges for "grandHead" (horizontally)
  colHeads.rbegin()->second->r = grandHead;
  grandHead->l = colHeads.rbegin()->second;
  
  return grandHead;  
}

void cover(DcLinkNode* c) {
  if (DEBUGGING) {
    printf("\t\t\tcovering c(origY: %d, origX: %d)\n", c->origY, c->origX);
  }
  c->r->l = c->l;
  c->l->r = c->r;

  DcLinkNode* itr1 = c->d;
  while (itr1 != c) {
    DcLinkNode* itr2 = itr1->r; 
    while (itr2 != itr1) {
      itr2->d->u = itr2->u;
      itr2->u->d = itr2->d;
      
      itr2 = itr2->r;
    }
    itr1 = itr1->d;
  }
} 

void uncover(DcLinkNode* c) {
  DcLinkNode* itr1 = c->u;
  while (itr1 != c) {
    DcLinkNode* itr2 = itr1->l; 
    while (itr2 != itr1) {
      itr2->d->u = itr2;
      itr2->u->d = itr2;

      itr2 = itr2->l;
    }
    itr1 = itr1->u;
  }

  c->r->l = c;
  c->l->r = c;
} 

vector<string> colVecToSolution(list<int> &selectedYs, int &n) {
  vector<string> toRet;
  for (int boardY = 0; boardY < n; ++boardY) {
    string row = "";
    for (int boardX = 0; boardX < n; ++boardX) {
      row.push_back('.');
    }
    toRet.push_back(row);
  }
  for (auto &yAdj : selectedYs) {
    int boardY = (yAdj/n), boardX = (yAdj%n);
    toRet[boardY][boardX] = 'Q';
  }
  return toRet;
}

void search(int k, DcLinkNode* grandHead, list<int> &selectedYs, map<int, DcLinkNode*> &colHeads, int &n, vector<vector<string>> &ans) {
  if (DEBUGGING) {
    printf("search, k:%d\n", k);
  }
  
  /*
    Note that the N-queens problem modeled here is slight different from "Exact Cover" in that a feasible answer doesn't have to hold a "1" in each "xAdj", for example answer
    ```
    .Q..
    ...Q
    Q...
    ..Q.
    ```
    doesn't fill "xAdj == 10" and "xAdj == 15".
  */
  if (k == n) {
    vector<string> singleAns = colVecToSolution(selectedYs, n);
    ans.push_back(singleAns);
    return;
  }

  DcLinkNode* c = grandHead->r;
  if (c == grandHead) {
    if (DEBUGGING) {
      printf("\tgot c(origY: %d, origX: %d) == grandHead(origY: %d, origX: %d)\n", c->origY, c->origX, grandHead->origY, grandHead->origX);
    }
    return;
  }

  if (DEBUGGING) {
    printf("\tchosen c(origY: %d, origX: %d)\n", c->origY, c->origX);
  }
  cover(c);
  DcLinkNode* itr1 = c->d;
  while (itr1 != c) {
    selectedYs.push_back(itr1->origY);
    if (DEBUGGING) {
      printf("\t\tselected itr1(origY: %d, origX: %d) into selectedYs\n", itr1->origY, itr1->origX);
    }
    DcLinkNode* itr2 = itr1->r;
    if (DEBUGGING) {
      printf("\t\t\tselected itr2(origY: %d, origX: %d) to start constraint covering\n", itr2->origY, itr2->origX);
    }
    while (itr2 != itr1) {
      DcLinkNode* colHead = colHeads[itr2->origX];
      cover(colHead);
      itr2 = itr2->r; 
    }
    search(k+1, grandHead, selectedYs, colHeads, n, ans);
    // backtracking#1
    selectedYs.pop_back();
    itr2 = itr1->l;
    while (itr2 != itr1) {
      DcLinkNode* colHead = colHeads[itr2->origX];
      uncover(colHead);
      itr2 = itr2->l; 
    }
    
    // move on to the next candidate
    itr1 = itr1->d;
  }
  // backtracking#2
  uncover(c);
}

class Solution {
public:
    vector< vector<string> > solveNQueens(int n) {
        vector<vector<int>> adj;
        // build the constraints
        /*
        - Each "yAdj" represents "placing a queen at (boardY: (yAdj/n), boardX: (yAdj%n))"
        - There're 4 types of "xAdj", 
          -- [0, n) represents "boardY occupied", 
          -- [n, 2n) represents "boardX occupied", 
          -- [2n, 4n-3) represents "diagonal occupied", 
          -- [4n-3, 6n-6) represents "reverse-diagonal occupied"
        */
        int yUpper = (n*n);  
        int xUpper = (6*n - 6);    

        for (int yAdj = 0; yAdj < yUpper; ++yAdj) {
          int boardY = (yAdj/n), boardX = (yAdj%n);
          adj.push_back(vector<int>{});
          
          // "boardY occupied" 
          adj[yAdj].push_back(boardY);

          // "boardX occupied" 
          adj[yAdj].push_back(boardX + n);

          // "diagonal occupied" 
          int diagonalConstraint = boardY + boardX + 2*n-1;
          if (diagonalConstraint >= 2*n && diagonalConstraint < 4*n-3) {
            adj[yAdj].push_back(diagonalConstraint);            
          }

          // "reverse-diagonal occupied" 
          int rDiagonalConstraint = n - 1 - boardY + boardX + 4*n - 4;
          if (rDiagonalConstraint >= 4*n-3 && rDiagonalConstraint < 6*n-6) {
            adj[yAdj].push_back(rDiagonalConstraint);
          }
          /*
          printf("@(boardY:%d, boardX: %d)\n\t", boardY, boardX);
          for (auto &xOfOne : adj[yAdj]) {
            printf("%d, ", xOfOne);
          }
          printf("\n");
          */
        }

        vector< vector<string> > ans;

        map<int, DcLinkNode*> colHeads; // ordered for traversal
        DcLinkNode* grandHead = buildLinks(adj, colHeads);

        int k = 0;
        list<int> selectedYs;
        search(k, grandHead, selectedYs, colHeads, n, ans);
        return ans;
    }
};

