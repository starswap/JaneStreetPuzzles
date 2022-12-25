#include <bits/stdc++.h>
using namespace std;

const double EPS = 1e-10; // For FP equality check 
const double PI  = 3.141592653589793238462643383279502884;
const double PHI = 1.61803398875; // Golden Ratio
const double INF = 1e9;
const int N_PENTS = 4;


template <class T> 
ostream& operator << (ostream& o, vector<T> v) {
    o << '[';
    for (auto x : v) {
        o << x << ",";
    }
    o << ']';
    return o;
}

inline double sinD(double deg) {
    return sin(deg / 180 * PI);
}

inline double cosD(double deg) {
    return cos(deg / 180 * PI);
}

inline double atanD(double x) {
    return 180 * atan(x) / PI;
}

struct Vec {
    double dx;
    double dy;
    
    Vec(double dx, double dy):dx(dx),dy(dy) {}

    double norm() {
        return sqrt(dx*dx+dy*dy);
    }

    Vec operator / (double x)   {
        return Vec(dx/x,dy/x);
    }

    Vec operator * (double x)   {
        return Vec(dx*x,dy*x);
    }
};

double dot(Vec a, Vec b) {
    return (a.dx*b.dx) + (a.dy*b.dy);
} 

class Point {
    public:
        double x,y;
        Point():x(0),y(0) {};
        Point(double x,double y):x(x),y(y) {};

        Point transform(Vec v) {
            return(Point(x + v.dx, y + v.dy)); 
        }
};

bool operator < (Point a, Point other) {
    return (a.x < other.x || -EPS < (a.x - other.x) < EPS && a.y < other.y);
}  

double dist(Point a, Point b) {
    return sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));
}

ostream& operator << (ostream& o, Point P) {
    o << "(" << P.x << "," << P.y << ")";
    return o;
}

class Line {
    public:
        Point a;
        Point b;
    
        Line(Point a, Point b):a(a),b(b) {};
};

class Pentagon {
    public:
        vector<Point> vertices;
        
        Pentagon (Point a, Point b, Point c, Point d, Point e):vertices({a,b,c,d,e}){};
        Pentagon (Point a, Point b, bool cw):vertices({a,b}) {
            fillIn(cw);
        }


    private:

        void fillIn(bool cw) {
            const Point &a = vertices[0];
            const Point &b = vertices[1];
            Point c,d,e;

            double dy = b.y-a.y;
            double dx = b.x-a.x;
            double theta;
            if (-EPS < dx && dx < EPS) {
                if (b.y > a.y)
                    theta = 90;
                else
                    theta = -90/2;
            }
            else {
                theta = atanD(dy/dx);
            }
            // cout << theta << endl;
            if (dx < 0 && dy < 0) {
                theta = -180+theta;
            }
            else if (dx < 0) {
                theta = 180+theta;
            }
                
            double a1 = theta + (cw ? -1 : 1)*36;
            c.x = a.x + PHI * cosD(a1);
            c.y = a.y + PHI * sinD(a1);
            double a2 = theta + (cw ? -1 : 1)*2*36;
            d.x = a.x + PHI * cosD(a2);
            d.y = a.y + PHI * sinD(a2);
            double a3 = theta + (cw ? -1 : 1)*3*36;
            e.x = a.x + cosD(a3);
            e.y = a.y + sinD(a3);

            vertices.push_back(c);
            vertices.push_back(d);
            vertices.push_back(e);
        }

    friend ostream& operator << (ostream& o, Pentagon p) {
        o << p.vertices[0] << endl;
        o << p.vertices[1] << endl;
        o << p.vertices[2] << endl;
        o << p.vertices[3] << endl;
        o << p.vertices[4] << endl;
        return o;
    }

};

double cornerToEdgeDistance(const Point& p, const Line& l) {
    Line l2(min(l.a,l.b),max(l.a,l.b));

    // (A -> P) . (A -> B)
    Vec AP(p.x - l.a.x, p.y - l.a.y);
    Vec AB(l.b.x - l.a.x, l.b.y - l.a.y);
    
    Point project = l2.a.transform(AB / AB.norm() * dot(AP,AB));
    if ((project.x > l2.a.x - EPS && project.x < l2.b.x + EPS && project.y > (min(l2.a.y,l2.b.y)) - EPS && project.y < (max(l2.a.y,l2.b.y) + EPS))) {}
    else if (fabs(project.x - l2.a.x) < fabs(project.x - l2.b.x)) {
        project = l2.a;
    }
    else {
        project = l2.b;
    }

    return dist(p, project);

    // Check if in line
    // if not use nearest endpoint

    // double dx = l.b.x - l.a.x;
    // double dy = l.b.y - l.a.y;
    
    // double pdx = p.x - l.a.x;
    // double pdy = p.y - l.a.y;

}

double pentDist(const Pentagon& p1, const Pentagon& p2) {
    // provided the lines are not parallel,
    // the minimum distance between them should be vertex to line, perpendicular.
    double minDist = INF;
    for (Point p : p1.vertices) {
        for (int i=0;i<5;++i) {
            minDist = min(cornerToEdgeDistance(p,Line(p2.vertices[i],p2.vertices[(i+1)%5])),minDist);
        }
    }
    return minDist;
}

vector<Pentagon> pents(1,Pentagon(Point(0,0),Point(-1/sqrt(2),-1/sqrt(2)),true));

double solve(int nPents) {
    if (nPents == 0) {
        double minDist = INF;
        for (int i = 0;i<N_PENTS;++i) {
            for (int j = 0;j<i;++j) {
                if (i != j) {
                    double dist = pentDist(pents[i],pents[j]);
                    if (dist > EPS)
                        minDist = min(minDist,dist);
                }
            }
        }
        return minDist;
    }
    else {
        double minDist = INF;        
        for (int startVertex=1;startVertex<5;++startVertex) { // will always be the first one that was taken.
            const Pentagon& prevPentagon = *pents.rbegin();
            pents.push_back(Pentagon(prevPentagon.vertices[startVertex],prevPentagon.vertices[(startVertex+1)%5],false));   
            minDist = min(minDist,solve(nPents - 1));
            pents.pop_back();
        }
        return minDist;
    }

}



int main() {

    pents.push_back(Pentagon(pents[0].vertices[0],pents[0].vertices[1],false));
    // cout << pents << endl;

    // pents[1].taken = 0;

    // cout << setprecision(10);
    cout << solve(N_PENTS-2) << endl;

    return 0;

    // won't need CW
}