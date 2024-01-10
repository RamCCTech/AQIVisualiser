# AQI Visualizer Project

## Overview
The AQI Visualizer project is designed to display the Air Quality Index (AQI) levels for various states in a visually appealing manner. The project utilizes a map with colored regions representing different AQI levels and a list view providing detailed information about each state.

## Features
- **Map Display:** Renders a map with states colored based on their AQI levels.
- **List View:** Presents a list view with the names of states and their corresponding AQI levels.
- **Data Loading:** Loads state information, coordinates, and AQI data from external files.
- **Date Selection:** Allows users to choose a specific date to view the AQI levels.

## Components

### 1. `AQIVisualizer` Class
   - **Description:** Main class handling the GUI and overall functionality.
   - **Methods:**
     - `loadFile()`: Loads state information and initializes AQI data.
     - `displayMap()`: Updates the map based on the current AQI data.
     - `loadAQIData()`: Loads AQI data from an external JSON file.
     - `updateAQI()`: Updates AQI values based on the selected date and refreshes the map and list view.
     - `updateAQIInListView()`: Updates the AQI value for a specific state in the list view.
     - `addRowToTable()`: Adds a new row to the list view.

### 2. `State` Class
   - **Description:** Represents a state with its name, coordinates, and AQI data.
   - **Methods:**
     - `setName(const std::string& name)`: Sets the name of the state.
     - `setPoints(const std::vector<Point3D>& coordinates)`: Sets the coordinates of the state.
     - `setAQIData(const std::map<std::string, int>& aqiData)`: Sets the AQI data for the state.
     - `name() const`: Retrieves the name of the state.
     - `coordinate() const`: Retrieves the coordinates of the state.
     - `getAQIData() const`: Retrieves the AQI data for the state.

### 3. `OpenGLWindow` Class
   - **Description:** Manages the OpenGL window for displaying the map.
   - **Methods:**
     - `reset()`: Resets the OpenGL context.
     - `initializeGL()`: Initializes OpenGL settings and shaders.
     - `setupMatrix()`: Sets up the transformation matrix for the map.
     - `paintGL()`: Renders the map using OpenGL.
     - `updateShape()`: Updates the map with new shapes and colors.
     - `drawVertices()`: Draws vertices on the map.
     - `mouseMoveEvent()`: Handles mouse movement events.
     - `wheelEvent()`: Handles wheel events for zooming.

### 4. `Point3D` Class
   - **Description:** Represents a 3D point with x and y coordinates.
   - **Methods:**
     - `Point3D(double inX, double inY)`: Constructor.
     - `~Point3D()`: Destructor.
     - `x()`: Retrieves the x-coordinate.
     - `y()`: Retrieves the y-coordinate.
     - `setX(double x)`: Sets the x-coordinate.
     - `setY(double y)`: Sets the y-coordinate.

## Usage
1. Click "Load Country" to load state information and initialize AQI data.
2. Select a date using the date picker.
3. Click "Fetch AQI" to update AQI values based on the selected date.
4. View the map with color-coded states and the list view with detailed AQI information.

## Dependencies
- Qt framework
- OpenGL
