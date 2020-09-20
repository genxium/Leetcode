class Solution {
public:
    int naiveExhausting(int n) {
        list<int> survivors;
        for (int i = 1; i <= n; ++i) {
            survivors.push_back(i);
        }
        int direction = +1;
        while (survivors.size() > 1) {
            if (direction == +1) {
                auto it = survivors.begin();
                int step = 1;
                while (survivors.size() > 1 && it != survivors.end()) {
                    if (step == 0) {
                        ++it;
                        step = 1;
                    } else {
                        it = survivors.erase(it);
                        --step;                        
                    }
                }
            } else {
                auto rit = survivors.rbegin();
                int step = 1;
                while (survivors.size() > 1 && rit != survivors.rend()) {
                    if (step == 0) {
                        ++rit;
                        step = 1;
                    } else {
                        survivors.erase(next(rit).base());
                        --step;
                    }
                }
            }
            direction = -direction;
        }
        
        return *(survivors.begin());    
    }
    
    int solve(int n, int direction) {
        if (1 == n) {
            return 1;
        }
        if (2 == n) {
            if (+1 == direction) {
                return 2;
            } else {
                return 1;
            }
        }
        if (3 == n) {
            return 2;
        }
        if (+1 == direction) {
            /*
            * After elimination, only the even numbers are remained.
            * 
            * 1*[1, 2, 3, ...] => 2*[1, 2, 3, ...]
            * 
            */
            return 2*solve(n/2, -direction);    
        } else {
            if (n%2 == 1) {
                // only the even numbers are remained, same as "+1 == direction"
                return 2*solve(n/2, -direction);    
            } else {
                /*
                * only the odd numbers are remained
                * 1*[1, 2, 3, ...] => 2*[1, 2, 3, ...] - 1
                */
                return 2*solve(n/2, -direction)-1;    
            }
        }
    }
    
    int lastRemaining(int n) {
        int ans = solve(n, +1);
        return ans;
    }
};
