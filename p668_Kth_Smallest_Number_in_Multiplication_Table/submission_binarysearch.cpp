class Solution {
private:
    bool enough(int x, int m, int n, int k) {
        int count = 0;
        for (int i = 1; i <= m; ++i) {
            // For each row consisting of {1*i, 2*i, ..., n*i}, x can be larger than min(x/i, n) elements
            count += min(x/i, n);
        }
        return count >= k;
    }
    
public:
    int findKthNumber(int m, int n, int k) {
        int l = 1, r = m*n;
        while (l < r) {
            int mid = ((l + r) >> 1);
            if (!enough(mid, m, n, k)) l = mid+1;
            else r = mid;
        }
        
        return l;
    }
};
