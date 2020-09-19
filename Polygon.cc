#include "Polygon.hh"
#include <cmath>
#include <set>
#include <fstream>
#include <cassert>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <pngwriter.h>
#include <limits>
using namespace std;


/** Constructor. */
ConvexPolygon::ConvexPolygon (const vector<Point>& Pol, const double& R_color, const double& G_color, const double& B_color)
:   vertices(Pol),
    R(R_color),
    G(G_color),
    B(B_color)
{}


/** Object to sort the vector. */
struct Order {
    Point ll;
    bool operator()(const Point& p1, const Point& p2) const {
        if (abs(ll.get_x() - p1.get_x()) < 1e-12 and abs(ll.get_x() - p2.get_x()) < 1e-12) return p1.get_y() < p2.get_y();
        if (abs(ll.get_x() - p1.get_x()) < 1e-12) return true;
        if (abs(ll.get_x() - p2.get_x()) < 1e-12) return false;
        if (abs((p1.get_y() -  ll.get_y()) / (p1.get_x() -  ll.get_x()) - (p2.get_y() -  ll.get_y()) / (p2.get_x() -  ll.get_x())) < 1e-12) return p1.get_x() < p2.get_x();
        else return (p1.get_y() -  ll.get_y()) / (p1.get_x() -  ll.get_x()) > (p2.get_y() -  ll.get_y()) / (p2.get_x() -  ll.get_x());
    }
};


/** Sorts the vector of vertices with the lower left vertice as reference. */
void simple_polygon (vector<Point>& p){
    int n = p.size();
    for (int i = 1; i < n; ++i) if ((p[i].get_x() < p[0].get_x()) or (abs(p[i].get_x() - p[0].get_x()) < 1e-12 and p[i].get_y() < p[0].get_y())) swap(p[0], p[i]);
    Order order;
    order.ll = p[0];
    sort(p.begin() + 1, p.end(), order);
    vector<Point> q;
    q.push_back(p[0]);
    for (int i = 1; i < n; ++i) if (p[i] != p[i-1]) q.push_back(p[i]);
    p = q;
}


/** Returns true if the angle formed by the three points is bigger than 180º. */
bool left_of(const Point a, const Point b, const Point c){
    return (b.get_x() - a.get_x()) * (c.get_y() - a.get_y()) >= (b.get_y() - a.get_y()) * (c.get_x() - a.get_x());
}


/** This polygon is updated with the vertices in the convex hull. */
ConvexPolygon& ConvexPolygon::convex_hull () {
    if (not vertices.empty()) {
        simple_polygon(vertices);
        if (vertices.size() > 2) {
            vector<Point> q;
            q.push_back(vertices[0]);
            int i = 1, n = vertices.size();
            while (i+1 < n and ((abs(vertices[0].get_x() - vertices[i].get_x()) < 1e-12 and abs(vertices[0].get_x() - vertices[i+1].get_x()) < 1e-12) or
                                 abs(vertices[0].slope(vertices[i]) - vertices[0].slope(vertices[i+1])) < 1e-12)) ++i;
            q.push_back(vertices[i]);
            if (not (i+1 == n)) {
                q.push_back(vertices[++i]);
                int m = 2;
                for (int j = i+1; j < n; ++j) {
                    while (left_of(q[m-1], q[m], vertices[j])) {
                        --m;
                        q.pop_back();
                    }
                    ++m;
                    q.push_back(vertices[j]);
                }
            }
            vertices = q;
        }
    }
    return *this;
}


/** Introduces the vertices in this polygon. */
ConvexPolygon& ConvexPolygon::new_pol (istringstream& iss){
    double x, y;
    while (iss >> x >> y) vertices.push_back(Point(x, y));
    return this->convex_hull();
}


/** Returns a string with the name of this polygon and the coordinates of his vertices in the convex hull. */
string ConvexPolygon::print () const {
    ostringstream oss;
    oss.setf(ios::fixed);
    oss.precision(3);
    for (Point i : vertices) oss << " " << i.get_x() << " " << i.get_y();
    return oss.str();
}


/** Returns the vector of vertices of this polygon. */
vector<Point> ConvexPolygon::get_vector() const{
    return vertices;
}


/** Returns the area of this polygon. */
double ConvexPolygon::area () const {
    if (vertices.size() < 3) return 0.0;
    double area = 0;
    int j = vertices.size() - 1;
    int n = vertices.size();
    for (int i = 0; i < n; ++i){
        area += (vertices[j].get_x() + vertices[i].get_x()) * (vertices[j].get_y() - vertices[i].get_y());
        j = i;
    }
    return abs(area / 2.0);
}


/** Returns the perimeter of this polygon. */
double ConvexPolygon::perimeter () const {
    double per = 0;
    int n = vertices.size() - 1;
    if (vertices.size() > 2){
        for (int i = 0; i < n; ++i) per += vertices[i].distance(vertices[i+1]);
    }
    if (vertices.size() > 1) per += vertices[0].distance(vertices[n]);
    return per;
}


