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

    Coordinate transform_axis_start = axis_start;
    Coordinate transform_axis_end = axis_end;
    double phi = 0;
    double theta = 0;

    // translate to origin
    // translate rotation axis to origin
    transform_axis_end.x -= transform_axis_start.x;
    transform_axis_end.y -= transform_axis_start.y;
    transform_axis_end.z -= transform_axis_start.z;
    transform_axis_start.x = 0;
    transform_axis_start.y = 0;
    transform_axis_start.z = 0;

    // translate object's vertices accordingly
    translate(polygon, -axis_start.x, -axis_start.y, -axis_start.z);

    // rotate to coincide with z axis

    // rotate into the yz plane if needed (ie if x coordinates are different)
    if (transform_axis_end.x != transform_axis_start.x) {
        if (transform_axis_end.z != transform_axis_start.z) { // find angle
            phi = atan(transform_axis_end.x / transform_axis_end.z);
            if (transform_axis_end.z > transform_axis_start.z) {
                phi = pi/2 - phi;
            } else if (transform_axis_end.z < transform_axis_start.z) {
                phi = -1*(pi/2+phi);
            }
        } else if (transform_axis_end.z == transform_axis_start.z) { // +/- 90 degrees
            if (transform_axis_end.x > transform_axis_start.x) { // -90
                phi = -pi/2;
            } else if (transform_axis_end.x < transform_axis_start.x) { // +90
                phi = pi/2;
            }
        }
        double x = transform_axis_end.x;
        double z = transform_axis_end.z;
        transform_axis_end.x = (x)*cos(phi) + (z)*sin(phi);
        transform_axis_end.z = (z)*cos(phi) - (x)*sin(phi);
        for (vector<Coordinate>::iterator itr = polygon.vertices.begin(); itr != polygon.vertices.end(); itr++) {
            double x = itr->x;
            double z = itr->z;
            itr->x = (x)*cos(phi) + (z)*sin(phi);
            itr->z = (z)*cos(phi) - (x)*sin(phi);
        }
    }

    // now in yz plane, rotate onto z axis unless already on z-axis by rotate about x axis
    if (transform_axis_end.y == transform_axis_start.y) {
        if (transform_axis_end.z < transform_axis_start.z) {
            theta = pi;
        }
    } else {
        theta = atan(transform_axis_end.z / transform_axis_end.y);
        if (transform_axis_end.y > transform_axis_start.y) {
            theta = pi/2 - theta;
        } else if (transform_axis_end.y < transform_axis_start.y) {
            theta = -1*(pi/2+theta);
        }

    }
    double y = transform_axis_end.y;
    double z = transform_axis_end.z;
    transform_axis_end.y = (y)*cos(theta) - (z)*sin(theta);
    transform_axis_end.z = (z)*cos(theta) + (y)*sin(theta);
    for (vector<Coordinate>::iterator itr = polygon.vertices.begin(); itr != polygon.vertices.end(); itr++) {
        double y = itr->y;
        double z = itr->z;
        itr->y = (y)*cos(theta) - (z)*sin(theta);
        itr->z = (z)*cos(theta) + (y)*sin(theta);
    }

    // do actual rotation about "z" axis
    for (vector<Coordinate>::iterator itr = polygon.vertices.begin(); itr != polygon.vertices.end(); itr++) {
        double x = itr->x;
        double y = itr->y;
        itr->x = (x)*cos(angle) - (y)*sin(angle);
        itr->y = (y)*cos(angle) + (x)*sin(angle);
    }

    // rotate back
    theta *= -1;
    for (vector<Coordinate>::iterator itr = polygon.vertices.begin(); itr != polygon.vertices.end(); itr++) {
        double y = itr->y;
        double z = itr->z;
        itr->y = (y)*cos(theta) - (z)*sin(theta);
        itr->z = (z)*cos(theta) + (y)*sin(theta);
    }

    phi *= -1;
    for (vector<Coordinate>::iterator itr = polygon.vertices.begin(); itr != polygon.vertices.end(); itr++) {
        double x = itr->x;
        double z = itr->z;
        itr->x = (x)*cos(phi) + (z)*sin(phi);
        itr->z = (z)*cos(phi) - (x)*sin(phi);
    }

    // translate back
    translate(polygon, axis_start.x, axis_start.y, axis_start.z);

}
