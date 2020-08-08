class RowFit {
public:
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

void searchIteratively(RowFit *grandHead, int &ans) {
  int rowOffset = 0;
  while (true) {
    int swapCntInc = 0;
    RowFit *trav = grandHead->next;
    if (NULL == trav) break;
    while (NULL != trav && false == trav->fitsTheRow(rowOffset)) {
      trav = trav->next;
      ++swapCntInc;
    }
    if (NULL == trav) {
      ans = -1;
      return;
    }
    // now that "true == trav->fitsTheRow(rowOffset))"
    cover(trav);
    ans += swapCntInc;
    ++rowOffset;
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
            for (int i = yUpper-1; i >= yUpper-1-cnt; --i) {
                single->fitsRows.insert(i);
                //printf("The orig %d-th row fits final %d-th row.\n", y, i);
            }
            tail->next = single;
            single->prev = tail;
            tail = single;
        }
        int ans = 0; 
        searchIteratively(grandHead, ans);
        
        return ans;
    }
};
