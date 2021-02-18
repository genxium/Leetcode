bool const debug = false;

bool cmp(int const &lhs, double const &rhs) {
    return (double)lhs < rhs;
}

class Solution {
private:
    int strictlySmallerCount(double x, vector<int>& arr, int &p, int &q) {
        // The "(p, q)" will hold the fraction (p/q >= x) and most close to x.
        int count = 0, n = arr.size();
        for (int i = 1; i < n; ++i) {
            // Each row consistis of {arr[0]/arr[i], arr[1]/arr[i], ..., arr[i-1]/arr[i]}.
            int denominator = arr[i];
            double target = x*denominator;
            auto it = lower_bound(arr.begin(), arr.begin()+i, target, &cmp); 
            // Now that "*it" is the first "*it >= target" in {arr[0], arr[1], ..., arr[i-1]}.
            int rowCnt = (it-arr.begin());
            if (debug) printf("Using denominator arr[i:%d]:%d, there're %d strictly smaller than target:%lf\n", i, arr[i], rowCnt, target);

            if (it != arr.end()) {
                if ((p == 0 && q == 0) || (*it)*q < p*arr[i]) {
                    // (*it/arr[i]) < (p/q) a closer (p, q)
                    p = *it;
                    q = arr[i];
                }
            }

            count += rowCnt;
        }

        if (debug) printf("There're %d fractions in the table strictly smaller than x:%lf, where the closest p/q >= x is (p: %d, q: %d)\n", count, x, p, q);
        return count;
    }

public:
    vector<int> kthSmallestPrimeFraction(vector<int>& arr, int k) {
        /*
             0    1    2   3   ...   k-1    k : numerator 
        0          
        1   0/1
        2   0/2  1/2
        3   0/3  1/3  2/3
        ..
        k   0/k  1/k  2/k 3/k  ... (k-1)/k
        :
        denominator      
        */
        double l = 0.0, r = 1.0;
        vector<int> ans = {0, 0};
        while (l < r) {
            int p = 0, q = 0;
            double mid = (l + r)*0.5;
            int cnt = strictlySmallerCount(mid, arr, p, q);
            if (cnt == k-1) {
                ans[0] = p, ans[1] = q;
                break;
            }
            if (cnt < k-1) {
                l = mid;
            } else {
                r = mid;
            }
        }

        return ans;
    }
};
