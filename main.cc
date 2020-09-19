#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <set>
#include <cassert>
#include <vector>
#include <fstream>
#include <pngwriter.h>
using namespace std;

#include "Point.hh"
#include "Polygon.hh"


// ************************************************************************************


void error (int i){
    if (i == 1) cout << "error: undefined polygon identifier" << endl;
    if (i == 2) cout << "error: command with wrong number of arguments" << endl;
}


void pol_polygon (map<string, ConvexPolygon>& polygons, istringstream& iss) {
    string name;
    iss >> name;
    if (name == ""){
        error(2);
        return;
    }
    ConvexPolygon pol;
    pol.new_pol(iss);
    polygons[name] = pol; //Retorna una referència a l'element amb clau k (afegint-lo, si no hi era)
    cout << "ok" << endl;
}


void pol_print (map<string, ConvexPolygon>& polygons, istringstream& iss) { // Only the convex hull
    string name;
    iss >> name;
    if (name == ""){
        error(2);
        return;
    }
    if (polygons.count(name)) cout << name << polygons[name].print() << endl;
    else error(1);
}


void pol_area (map<string, ConvexPolygon>& polygons, istringstream& iss) {
    string name;
    iss >> name;
    if (name == ""){
        error(2);
        return;
    }
    if (polygons.count(name)) cout << polygons[name].area() << endl;
    else error(1);

}


void pol_perimeter (map<string, ConvexPolygon>& polygons, istringstream& iss) {
    string name;
    iss >> name;
    if (name == ""){
        error(2);
        return;
    }
    if (polygons.count(name)) cout << polygons[name].perimeter() << endl;
    else error(1);
}


void pol_vertices (map<string, ConvexPolygon>& polygons, istringstream& iss) {
    string name;
    iss >> name;
    if (name == ""){
        error(2);
        return;
    }
    if (polygons.count(name)) cout << polygons[name].num_vertices() << endl;
    else error(1);
}


void pol_centroid (map<string, ConvexPolygon>& polygons, istringstream& iss) {
    string name;
    iss >> name;
    if (name == ""){
        error(2);
        return;
    }
    string extra;
    iss >> extra;
    if (extra != ""){
        error(2);
        return;
    }
    if (polygons.count(name)) {
        if (polygons[name].num_vertices() != 0) {
            Point p = polygons[name].centroid();
            cout << p.get_x() << " " << p.get_y() << endl;
        } else cout << "error: the polygon has no vertices" << endl;
    } else error(1);
}


void pol_list (map<string, ConvexPolygon>& polygons) {
    for (auto it : polygons) cout << it.first << " ";
    cout << endl;
}


void pol_save (map<string, ConvexPolygon>& polygons, istringstream& iss) {
    string file, name;
    iss >> file;
    if (file == ""){
        error(2);
        return;
    }
    ofstream outdata;
    outdata.open(file);
    while (iss >> name){
        if (polygons.count(name)) {
            outdata << name;
            polygons[name].save(outdata);
        } else {
            error(1);
            return;
        }
    }
    if (name == ""){
        error(2);
        return;
    }
    outdata.close();
    cout << "ok" << endl;
}


void pol_load (map<string, ConvexPolygon>& polygons, istringstream& iss) {
    string file;
    iss >> file;
    if (file == ""){
        error(2);
        return;
    }
    ifstream inFile;
    inFile.open(file);
    if (!inFile) cout << "error: file not valid" << endl; // The file doesn't exist or another program is writing it
    else {
        string line;
        while (getline(inFile, line)) {
            istringstream iss_2(line);
            pol_polygon(polygons, iss_2);
        }
    }
}


void pol_setcol (map<string, ConvexPolygon>& polygons, istringstream& iss) {
    string name;
    iss >> name;
    if (name == ""){
        error(2);
        return;
    }
    if (polygons.count(name)) {
        polygons[name].setcol(iss);
    } else error(1);
}


