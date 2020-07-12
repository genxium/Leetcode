#define eps 1e-12

typedef struct point {
  double x,y;
} point;

double dis(point a,point b) {
  return sqrt(pow(a.x-b.x,2)+pow(a.y-b.y,2));
}

double getsum(int n,point p[],point g) {
  double sum=0;
  for(int i = 0; i < n; i++) {
    sum+=dis(g,p[i]);
  }
  return sum;
}

point p[51];
class Solution {
    public:
      double getMinDistSum(vector<vector<int>>& positions) {
        int n = positions.size();
        point g;
        point f1,f2,f3,f4,f5,f6,f7,f8;
        double ans,t1,t2,t3,t4,t5,t6,t7,t8;
            
        g.x = g.y = 0;
        for (int i = 0; i < n; ++i) {
          p[i].x = (double)positions[i][0];
          p[i].y = (double)positions[i][1];
          g.x+=p[i].x;
          g.y+=p[i].y;
        }
        g.x /= n;
        g.y /= n;
        
        double step=100.0; 
        ans=getsum(n, p, g);

        while(step>eps)
        {
          double cand = getsum(n,p,g);  
          if(cand<ans) {ans=cand;}
         
          f1.x=g.x;f1.y=g.y+step;
          f2.x=g.x;f2.y=g.y-step;
          f3.x=g.x+step;f1.y=g.y;
          f4.x=g.x-step;f4.y=g.y;
          f5.x=g.x-step;f5.y=g.y-step;
          f6.x=g.x-step;f6.y=g.y+step;
          f7.x=g.x+step;f7.y=g.y-step;
          f8.x=g.x+step;f8.y=g.y+step;
            
          t1=getsum(n,p,f1);
          t2=getsum(n,p,f2);
          t3=getsum(n,p,f3);
          t4=getsum(n,p,f4);
          t5=getsum(n,p,f5);
          t6=getsum(n,p,f6);
          t7=getsum(n,p,f7);
          t8=getsum(n,p,f8);
          if(t1<ans) {g=f1;ans=t1;}
          if(t2<ans) {g=f2;ans=t2;}
          if(t3<ans) {g=f3;ans=t3;}
          if(t4<ans) {g=f4;ans=t4;}
          if(t5<ans) {g=f5;ans=t5;}
          if(t6<ans) {g=f6;ans=t6;}
          if(t7<ans) {g=f7;ans=t7;}
          if(t8<ans) {g=f8;ans=t8;}
          //printf("now== %.2f\n",ans);
          step *= 0.95;
        }
        return ans;
      }
  };
