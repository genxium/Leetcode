class Solution {
private:
    vector<int> mutantAreaPrefixSums;
    vector<vector<int>> rects;
public:
    Solution(vector<vector<int>>& rects) {
      this->rects = rects;
      for (int i = 0; i < this->rects.size(); ++i) {
        vector<int> &rect = this->rects[i];
        int mutantArea = (rect[3]-rect[1]+1)*(rect[2]-rect[0]+1);
        mutantAreaPrefixSums.push_back(mutantArea + (mutantAreaPrefixSums.empty() ? 0 : mutantAreaPrefixSums.back()));
      }
    }
    
    vector<int> pick() {
      int n = this->rects.size();
      // int k = rand() % n; // This intuitive idea will fail to make the generated points as random as possible w.r.t. the area ratio of each rectangle.
      auto it = upper_bound(mutantAreaPrefixSums.begin(), mutantAreaPrefixSums.end(), rand()%mutantAreaPrefixSums.back());
      int k = it - mutantAreaPrefixSums.begin();
      vector<int> &targetRect = this->rects[k];
      
      int x = (rand() % (targetRect[2]-targetRect[0]+1) + targetRect[0]);
      int y = (rand() % (targetRect[3]-targetRect[1]+1) + targetRect[1]);
      
      return {x, y};
    }
};

/**
 * Your Solution object will be instantiated and called as such:
 * Solution* obj = new Solution(rects);
 * vector<int> param_1 = obj->pick();
 */
