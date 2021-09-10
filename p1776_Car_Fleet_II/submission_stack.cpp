bool debug = false;

/*
test cases
[[1,2],[2,1],[4,3],[7,2]]
[[1,2],[2,2],[4,2],[7,2]]
[[3,4],[5,4],[6,3],[9,1]]
[[1,4],[2,4],[50,4],[51,3],[52,2]]
[[1,4],[4,5],[7,1],[13,4],[14,3],[15,2],[16,5],[19,1]]
[[6,3],[8,5],[13,3],[18,2]]
[[6,3],[8,5],[13,3],[18,2],[19,3],[20,5]]
*/
class Solution {
public:
    vector<double> getCollisionTimes(vector<vector<int>>& cars) {
        /*
        The key idea is to avoid simulation which can be quite complicated in details for debugging. 
        
        For any "cars[i]", if it would eventually catch a fleet (i.e. of several cars joined) in front of it, then there MUST BE a "repIdx = firstCatchTimeRepresentative(cars[i])" such that "firstCatchTime(cars[i]) == catchTime(cars[i], cars[repIdx])" where the latter is AS IF there were no car between "cars[i], ..., cars[repIdx]" and of course "repIdx > i" and "cars[i][1] > cars[repIdx][1]".

        Consider the sequence of car speeds
        [5, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1]
         ^

        The "cars[repIdx]" for "cars[0]: speed == 5" might have speed of any value "< 5", e.g. in [4, 3, 2, 1], but the precise value depends on initial position as well.
        */
        int n = cars.size();
        vector<double> ans(n, -1.0);

        stack<int> stk; // If (false == stk.empty()), we guaranteed that "cars[onstackIdx]" has decreasing speeds from top down.
        for (int i = n-1; i >= 0; --i) {
            auto &currentCar = cars[i];
            while (!stk.empty()) {
                int repIdx = stk.top(); 
                auto &nearestFleetRepCand = cars[repIdx];

                bool tooFast1 = currentCar[1] <= nearestFleetRepCand[1]; 
                bool tooFast2 = ans[repIdx] > 0 && (double)(nearestFleetRepCand[0] - currentCar[0])/(currentCar[1] - nearestFleetRepCand[1]) >= ans[repIdx]; 
                if (tooFast1 || tooFast2) {
                    /*
                    This "nearestFleetRepCand" is too fast such that even if "currentCar" would join the same fleet, it would be strictly later than the joining of "nearestFleetRepCand".

                    Moreover, such "nearestFleetRepCand" has no chance to be a candidate for any "cars[h < i]" again, because either "cars[i]" or the "nearestRep of this nearestFleetRepCand" can be a better candidate for "cars[h]".
                    */
                    stk.pop();
                } else {
                    break;
                }
            }


            if (!stk.empty()) {
                /*
                Now in the rest of the stack are all valid candidates, but only the top is the correct representative for the fleet that "cars[i]" will join first, because 
- the top is pushed last hence nearest to the current "cars[i]", and 
- the top is guaranteed NOT to collide with the second-top earlier than colliding with "cars[i]" by the just ended traversal.
                */

                // This is valid for "ans[repIdx] == -1.0" too.
                int repIdx = stk.top();
                auto &nearestFleetRep = cars[repIdx];
                ans[i] = (double)(nearestFleetRep[0] - currentCar[0]) / (currentCar[1] - nearestFleetRep[1]);
            }

            stk.push(i);
        }
        return ans;
    }
};

