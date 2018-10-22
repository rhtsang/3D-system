#include "transform.h"

#include "polygon.h"
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

const double pi = 3.14159265358979323846;

Coordinate findCentroid(Polygon polygon) {

    double centroid_x = 0;
    double centroid_y = 0;
    double centroid_z = 0;
    for (vector<Coordinate>::iterator itr = polygon.vertices.begin(); itr != polygon.vertices.end(); itr++) {
        centroid_x += itr->x;
        centroid_y += itr->y;
        centroid_z += itr->z;
    }
    centroid_x /= polygon.numVertices;
    centroid_y /= polygon.numVertices;
    centroid_z /= polygon.numVertices;

    Coordinate centroid(centroid_x, centroid_y, centroid_z);
    return centroid;
}

void translate(Polygon& polygon, double translate_x, double translate_y, double translate_z) {
    for (int i = 0; i < polygon.vertices.size(); i++) {
        polygon.vertices.at(i).x += translate_x;
        polygon.vertices.at(i).y += translate_y;
        polygon.vertices.at(i).z += translate_z;
    }
}

void scale(Polygon& polygon, double factor) {

    Coordinate centroid = findCentroid(polygon);

    translate(polygon, -1*centroid.x, -1*centroid.y, -1*centroid.z);

    for (int i = 0; i < polygon.vertices.size(); i++) {
        polygon.vertices.at(i).x *= factor;
        polygon.vertices.at(i).y *= factor;
        polygon.vertices.at(i).z *= factor;
    }

    translate(polygon, centroid.x, centroid.y, centroid.z);

}

