class RowFit {
public:
  vector<int> payload;
  set<int> fitsRows;
  RowFit *prev = NULL, *next = NULL;

  bool fitsTheRow(int rowOffset) {
    return !(fitsRows.find(rowOffset) == fitsRows.end());
  }
};

void cover(RowFit* curr) {
  if (curr->prev) {
    curr->prev->next = curr->next;
  }
  if (curr->next) {
    curr->next->prev = curr->prev;
  }
}

void uncover(RowFit* curr) {
  if (curr->prev) {
    curr->prev->next = curr;
  }
  if (curr->next) {
    curr->next->prev = curr;
  }  
}

void recurse(int rowOffset, int nRows, int byFarSwapCnt, RowFit *grandHead, int &ans) {
  /*
  This is an easy tweak of "DancingLinks", a trick for "backtracking search w.r.t. LinkedList", see https://www.yinxiang.com/everhub/note/e71e7e87-ca06-40e5-a880-c9517a9afd33 for more information.
  */
  //printf("recurse, rowOffset:%d, byFarSwapCnt:%d, ans:%d\n", rowOffset, byFarSwapCnt, ans);
  if (byFarSwapCnt > ans) return;
  if (rowOffset == nRows) {
    ans = min(ans, byFarSwapCnt);
    return;
  } 
  int swapCntInc = 0;
  RowFit *trav = grandHead->next;
  while (NULL != trav) {
    if (trav->fitsTheRow(rowOffset)) {
      cover(trav);
      recurse(rowOffset+1, nRows, byFarSwapCnt+swapCntInc, grandHead, ans);
      uncover(trav);
    }
    trav = trav->next;
    ++swapCntInc;
  }
}

class Solution {
public:
    int minSwaps(vector<vector<int>>& grid) {
        int yUpper = grid.size();
        if (0 == yUpper) return -1;
        int xUpper = grid[0].size();

        vector<RowFit*> rowFits; 
        RowFit *grandHead = new RowFit(); // the "grandHead" contains no data
        RowFit *tail = grandHead;
        for (int y = 0; y < yUpper; ++y) {
            auto row = grid[y];
            int cnt = 0; 
            for (auto it = row.rbegin(); it != row.rend(); ++it) {
                if (*it == 1) break;
                ++cnt;
            }
            /*
            "cnt == 0" fits only row index "n-1"
            "cnt == 1" fits row indices "n-1, n-2"
            "cnt == 2" fits row indices "n-1, n-2, n-3"
            ...
            */
            RowFit *single = new RowFit();
            single->payload = row;
            for (int i = yUpper-1; i >= yUpper-1-cnt; --i) {
                single->fitsRows.insert(i);
                //printf("The orig %d-th row fits final %d-th row.\n", y, i);
            }
            tail->next = single;
            single->prev = tail;
            tail = single;
        }
        int ans = INT_MAX; 
        recurse(0, yUpper, 0, grandHead, ans);
        if (INT_MAX == ans) {
            return -1;
        } else {
            return ans;            
        }
    }
};
