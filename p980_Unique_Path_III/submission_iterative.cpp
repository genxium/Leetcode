#define MAXN 101

class Node {
  public:
    int x, y;    
    int uniquePathCount;
    Node() {
      this->x = -1;
      this->y = -1;
      this->uniquePathCount = 0;
    }
    Node(int aX, int aY) {
      this->x = aX;
      this->y = aY;
      this->uniquePathCount = 0;
    }
};

class NodeCompare {
  public: 
    int operator() (Node const &lhs, Node const &rhs) {
      if (lhs.x != rhs.x) {
        return lhs.x - rhs.x;
      } else {
        return lhs.y - rhs.y;
      }
    }
};

// TBD.
