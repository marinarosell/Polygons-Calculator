#ifndef Polygon_hh
#define Polygon_hh


#include "Point.hh"
#include <vector>
#include <sstream>
#include <fstream>
#include <pngwriter.h>

using namespace std;


/** The Polygon class stores a vector if two dimensional points in the plane
    and provides some usefull operations for it.
*/


class ConvexPolygon {

public:

    /** Constructor: Creates a polygon with its vector of vertices, empty in default. */
    ConvexPolygon (const vector<Point>& Pol = {}, const double& R = 0.0, const double& G = 0.0, const double& B = 0.0);

    /** This polygon is updated with the vertices in the convex hull. */
    ConvexPolygon& convex_hull ();

    /** Introduces the vertices in this polygon. */
    ConvexPolygon& new_pol (istringstream& iss);

    /** Returns a string with the name of this polygon and the coordinates of all his vertices in the convex hull. */
    string print () const;

    /** Returns the vector of vertices of this polygon. */
    vector<Point> get_vector() const;

    /** Returns the area of this polygon. */
    double area () const;

    /** Returns the perimeter of this polygon. */
    double perimeter () const;

    /** Returns the number of vertices of the convex hull of this polygon. */
    int num_vertices () const;

    /** Returns a point with the coordinates of the centroid of this polygon. */
    Point centroid () const;

    /** Saves the given polygons in a file, overwrites it if it already existed. */
    void save (ofstream& outdata) const;

    /** Associates a colot to this polygon. */
    void setcol (istringstream& iss);

    /** Returns a double with the size of the drawing */
    double size_drawing() const;

    /** Plots this polygons and save it on a png file. */
    void draw(pngwriter& png, const ConvexPolygon& bbox, const double scale) const;

    /** Check if a point is inside a polygon. It is marked static to be private in this module. */
    bool point_inside(const Point& A) const;

    /** This polygon is updated to the intersection of the original polygon and p */
    ConvexPolygon& intersection (const ConvexPolygon& pol);

    /** This polygon is updated to the union of the original polygon and p */
    ConvexPolygon& p_union (const ConvexPolygon& p);

    /** Check if this polygon is inside the second polygon */
    bool inside (const ConvexPolygon& pol) const;

    /** Creates a new polygon with the four vertices corresponding to the bounding box of the given polygons */
    ConvexPolygon& bbox (const ConvexPolygon& pol);


private:

    /** Vertices of the polygon. */
    vector<Point> vertices;

    /** Color of the polygon */
    double R, G, B;

};

#endif
