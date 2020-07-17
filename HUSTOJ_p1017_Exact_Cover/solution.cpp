#define MAXN 1001
#define DEBUGGING false // set it to false to disable debug logging

#include <stdio.h>
#include <vector>
#include <map>
#include <unordered_map>
#include <list>
#define INVALID -1
using namespace std;

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
  
  if (DEBUGGING) {
    printf("buildLinks, nOrigAdjYUpper:%d\n", nOrigAdjYUpper);
  }

  for (int y = 0; y < nOrigAdjYUpper; ++y) {
    vector<int> xOfOnesAtThisY = adj[y];  
    DcLinkNode* rowHeadAtThisY = NULL;
    DcLinkNode* prevAtThisY = NULL;

    if (DEBUGGING) {
      printf("\tChecking the %d-th row.\n", y);
    }

    for (int j = 0; j < xOfOnesAtThisY.size(); ++j) {
      DcLinkNode* curColHead = NULL;
      int xOfOne = xOfOnesAtThisY[j];
      if (DEBUGGING) {
        printf("\t\tChecking the %d-th column.\n", xOfOne);
      }
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
        if (DEBUGGING) {
          printf("\t\tConnected curColHead(origY: %d, origX: %d) <-> newNode(origY: %d, origX: %d) vertically.\n", curColHead->origY, curColHead->origX, newNode->origY, newNode->origX);
        }
      }

      if (j == xOfOnesAtThisY.size()-1) {
        // connect cycle edges (horizontally)
        newNode->r = rowHeadAtThisY;  
        rowHeadAtThisY->l = newNode;
        if (DEBUGGING) {
          printf("\t\tConnected cyclic edge rowHeadAtThisY(origY: %d, origX: %d) <-> newNode(origY: %d, origX: %d) horizontally.\n", rowHeadAtThisY->origY, rowHeadAtThisY->origX, newNode->origY, newNode->origX);
        }
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

  if (DEBUGGING) {
    printf("\tbuildLinks, milestone#1\n");
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
    if (DEBUGGING) {
      printf("\t\tConnected cyclic edge colHead(origY: %d, origX: %d) <-> colTail(origY: %d, origX: %d) vertically.\n", colHead->origY, colHead->origX, colTail->origY, colTail->origX);
    }

    if (NULL != prevAtThisY) {
      prevAtThisY->r = colHead; 
      colHead->l = prevAtThisY;
      if (DEBUGGING) {
        printf("\t\tConnected colHeads (origY: %d, origX: %d) <-> (origY: %d, origX: %d) horizontally.\n", prevAtThisY->origY, prevAtThisY->origX, colHead->origY, colHead->origX);
      }
    } 
    prevAtThisY = colHead;
  }
  grandHead->r = colHeads.begin()->second;
  colHeads.begin()->second->l = grandHead;

  if (DEBUGGING) {
    printf("\tbuildLinks, milestone#2\n");
  }

  if (DEBUGGING) {
    printf("\t\tConnected grandHead(origY: %d, origX: %d) <-> colHeads.begin(origY: %d, origX: %d) horizontally.\n", grandHead->origY, grandHead->origX, colHeads.begin()->second->origY, colHeads.begin()->second->origX);
  }

  // connect cyclic edges for "grandHead" (horizontally)
  colHeads.rbegin()->second->r = grandHead;
  grandHead->l = colHeads.rbegin()->second;
  if (DEBUGGING) {
    printf("\t\tConnected grandHead(origY: %d, origX: %d) <-> colHeads.rbegin(origY: %d, origX: %d) horizontally.\n", grandHead->origY, grandHead->origX, colHeads.rbegin()->second->origY, colHeads.rbegin()->second->origX);
  }

  if (DEBUGGING) {
    printf("\tbuildLinks, milestone#3\n");
  }

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

void search(int k, DcLinkNode* grandHead, list<int> &selectedYs, map<int, DcLinkNode*> &colHeads, int &coveredColCount, int &xUpper, bool &found) {
  if (true == found) {
    return;
  }
  if (DEBUGGING) {
    printf("search, k:%d\n", k);
  }

  DcLinkNode* c = grandHead->r;
  if (c == grandHead) {
    if (DEBUGGING) {
      printf("\tgot c(origY: %d, origX: %d) == grandHead(origY: %d, origX: %d)\n", c->origY, c->origX, grandHead->origY, grandHead->origX);
    }
    if (coveredColCount == xUpper) {
      found = true;
    }
    return;
  }

  if (DEBUGGING) {
    printf("\tchosen c(origY: %d, origX: %d)\n", c->origY, c->origX);
  }
  cover(c);
  ++coveredColCount;
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
      ++coveredColCount;
      itr2 = itr2->r; 
    }
    search(k+1, grandHead, selectedYs, colHeads, coveredColCount, xUpper, found);
    if (true == found) {
      return;
    }
    // backtracking#1
    selectedYs.pop_back();
    itr2 = itr1->l;
    while (itr2 != itr1) {
      DcLinkNode* colHead = colHeads[itr2->origX];
      uncover(colHead);
      --coveredColCount;
      itr2 = itr2->l; 
    }
    
    // move on to the next candidate
    itr1 = itr1->d;
  }
  // backtracking#2
  uncover(c);
  --coveredColCount;
}

int main() {
  freopen ("test_cases.txt","r",stdin);
  int yUpper = 0, xUpper = 0;
  while (scanf("%d %d", &yUpper, &xUpper) != EOF) {
    vector<vector<int>> adj;
    for (int i = 0; i < yUpper; ++i) {
      int xOfOnesCount;
      vector<int> xOfOnes;
      scanf("%d", &xOfOnesCount);
      for (int j = 0; j < xOfOnesCount; ++j) {
        int col = 0;
        scanf("%d", &col); 
        --col;
        xOfOnes.push_back(col);
      }
      adj.push_back(xOfOnes);
    }

    map<int, DcLinkNode*> colHeads; // ordered for traversal
    DcLinkNode* grandHead = buildLinks(adj, colHeads);

    int k = 0, coveredColCount = 0;
    list<int> selectedYs;
    bool found = false;
    search(k, grandHead, selectedYs, colHeads, coveredColCount, xUpper, found);
    if (false == found || coveredColCount < xUpper) {
      printf("NO\n");
    } else {
      printf("%d ", selectedYs.size());
      for (auto &selectedY : selectedYs) {
        printf("%d ", selectedY+1);
      }
      printf("\n");
    }
  }

  return 0;
}
