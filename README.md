# Polygons Practice AP2 2019


1. Descarregueu el repositori amb la pràctica.

2. Entreu al directori `PracticaPolygons` i compileu amb `make`.

3. Hauràs de tenir descarregat un compilador de C++ i la llibreria de codi obert per a crear imatges PNG a C++




## The Polygon calculator

The Polygon calculator reads commands from the standard input and writes
their answers to the standard output. In some cases, it also uses some
files.


The specification of the commands is given bellow. Each command is given in a line and produces exactly one line of output.


### Comments

Lines starting with a hash sign (`#`) are comments. Their output is just a
hash sign.

### Polygon identifiers

All commands include polygon identifiers. These are made by words, such as
`p`, `p1`, `p2`, or `pol_name`.

### Points

Points in the commands are given by two pairs of real numbers, in standard
notation, to denote the X and Y coordinates. For instance, `0 0` or `8.34
-2.3`. When printed, all real numbers are formatted with three digits
after the decimal dot.

### Colors

Colors in the commands are given by three real numbers in [0,1], in standard
notation, to denote the RGB color. For instance, `0 0 0` denotes black, `1 0
0` denotes red, and `1 0.64 0` denotes orange.

### File names

Filenames in the commands are made up of words, such as `f`, `pol.txt` or
`some_file_name.pol`.

### The `polygon` command

The `polygon` command associates an identifier with a convex polygon made by a
set of zero or more points. If the polygon identifier is new, it will create
it. If it already existed, it will overwrite the previous polygon. New
polygons are black.


### The `print` command

The `print` command prints the name and the vertices of a given
polygon. The output only contains the vertices in the convex hull of the
polygon, in clockwise order, starting from the vertex will lower X (and the
vertex with lower Y in case of ties). They are printed in a single line,
with one space separating each value.


### The `area` command

The `area` command prints the area of the given polygon.

### The `perimeter` command

The `perimeter` command prints the perimeter of the given polygon.

### The `vertices` command

The `vertices` command prints the number of vertices of the convex hull of the
given polygon.


### The `centroid` command

The `centroid` command prints the centroid of the given polygon.

If the given polygon has no vertices the following error is printed: the polygon has no vertices

### The `list` command

The `list` command lists all polygon identifiers, lexycographically sorted.

### The `save` command

The `save` command saves the given polygons in a file, overwriting it if it
already existed. The contents of the file are the same as in the `print`
command, with a polygon per line.

### The `load` command

The `load` command loads the polygons stored in a file, in the same way as
`polygon`, but retrieving the vertices and identifiers from the file.

If the file doesn't exist or another program is writing it the following error is printed: file not valid

### The `setcol` command

The `setcol` command associates a color to the given polygon.

If the numbers that associate the colors are bigger than 1.0 or smaller than 0.0 the following error is printed: invalid color

### The `draw` command

The `draw` command draws a list of polygons in a PNG file, each one with its
associated color. The image is of 500x500 pixels, with white background
and the coordinates of the vertices are scaled to fit in the 498x498
central part of the image, while preserving the original aspect ratio.

If any of the polygons given have no vertices the following error is printed: the polygon has no vertices.


### The `intersection` command

This command may receive two or three parameters:

- When receiving two parameters `p1` and `p2`, `p1` is updated to the intersection of the original `p1` and `p2`.

- When receiving three parameters `p1`, `p2` and `p3`, `p1` is updated to the intersection of `p2` and `p3`.
If the parameter `p1` already existed it is overwrited with the value of the intersection.


### The `union` command

Just as the `intersection` command, but with the convex union of polygons.


### The `inside` command

Given two polygons, the `inside` command prints `yes` or `not` to tell whether
the first is inside the second or not.


### The `bbox` command

The `bbox` command creates a new polygon with the four vertices corresponding to the
bounding box of the given polygons.


### Commands without answer

Some commands do not really produce an answer. In
this case `ok` is printed, unless there is some error.


### Errors

If any command contains or produces an error, the error is printed in a
line starting with `error: ` and the command is completely ignored (as if
it was not given). Possible errors include:

- invalid command
- command with wrong number of arguments
- undefined polygon identifier
- invalid color
- file not valid

### Precision

In order to cope with precision issues of float numbers, I have used an absolute
tolerance of `1e-12` when comparing values.