/** Returns the number of vertices of the convex hull of this polygon. */
int ConvexPolygon::num_vertices () const {
    int v = vertices.size();
    return v;
}


/** Returns a point with the coordinates of the centroid of this polygon. */
Point ConvexPolygon::centroid () const {
    double x = 0, y = 0;
    for (Point p : vertices){
        x += p.get_x();
        y += p.get_y();
    }
    double i = vertices.size();
    return Point(x/i, y/i);
}


/** Saves the given polygons in a file, overwrites it if it already existed. */
void ConvexPolygon::save (ofstream& outdata) const {
    outdata.setf(ios::fixed);
    outdata.precision(3);
    for(Point v : vertices) outdata << " " << v.get_x() << " " << v.get_y();
    outdata << endl;
}


/** Associates a color to this polygon. */
void ConvexPolygon::setcol (istringstream& iss){
    double r, g, b;
    iss >> r >> g >> b;
    if (r > 1 or r < 0 or g > 1 or g < 0 or b > 1 or b < 0) {
        cout << "error: invalid color" << endl;
        return;
    }
    R = r;
    G = g;
    B = b;
    cout << "ok" << endl;
}


/** Returns a double with the size of the drawing. */
double ConvexPolygon::size_drawing() const{
    if (vertices.empty()){
        cout << "error: the polygon has no vertices" << endl;
        return 0;
    }
    if (vertices[2].get_x()-vertices[0].get_x() > vertices[2].get_y()-vertices[0].get_y()) return (vertices[2].get_x()-vertices[0].get_x());
    return (vertices[2].get_y()-vertices[0].get_y());
}


/** Plots this polygons and save it on a png file. */
void ConvexPolygon::draw(pngwriter& png, const ConvexPolygon& bbox, const double scale) const{
    if (not vertices.empty()){
        int xpos = (vertices[0].get_x() - bbox.vertices[0].get_x()) * scale + 2;
        int ypos = (vertices[0].get_y() - bbox.vertices[0].get_y()) * scale + 2;
        png.filledcircle(xpos, ypos, 2, R, G, B);
        if (vertices.size() > 1){
            int n = vertices.size();
            for (int i = 1; i < n; ++i){
                int nxpos = (vertices[i].get_x() - bbox.vertices[0].get_x()) * scale + 2;
                int nypos = (vertices[i].get_y() - bbox.vertices[0].get_y()) * scale + 2;
                png.line(xpos, ypos, nxpos, nypos, R, G, B);
                xpos = nxpos;
                ypos = nypos;
                png.filledcircle(xpos, ypos, 2, R, G, B);
            }
            png.line(xpos, ypos, (vertices[0].get_x() - bbox.vertices[0].get_x()) * scale + 2, (vertices[0].get_y() - bbox.vertices[0].get_y()) * scale + 2, R, G, B);
        }
    }
}


bool SameSide(const Point& A, const Point& B, const Point& P, const Point& Q){
    double dx = P.get_x() - Q.get_x();
    double dy = P.get_y() - Q.get_y();
    double dxA = A.get_x() - P.get_x();
    double dyA = A.get_y() - P.get_y();
    double dxB = B.get_x() - P.get_x();
    double dyB = B.get_y() - P.get_y();
    if (abs(dy * dxA - dx * dyA) < 1e-12) return true;
    return (dy * dxA - dx * dyA) * (dy * dxB - dx * dyB) >= 0 or abs((dy * dxA - dx * dyA) * (dy * dxB - dx * dyB)) < 1e-12;
}


/** Checks if the points are aligned */
bool aligned(const Point& A, const Point& B, const Point& C){
    return (A.get_x()*(C.get_y()-B.get_y())+B.get_y()*(C.get_x()-B.get_x())-(C.get_y()-B.get_y())*B.get_x() == A.get_y()*(C.get_x()-B.get_x())
            and (A.get_x() <= B.get_x() or A.get_x() <= C.get_x()) and ((A.get_x() >= B.get_x() or A.get_x() >= C.get_x()))
            and (A.get_y() <= B.get_y() or A.get_y() <= C.get_y()) and (A.get_y() >= B.get_y() or A.get_y() >= C.get_y()));
}


bool Intersect(const Point& A, const Point& B, const Point& C, const Point D, bool x){
    if (aligned(A, C, D)) return true;
    return not (SameSide(A, B, C, D) or SameSide(C, D, A, B));
}