void rotate(Polygon& polygon, Coordinate axis_start, Coordinate axis_end, double angle) {

    // translate to origin
    translate(polygon, -1*(axis_start.x), -1*(axis_start.y), -1*(axis_start.z));
    cout << "Post translation: " << endl;
    for (int i = 0; i < polygon.vertices.size(); i++) {
        cout << polygon.vertices.at(i).x << ' ' << polygon.vertices.at(i).y << ' ' << polygon.vertices.at(i).z << endl;
    }

    // rotate into yz plane by rotating about y axis
    if (axis_end.z != axis_start.z && axis_end.x != axis_start.x) {
        double phi = atan( (axis_end.x - axis_start.x) / (axis_end.z - axis_start.z) );
        cout << phi << endl;
        for (int i = 0; i < polygon.vertices.size(); i++) {

            double x = polygon.vertices.at(i).x;
            double z = polygon.vertices.at(i).z;

            if (axis_end.z > axis_start.z) {
                polygon.vertices.at(i).x = x*cos(pi/2 - phi) - z*sin(pi/2 - phi);
                polygon.vertices.at(i).z = z*cos(pi/2 - phi) - x*sin(pi/2 - phi);
                cout << polygon.vertices.at(i).x << endl;
                cout << polygon.vertices.at(i).z << endl;
            } else if (axis_end.z < axis_start.z) {
                polygon.vertices.at(i).x = x*cos(-1*(pi/2 + phi)) - z*sin(-1*(pi/2 + phi));
                polygon.vertices.at(i).z = z*cos(-1*(pi/2 + phi)) - x*sin(-1*(pi/2 + phi));
            }

        }
        cout << "Post rotation about y axis: " << endl;
        for (int i = 0; i < polygon.vertices.size(); i++) {
            cout << polygon.vertices.at(i).x << ' ' << polygon.vertices.at(i).y << ' ' << polygon.vertices.at(i).z << endl;
        }
    } else if (axis_end.z == axis_start.z && axis_end.x != axis_start.x) {
        cout << "special case y" << endl;
        for (int i = 0; i < polygon.vertices.size(); i++) {

            double x = polygon.vertices.at(i).x;
            double z = polygon.vertices.at(i).z;
            if (axis_end.x > axis_start.x) {
                polygon.vertices.at(i).x = x*cos(-1*(pi/2)) - z*sin(-1*(pi/2));
                polygon.vertices.at(i).z = z*cos(-1*(pi/2)) - x*sin(-1*(pi/2));
            } else if (axis_end.x < axis_start.x) {
                polygon.vertices.at(i).x = x*cos(pi/2) - z*sin(pi/2);
                polygon.vertices.at(i).z = z*cos(pi/2) - x*sin(pi/2);
            }
        }
    }

    // rotate onto z axis by rotate about x axis
    if (axis_end.y != axis_start.y && axis_end.z != axis_start.z) {
        double theta = atan( (axis_end.z - axis_start.z) / (axis_end.y - axis_start.y) );
        for (int i = 0; i < polygon.vertices.size(); i++) {

            double y = polygon.vertices.at(i).y;
            double z = polygon.vertices.at(i).z;

            if (axis_end.y > axis_start.y) {
                polygon.vertices.at(i).y = y*cos(theta/2 - theta) - z*sin(theta/2 - theta);
                polygon.vertices.at(i).z = y*sin(theta/2 - theta) + z*cos(theta/2 - theta);
            } else if (axis_end.y < axis_start.y) {
                polygon.vertices.at(i).y = y*cos(-1*(theta/2 + theta)) - z*sin(-1*(theta/2 + theta));
                polygon.vertices.at(i).z = y*sin(-1*(theta/2 + theta)) + z*cos(-1*(theta/2 + theta));
            }

        }
        cout << "Post rotation about x axis: " << endl;
        for (int i = 0; i < polygon.vertices.size(); i++) {
            cout << polygon.vertices.at(i).x << ' ' << polygon.vertices.at(i).y << ' ' << polygon.vertices.at(i).z << endl;
        }
    }

    // rotate about the "z" axis
    for (int i = 0; i < polygon.vertices.size(); i++) {
        double x = polygon.vertices.at(i).x;
        double y = polygon.vertices.at(i).y;

        polygon.vertices.at(i).x = x*cos(angle) - y*sin(angle);
        polygon.vertices.at(i).y = x*sin(angle) + y*cos(angle);
    }
    cout << "Post rotation about z axis: " << endl;
    for (int i = 0; i < polygon.vertices.size(); i++) {
        cout << polygon.vertices.at(i).x << ' ' << polygon.vertices.at(i).y << ' ' << polygon.vertices.at(i).z << endl;
    }

    // rotate back about x axis
    if (axis_end.y != axis_start.y && axis_end.z != axis_start.z) {
        double theta = atan( (axis_end.z - axis_start.z) / (axis_end.y - axis_start.y) );
        for (int i = 0; i < polygon.vertices.size(); i++) {

            double y = polygon.vertices.at(i).y;
            double z = polygon.vertices.at(i).z;

            if (axis_end.y > axis_start.y) {
                polygon.vertices.at(i).y = y*cos(theta - theta/2) - z*sin(theta - theta/2);
                polygon.vertices.at(i).z = y*sin(theta - theta/2) + z*cos(theta - theta/2);
            } else if (axis_end.y < axis_start.y) {
                polygon.vertices.at(i).y = y*cos(theta/2 + theta) - z*sin(theta/2 + theta);
                polygon.vertices.at(i).z = y*sin(theta/2 + theta) + z*cos(theta/2 + theta);
            }

        }
        cout << "Post rotation back about x axis: " << endl;
        for (int i = 0; i < polygon.vertices.size(); i++) {
            cout << polygon.vertices.at(i).x << ' ' << polygon.vertices.at(i).y << ' ' << polygon.vertices.at(i).z << endl;
        }
    }

    // rotate back about y axis
    if (axis_end.z != axis_start.z && axis_end.x != axis_start.x) {
        double phi = atan( (axis_end.x - axis_start.x) / (axis_end.z - axis_start.z) );
        for (int i = 0; i < polygon.vertices.size(); i++) {

            double x = polygon.vertices.at(i).x;
            double z = polygon.vertices.at(i).z;

            if (axis_end.z > axis_start.z) {
                polygon.vertices.at(i).x = x*cos(phi - pi/2) - z*sin(phi - pi/2);
                polygon.vertices.at(i).z = z*cos(phi - pi/2) - x*sin(phi - pi/1);
            } else if (axis_end.z < axis_start.z) {
                polygon.vertices.at(i).x = x*cos(pi/2 + phi) - z*sin(pi/2 + phi);
                polygon.vertices.at(i).z = z*cos(pi/2 + phi) - x*sin(pi/2 + phi);
            }

        }
        cout << "Post rotation back about y axis: " << endl;
        for (int i = 0; i < polygon.vertices.size(); i++) {
            cout << polygon.vertices.at(i).x << ' ' << polygon.vertices.at(i).y << ' ' << polygon.vertices.at(i).z << endl;
        }
    } else if (axis_end.z == axis_start.z && axis_end.x != axis_start.x) {
        cout << "special case y reverse" << endl;
        for (int i = 0; i < polygon.vertices.size(); i++) {

            double x = polygon.vertices.at(i).x;
            double z = polygon.vertices.at(i).z;
            if (axis_end.x > axis_start.x) {
                polygon.vertices.at(i).x = x*cos((pi/2)) - z*sin((pi/2));
                polygon.vertices.at(i).z = z*cos((pi/2)) - x*sin((pi/2));
            } else if (axis_end.x < axis_start.x) {
                polygon.vertices.at(i).x = x*cos(-pi/2) - z*sin(-pi/2);
                polygon.vertices.at(i).z = z*cos(-pi/2) - x*sin(-pi/2);
            }
        }
    }

    // translate back
    translate(polygon, axis_start.x, axis_start.y, axis_start.z);
}