void pol_draw (map<string, ConvexPolygon>& polygons, istringstream& iss) {
    string file;
    iss >> file;
    if (file == ""){
        error(2);
        return;
    }
    string p;
    set<string> pols;
    while (iss >> p) {
        if (polygons.count(p)) pols.insert(p);
        else {
            error(1);
            return;
        }
    }
    ConvexPolygon bboxpol;
    vector<string> v;
    if (not pols.empty()) {
        for (string pol : pols){
            bboxpol.bbox(polygons[pol]);
            v.push_back(pol);
        }
    } else{
        error(2);
        return;
    }
    double size = bboxpol.size_drawing();
    if (size == 0) return;
    double scale = 497 / size;
    int n = file.size();
    char char_array[n];
    strcpy(char_array, file.c_str());
    pngwriter png(500, 500 , 1.0, char_array);
    for(string pol : v) polygons[pol].draw(png, bboxpol, scale);
    png.close();
    cout << "ok" << endl;
}


//Afegir comentari
void pol_modify (string action, map<string, ConvexPolygon>& polygons, istringstream& iss) {
    string p1, p;
    iss >> p1;
    if (p1 ==""){
        error(2);
        return;
    }
    set<string> pols;
    while (iss >> p) {
        if (p == p1){
            error(2);
            return;
        }
        else if (polygons.count(p)) pols.insert(p);
        else{
            error(1);
            return;
        }
    }
    if (pols.size() == 1){
        if (not polygons.count(p1)){
            error(1);
            return;
        }
        auto sos = pols.begin();
        string spol = *sos;
        if (action == "intersection") polygons[p1].intersection(polygons[spol]);
        else if (action == "union") polygons[p1].p_union(polygons[spol]);
        else assert(false);
    } else if (pols.size() == 2){
        auto sos = pols.begin();
        string spol = *sos;
        ConvexPolygon aux = polygons[spol];
        ++sos;
        spol = *sos;
        if (action == "intersection") aux.intersection(polygons[spol]);
        else if (action == "union") aux.p_union(polygons[spol]);
        else assert(false);
        polygons[p1] = aux;
    } else {
        error(2);
        return;
    }
    cout << "ok" << endl;
}


void pol_inside (map<string, ConvexPolygon>& polygons, istringstream& iss) {
    string p1, p2;
    iss >> p1 >> p2;
    if (p2 == ""){
        error(2);
        return;
    }
    string extra;
    iss >> extra;
    if (extra != ""){
        error(2);
        return;
    }
    if (polygons.count(p1) and polygons.count(p2)){
        if (polygons[p1].inside(polygons[p2])) cout << "yes" << endl;
        else cout << "not" << endl;
    } else error(1);
}


void pol_bbox (map<string, ConvexPolygon>& polygons, istringstream& iss) {
    string bounding_box;
    iss >> bounding_box;
    ConvexPolygon bboxpol;
    string p;
    set<string> pols;
    while (iss >> p) {
        if (polygons.count(p)) pols.insert(p);
        else {
            error(1);
            return;
        }
    }
    if (not pols.empty()) for (string pol : pols) bboxpol.bbox(polygons[pol]);
    else{
        error(2);
        return;
    }
    cout << "ok" << endl;
    polygons[bounding_box] = bboxpol;
}

// ************************************************************************************


int main () {
    map<string, ConvexPolygon> polygons;
    string line;
    cout.setf(ios::fixed);
    cout.precision(3);
    while (getline(cin, line)) {
      istringstream iss(line);
      string action;
      iss >> action;
             if (action == "polygon")               pol_polygon(polygons, iss);
        else if (action == "print")                 pol_print(polygons, iss);
        else if (action == "area")                  pol_area(polygons, iss);
        else if (action == "perimeter")             pol_perimeter(polygons, iss);
        else if (action == "vertices")              pol_vertices(polygons, iss);
        else if (action == "centroid")              pol_centroid(polygons, iss);
        else if (action == "list")                  pol_list(polygons);
        else if (action == "save")                  pol_save(polygons, iss);
        else if (action == "load")                  pol_load(polygons, iss);
        else if (action == "setcol")                pol_setcol(polygons, iss);
        else if (action == "draw")                  pol_draw(polygons, iss);
        else if (action == "intersection")          pol_modify(action, polygons, iss);
        else if (action == "union")                 pol_modify(action, polygons, iss);
        else if (action == "inside")                pol_inside(polygons, iss);
        else if (action == "bbox")                  pol_bbox(polygons, iss);

        else if (action == "#")                     cout << "#" << endl;
        else cout << "error: invalid command" << endl;
    }
}
