# Geometry DLL

## Overview
The Geometry DLL provides a simple implementation of geometric entities such as points and lines. It includes two classes, `Point3D` and `Line`, which can be used to represent and manipulate 3D points and lines in space.

## Classes

### 1. `Point3D` Class
   - **Description:** Represents a 3D point with x and y coordinates.
   - **Methods:**
     - `Point3D(double inX, double inY)`: Constructor.
     - `~Point3D()`: Destructor.
     - `x()`: Retrieves the x-coordinate.
     - `y()`: Retrieves the y-coordinate.
     - `setX(double x)`: Sets the x-coordinate.
     - `setY(double y)`: Sets the y-coordinate.

### 2. `Line` Class
   - **Description:** Represents a line segment connecting two `Point3D` instances.
   - **Methods:**
     - `Line(Point3D p1, Point3D p2)`: Constructor.
     - `Line(const Line& other)`: Copy constructor.
     - `~Line()`: Destructor.
     - `p1()`: Retrieves the first endpoint (`Point3D`) of the line.
     - `p2()`: Retrieves the second endpoint (`Point3D`) of the line.
     - `operator=(const Line& other)`: Assignment operator.

## Building and Using the DLL
1. Include the provided header files (`Point3D.h` and `Line.h`) in your project.
2. Link your project with the compiled Geometry DLL.
3. Utilize the `Point3D` and `Line` classes in your application code.

## Building the DLL
If you wish to build the Geometry DLL, use the provided source files (`Point3D.cpp` and `Line.cpp`). Compile the source files into a dynamic-link library (DLL) using your preferred compiler.
