/*
test cases
1
2
5
9
15
1000
1000000
1000000000
*/
class Solution {
public:
    int consecutiveNumbersSum(int N) {
        if (1 == N) return 1;
        /*
        For any (k1, k2) where "k1 <= k2" && "N == (k2+k1)*(k2-k1+1)/2", it's equivalent that "2N == (k2+k1)*(k2-k1+1) == counterFactor*factor".
        */
        int ans = 0;
        int twiceN = (N << 1);
        for (int factor = 1; factor < N; ++factor) {
            if (twiceN % factor != 0) continue;
            int counterFactor = twiceN/factor;
            if (counterFactor < factor) break;
            
            /*
            k1+k2   == counterFactor
            k2-k1+1 == factor
            
            thus 
            
            2*k2+1 == factor+counterFactor
            */
            
            int twiceK2 = factor+counterFactor-1;
            if (twiceK2 % 2 != 0) continue;
            int k2 = twiceK2/2;
            int k1 = counterFactor-k2;
            if (k1 > k2) continue;
            ++ans;
        }
        
        return ans;
    }
};