/** Check if a point is inside a polygon. */
bool ConvexPolygon::point_inside(const Point& A) const{
    if (vertices.empty()) return false;
    if (vertices.size() == 1) return (vertices[0] == A);
    if (vertices.size() == 2) return aligned(A, vertices[0], vertices[1]);
    ConvexPolygon bbox;
    bbox.bbox(*this);
    if (bbox.vertices[0].get_x() > A.get_x()) return false;
    Point B = Point (numeric_limits<double>::infinity(), A.get_y());
    bool inside = false;
    int previ = vertices.size()-1;
    Point max = vertices[0], min = vertices[0];
    int cmax = 0, cmin = 0;
    int n = vertices.size();
    for(int i = 0; i < n; ++i){
        if (aligned(A, vertices[previ], vertices[i])) return true;
        if (Intersect (A, B, vertices[previ], vertices[i], 0)) inside = not inside;
        if (A.get_x() - vertices[i].get_x() < 1e-12 and abs(A.get_y() - vertices[i].get_y()) < 1e-12) inside = not inside;
        previ = i;
        if (vertices[i].get_y() > max.get_y()) max = vertices[i];
        else if (abs(vertices[i].get_y() - max.get_y()) < 1e-12) ++cmax;
        else if (vertices[i].get_y() < min.get_y()) min = vertices[i];
        else if (abs(vertices[i].get_y() - min.get_y()) < 1e-12) ++cmin;
    }
    if (abs(A.get_y() - max.get_y()) < 1e-12 and A.get_x() < max.get_x()) for (int i = 0; i < cmax; ++i) inside = not inside;
    else if (abs(A.get_y() - min.get_y()) < 1e-12 and A.get_x() < min.get_x()) for (int i = 0; i < cmin; ++i) inside = not inside;
    return inside;
}


/** Finds the point where two edges intersect. */
Point find_point(const Point& A, const Point& B, const Point& C, const Point& D){
    double xAB = B.get_x() - A.get_x();
    double yAB = B.get_y() - A.get_y();
    double xCD = D.get_x() - C.get_x();
    double yCD = D.get_y() - C.get_y();
    double x = (C.get_y() * xCD * xAB - yCD * xAB * C.get_x() - (A.get_y() * xAB * xCD - yAB * xCD * A.get_x())) / (yAB * xCD - yCD * xAB);
    if (abs(xAB) < 1e-12) return Point(x, yCD / xCD * x + C.get_y() - yCD / xCD * C.get_x());
    return Point(x, yAB / xAB * x + A.get_y() - yAB / xAB * A.get_x());
}


/** This polygon is updated to the intersection of the original polygon and p */
ConvexPolygon& ConvexPolygon::intersection (const ConvexPolygon& pol){
    vector<Point> v;
    if (vertices.size() > 1 and pol.vertices.size() > 1){
        int n = vertices.size(), previ = vertices.size() - 1;
        for (int i = 0; i < n; ++i){
            int  m = pol.vertices.size(), prevj = pol.vertices.size() - 1;
            for (int j = 0; j < m; ++j){
                if (Intersect(vertices[previ], vertices[i], pol.vertices[prevj], pol.vertices[j], 1)) {
                    if (not aligned(vertices[previ], pol.vertices[prevj], pol.vertices[j])) v.push_back(find_point(vertices[previ], vertices[i], pol.vertices[prevj], pol.vertices[j]));
                }
                prevj = j;
            }
            previ = i;
        }
        for (int i = 0; i < n; ++i) if (pol.point_inside(vertices[i])) v.push_back(vertices[i]);
        n = pol.vertices.size();
        for (int i = 0; i < n; ++i) if (this->point_inside(pol.vertices[i])) v.push_back(pol.vertices[i]);
    }
    if (vertices.size() == 1 and pol.point_inside(vertices[0])) v.push_back(vertices[0]);
    if (pol.vertices.size() == 1 and this->point_inside(pol.vertices[0])) v.push_back(pol.vertices[0]);
    vertices = v;
    this->convex_hull();
    return *this;
}


//Done
/** This polygon is updated to the union of the original polygon and p */
ConvexPolygon& ConvexPolygon::p_union (const ConvexPolygon& p){
    vector<Point> v = vertices;
    for (Point p : p.vertices) v.push_back(p);
    vertices = v;
    this->convex_hull();
    return *this;
}


//Done
/** Check if this polygon is inside the second polygon */
bool ConvexPolygon::inside (const ConvexPolygon& pol) const{
    if (vertices.empty() or pol.vertices.empty()) return false;
    int n = vertices.size();
    for (int i = 0; i < n; ++i) if (not pol.point_inside(vertices[i])) return false;
    return true;
}


//Done
/** Creates a new polygon with the four vertices corresponding to the bounding box of the given polygons */
ConvexPolygon& ConvexPolygon::bbox (const ConvexPolygon& pol){
    if (pol.vertices.empty()) {
        *this = pol;
        return *this;
    }
    double lx, hx, ly, hy;
    if (vertices.empty()){
        lx = pol.vertices[0].get_x();
        hx = lx;
        ly = pol.vertices[0].get_y();
        hy = ly;
    } else {
        lx = vertices[0].get_x();
        hx = vertices[2].get_x();
        ly = vertices[0].get_y();
        hy = vertices[2].get_y();
    }
    for (Point p : pol.vertices){
        if (p.get_x() < lx) lx = p.get_x();
        else if (p.get_x() > hx) hx = p.get_x();
        if (p.get_y() < ly) ly = p.get_y();
        else if (p.get_y() > hy) hy = p.get_y();
    }
    vertices = {Point(lx, ly), Point(lx, hy), Point(hx, hy), Point(hx, ly)};
    return *this;
}
