#include<bits/stdc++.h>
using namespace std;
#define PI acos(-1)
const double INF=1e4;
const double EPS=1e-10;
struct Point
{
    double x,y;

    Point() {}
    Point(double x, double y):x(x),y(y) {}
    Point(const Point &p): x(p.x),y(p.y) {}

    void input()
    {
        scanf("%lf%lf",&x,&y);
    }

    Point operator + (const Point &p) const
    {
        return Point(x+p.x, y+p.y);
    }

    Point operator - (const Point &p) const
    {
        return Point(x-p.x, y-p.y);
    }

    Point operator * (double c) const
    {
        return Point(x*c, y*c);
    }

    Point operator / (double c) const
    {
        return Point(x/c, y/c);
    }
};
vector<Point>polygon;
double getClockwiseAngle(Point p)
{
    return -1 * atan2(p.x, -1 * p.y);
}
//compare function to compare clockwise
bool comparePoints(Point p1, Point p2)
{
    return getClockwiseAngle(p1) < getClockwiseAngle(p2);
}
// rotate 90 degree counter clockwise
Point RotateCCW90(Point p)
{
    return Point(-p.y,p.x);
}
// rotate 90 degree clockwise
Point RotateCW90(Point p)
{
    return Point(p.y,-p.x);
}
Point RotateCCW(Point p, double t)
{
    return Point(p.x*cos(t)-p.y*sin(t),p.x*sin(t)+p.y*cos(t));
}
Point RotateCW(Point p, double t)
{
    return Point(p.x*cos(t)+p.y*sin(t),-p.x*sin(t)+p.y*cos(t));
}
double dot(Point A, Point B)
{
    return A.x*B.x+A.y*B.y;
}
double cross(Point A, Point B)
{
    return A.x*B.y-A.y*B.x;
}
double dist2(Point A, Point B)
{
    return dot(A-B,A-B);
}
// returns distance between two point
double dist(Point A, Point B)
{
    return sqrt(dot(A-B,A-B));
}
// Distance between point A and B
double distBetweenPoint(Point A, Point B)
{
    return sqrt(dot(A-B,A-B));
}
// project point c onto line AB (A!=B)
Point ProjectPointLine(Point A, Point B, Point C)
{
    return A+(B-A)*dot(C-A,B-A)/dot(B-A,B-A);
}
// Determine if Line AB and CD are parallel or collinear
bool LinesParallel(Point A, Point B, Point C, Point D)
{
    return fabs(cross(B-A,C-D))<EPS;
}
// Determine if Line AB and CD are collinear
bool LinesCollinear(Point A, Point B, Point C, Point D)
{
    return LinesParallel(A,B,C,D) && fabs(cross(A-B,A-C))<EPS && fabs(cross(C-D,C-A))<EPS;
}
//checks if AB intersect with CD
bool SegmentIntersect(Point A, Point B, Point C, Point D)
{
    if(LinesCollinear(A,B,C,D))
    {
        if(dist2(A,C)<EPS || dist2(A,D)<EPS || dist2(B,C)<EPS || dist2(B,D)<EPS)
            return true;
        if(dot(C-A,C-B) > 0 && dot(D-A,D-B) > 0 && dot(C-B,D-B) > 0)
            return false;
        return true;
    }
    if(cross(D-A,B-A) * cross(C-A,B-A) > 0)
        return false;
    if(cross(A-C,D-C) * cross(B-C,D-C) > 0)
        return false;
    return true;
}
// Compute the coordinates where AB and CD intersect
Point ComputeLineIntersection(Point A, Point B, Point C, Point D)
{
    double a1,b1,c1,a2,b2,c2;
    a1=A.y-B.y;
    b1=B.x-A.x;
    c1=cross(A,B);
    a2=C.y-D.y;
    b2=D.x-C.x;
    c2=cross(C,D);
    double Dist=a1*b2-a2*b1;
    return Point((b1*c2-b2*c1)/Dist,(c1*a2-c2*a1)/Dist);
}
//Project point C onto line segment AB -- return the Point from AB which is the closest to C --
Point ProjectPointSegment(Point A, Point B, Point C)
{
    double r=dot(B-A,B-A);
    if(fabs(r)<EPS)
        return A;
    r=dot(C-A,B-A)/r;
    if(r<0)
        return A;
    if(r>1)
        return B;
    return A+(B-A)*r;
}
// return the minimum distance from a point C to a line AB
double DistancePointSegment(Point A, Point B, Point C)
{
    return distBetweenPoint(C,ProjectPointSegment(A,B,C));
}
// return distance between P and a point where p is perpendicular on AB. AB er upore p jei point e lombo shei point theke p er distance
double distToLine(Point p, Point a, Point b)
{
    pair<double,double>c;
    double scale=(double)(dot(p-a,b-a))/(dot(b-a,b-a));
    c.first=a.x+scale*(b.x-a.x);
    c.second=a.y+scale*(b.y-a.y);
    double dx=(double)p.x-c.first,dy=(double)p.y-c.second;
    return sqrt(dx*dx+dy*dy);
}
int orientation(Point p, Point q, Point r)
{
    int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

    if (val == 0)
        return 0;  // colinear
    return (val > 0)? 1: 2; // clock or counterclock wise
}
// Given three colinear points p, q, r, the function checks if
// point q lies on line segment 'pr'
bool onSegment(Point p, Point q, Point r)
{
    if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) && q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
        return true;
    return false;
}
//checks if Point P is inside of polygon or not
bool isInside(int n, Point p)
{
    if (n < 3)
        return false;
    Point extreme = Point(INF, p.y); // here INF=1e4
    int count = 0, i = 0;
    do
    {
        int next = (i+1)%n;
        if (SegmentIntersect(polygon[i], polygon[next], p, extreme))
        {
            if (orientation(polygon[i], p, polygon[next]) == 0)
                return onSegment(polygon[i], p, polygon[next]);
            count++;
        }
        i = next;
    }
    while (i != 0);
    return count&1;
}
// returns the perimeter of a polygon
double polygonPerimeter(int n)
{
    double perimeter = 0.0;
    for (int i = 0; i < n - 1; i++) //polygon vector holds the corner points of the given polygon
        perimeter += dist(polygon[i], polygon[i + 1]);
    perimeter += dist(polygon[0], polygon[n - 1]);
    return perimeter;
}
//returns the area of a polygon
double polygonArea(int n)
{
    double area = 0.0;
    int j = n - 1;
    for (int i = 0; i < n; i++)
    {
        area += (polygon[j].x + polygon[i].x) * (polygon[j].y - polygon[i].y);
        j = i;
    }
    return abs(area);
}
double getTriangleArea(Point a, Point b, Point c)
{
    return fabs(cross(b - a, c - a));
}
int compareConvex(const void *vp1, const void *vp2)
{
   Point *p1 = (Point *)vp1;
   Point *p2 = (Point *)vp2;

   // Find orientation
   int o = orientation(p0, *p1, *p2);
   if (o == 0)
     return (dist(p0, *p2) >= dist(p0, *p1))? -1 : 1;

   return (o == 2)? -1: 1;
}
Point nextToTop(stack<Point> &S)
{
    Point p = S.top();
    S.pop();
    Point res = S.top();
    S.push(p);
    return res;
}
// make a minimum area polygon
stack<Point> convexHull(Point points[], int n)
{
   int ymin = points[0].y, min = 0;
   for (int i = 1; i < n; i++)
   {
     int y = points[i].y;
     if ((y < ymin) || (ymin == y &&
         points[i].x < points[min].x))
        ymin = points[i].y, min = i;
   }
   swap(points[0], points[min]);
   p0 = points[0]; // p0 is a global declared point
   qsort(&points[1], n-1, sizeof(Point), compareConvex);
   int m = 1;
   for (int i=1; i<n; i++)
   {
       while (i < n-1 && orientation(p0, points[i],points[i+1]) == 0)
          i++;
       points[m] = points[i];
       m++;
   }
   stack<Point> S;
   if (m < 3) return S;
   S.push(points[0]);
   S.push(points[1]);
   S.push(points[2]);
   for (int i = 3; i < m; i++)
   {
      while (orientation(nextToTop(S), S.top(), points[i]) != 2)
         S.pop();
      S.push(points[i]);
   }
   return S;
}
int main()
{

}