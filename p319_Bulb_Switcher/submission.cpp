class Solution {
public:
    int bulbSwitch(int n) {
      /*
      For each "k" in [1, ..., n], it's on after the n rounds iff it were toggled odd times.
      
      The k-th bulb is toggled "numOfFactors(k)" times whilst "numOfFactors(k) = product(q[i]+1)" where "q[i]" is the power of "p[i]" which is in turn a "PrimeFactor of k".
      
      Therefore k-th bulb is only on if every "q[i]" is even, which means that "k" should be a square number.
      */
      return floor(sqrt(n));
    }
};
