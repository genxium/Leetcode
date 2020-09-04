/*
This problem only asks whether or not we can meet the required liter, not how to meet it. 

Both of my exhaustive search trials failed to pass, because they branched too much at each state. In fact we can just be greedy to simulate the "Extended Euclid GCD" process as follows.
```
HowTo#A
1. Fill the x litre jug and empty it into y liter jug. 
2. Whenever the x liter jug becomes empty fill it. [contributes to +1 x factor]
3. Whenever the y liter jug becomes full empty it. [contributes to -1 y factor]
4. Repeat steps 1, 2, 3 till either y liter jug or the x liter jug contains z litres of water.
```

or 

```
HowTo#B
1. Fill the y liter jug and empty it into x liter jug.
2. Whenever the y liter jug becomes empty fill it. [contributes to +1 y factor]
3. Whenever the x liter jug becomes full empty it. [contributes to -1 x factor]
4. Repeat steps 1, 2, 3 till either y liter jug or the x liter jug contains z liters of water.
```

Either "HowTo" will work because there're more than 1 pairs of {p, q} such that "p*x + q*y == z", as long as it's possible to make so and "p*q <= 0". Without loss of generality, assume that "x > y > 0", then "HowTo#A" will result in the following sequence.

```
HowTo#A step by step 
{0, 0} -> {x, 0} -> {r1, 0} -> {0, r1} -> {x, r1} -> {x-(y-r1), y} -> {x-(y-r1), 0}

where "r1 = x%y", note that "(x-(y-t*r1))%y = r1-0+t*r1 = (t+1)*r1" as long as "t*r1 < y", therefore the sequence goes on like 

{x-(y-r1), 0} -> {2*r1, 0} -> ... -> {t*r1, 0} -> {0, t*r1} -> {x, t*r1} -> {x-r2, y} -> {x-r2, 0}

where "r2 = y%r1 = y-t*r1", again note that "(x-r2)%y = r1-r2" because "r2 < r1 < y", the sequence goes on like

{x-r2, 0} -> {r1-r2, 0} -> {0, r1-r2} -> {x, r1-r2} -> {x-(y-(r1-r2)), y} -> {x-(y-(r1-r2)), 0}

again note that  "(x-(y-(r1-r2)))%y = r1-(0-(r1-r2)) = 2*r1-r2", the sequence goes on like

{x-(y-(r1-r2)), 0} -> {2*r1-r2, 0} -> {0, 2*r1-r2} -> {x, 2*r1-r2} -> {x-(y-(2*r1-r2)), y} -> {x-(y-(2*r1-r2)), 0} -> {3*r1-r2, 0}
```

See https://www.geeksforgeeks.org/two-water-jug-puzzle/#:~:text=Solution%201%20(Always%20pour%20from,contains%20d%20litres%20of%20water. for more information. 
*/

class Solution {
private:
    int gcd(int x, int y) {
        return y == 0 ? x : gcd(y, x % y);
    }
public:
    bool canMeasureWater(int x, int y, int z) {
        if (x < y) swap(x, y);
        if (0 == z) return true;
        int gcdVal = gcd(x, y);
        return (z <= x+y) && (gcdVal > 0 && z % gcdVal == 0);
    }
};
