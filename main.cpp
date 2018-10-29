#include <GL/glut.h>
#include <vector>
#include <iostream>
#include "polygon.h"
#include "filemanipulation.h"
#include "transform.h"
#include "projection.h"

void display();
vector<Polygon> polygons;
char* outputFile;

int main(int argc, char *argv[])
{
	readFile(argv[1], polygons);
    outputFile = argv[2];

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE);

	//create and set main window title
	int MainWindow = glutCreateWindow("ECS175 Project 2");
	glClearColor(0, 0, 0, 0); //clears the buffer of OpenGL
	glutDisplayFunc(display);

	glutMainLoop();//main display loop, will display until terminate

	return 0;
}

//main display loop, this function will be called again and again by OpenGL
void display()
{
    // transformation prompt
    int polygonID;
    char transformation;
    cout << "Enter the ID of the polygon you want to manipulate (indexing starts at 0): ";
    cin >> polygonID;
    cout << "Enter the first letter of the transformation you want performed. 's' for scale, 'r' for rotate, 't' for translate: ";
    cin >> transformation;

    if (transformation == 't') {
        float translate_x, translate_y, translate_z;
        cout << "How far along the x-axis would you like to translate? ";
        cin >> translate_x;
        cout << "How far along the y-axis would you like to translate? ";
        cin >> translate_y;
        cout << "How far along the z-axis would you like to translate? ";
        cin >> translate_z;
        translate(polygons.at(polygonID), translate_x, translate_y, translate_z);
    } else if (transformation == 'r') {
        float axis_start_x, axis_start_y, axis_start_z, axis_end_x, axis_end_y, axis_end_z;
        float angle;
        cout << "Enter the x-coordinate of the starting point of the rotation axis: ";
        cin >> axis_start_x;
        cout << "Enter the y-coordinate of the starting point of the rotation axis: ";
        cin >> axis_start_y;
        cout << "Enter the z-coordinate of the starting point of the rotation axis: ";
        cin >> axis_start_z;
        Coordinate start(axis_start_x, axis_start_y, axis_start_z);
        cout << "Enter the x-coordinate of the ending point of the rotation axis: ";
        cin >> axis_end_x;
        cout << "Enter the y-coordinate of the ending point of the rotation axis: ";
        cin >> axis_end_y;
        cout << "Enter the z-coordinate of the ending point of the rotation axis: ";
        cin >> axis_end_z;
        Coordinate end(axis_end_x, axis_end_y, axis_end_z);
        cout << "How far would you like to rotate? (enter an angle in radians) ";
        cin >> angle;
        rotate(polygons.at(polygonID), start, end, angle);
    } else if (transformation == 's') {
        float factor;
        cout << "How much would you like to scale? ";
        cin >> factor;
        scale(polygons.at(polygonID), factor);
    }

    writeFile(outputFile, polygons);

	//Misc.
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

    // bound and project onto planes
    vector<Polygon> boundedPolygons = bound(polygons);
    project(boundedPolygons);

	//window refresh
	glFlush();
    glutPostRedisplay();
}
