# Project 2: A 3D Transformation and Projection System

###### Raymond Tsang
###### 912868864
###### rhtsang@ucdavis.edu

Compilation and name of executable
----------
In the project's root directory, run 'make' to compile the executable named 'draw'

How to run, command syntax, input/output file format
---------
The syntax to run the program is "./draw input output" where "input" and "output" are names of input and output files.  
The file format of the input and output files is the same as the one described in the project specifications.

Location of algorithm implementations
-----------------

Transformations: transform.cpp, under the functions rotate(), scale(), and translate()  
Projections: projection.cpp, under the functions bound() (for computing a bounding box and mapping to NDC) and project()

Other
--------------
When coordinates are rotated, the rotated coordinates written to the output file have some rounding error. I assume this to be because of the cmath library's sine, cosine, and tangent functions.  
It is assumed that the coordinates of the input polygons all lie within the unit cube, as stated in the project specifications.  
When the wire frames are projected onto the viewing screen, the upper-left quadrant corresponds to the XY plane, upper-right quadrant to the YZ plane, and bottom-right to the XZ plane.  
