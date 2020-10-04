#define ULL uint64_t
#define MAXN 1000
#define INVALID 0u
#define MOD 1000000007
#define PI pair<int, int>

ULL C[MAXN+1][MAXN+1]; // C[n][k] equals the number of ways to take k elements from n distinct elements, regardless of order

ULL combination(int n, int k) {
  if (n < 0 || k < 0) return 0;
  if (INVALID != C[n][k]) return C[n][k];
  if (k == n) return C[n][k] = 1u; // including {n: 0, k:0}
  if (k < n) {
    return C[n][k] = (combination(n-1, k-1) + combination(n-1, k))%MOD;
  }
  return INVALID;
}

int waysToInterleave(int x, int y) {
  // the number of ways to interleave a[0, ..., x-1] and b[0, ..., y-1] is equivalent ot the number of ways to pick x slots from x+y slots
  return combination(x+y, x);
}

void insertIntoBst(TreeNode* root, int num) {
  if (num > root->val) {
    if (root->right) insertIntoBst(root->right, num);
    else {
      auto newNode = new TreeNode(num);
      root->right = newNode;
    }
  } else if (num < root->val) {
    if (root->left) insertIntoBst(root->left, num);
    else {
      auto newNode = new TreeNode(num);
      root->left = newNode;
    }
  } else return;
}

PI solve(TreeNode* root) {
  if (!root) {
    return {0, 1}; // 0 element, 1 degenerate
  }
  auto leftRes = solve(root->left);
  auto rightRes = solve(root->right);
  // specifically, either "leftRes.first" or "rightRes.first" could be 0
  int elementCount = 1+leftRes.first+rightRes.first;
  
  // for each pair of {leftDegenerateSequence, rightDegenerateSequence}, we shall interleave them to form new degenerate
  ULL degenerateCount = waysToInterleave(leftRes.first, rightRes.first)*(((ULL)leftRes.second*(ULL)rightRes.second)%MOD); 
  degenerateCount %= MOD;
  return {elementCount, (int)degenerateCount};
}

/*
test case#1
[1]

test case#2
[9,4,2,1,3,6,5,7,8,14,11,10,12,13,16,15,17,18]

test case#3
[10,23,12,18,4,29,2,8,41,31,25,21,14,35,26,5,19,43,22,37,9,20,44,28,1,39,30,38,36,6,13,16,27,17,34,7,15,3,11,24,42,33,40,32]
*/
class Solution {
public:
    int numOfWays(vector<int>& nums) {
      int n = nums.size();
      if (0 == n) return 0;
      if (1 == n) return 0;
      memset(C, INVALID, sizeof(C));
      auto root = new TreeNode(nums[0]);
      for (int i = 1; i < n; ++i) {
        insertIntoBst(root, nums[i]);
      }
      auto res = solve(root);
      return res.second-1;
    }
};
