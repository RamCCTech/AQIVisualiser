## AQIVisualiser

### Overview

The AQIVisualiser application is designed to visually represent the Air Quality Index (AQI) levels of different states using a color-coded system. This application utilizes FileReader, Geometry, and State DLLs to efficiently process and display the AQI data.

### Components

1. **FileReader DLL**
   - Responsible for reading input data files containing Air Quality Index information for various states.
   - Parses and extracts relevant data to be used by the AQIVisualiser application.

2. **Geometry DLL**
   - Manages geometric calculations and visualization components within the AQIVisualiser.
   - Utilizes data from the FileReader to create a visual representation of AQI levels.

3. **State DLL**
   - Provides functionality specific to individual states, including data processing and formatting.
   - Integrates with FileReader and Geometry DLLs to contribute to the overall AQIVisualiser functionality.

4. **Software Requirements Specification (SRS)**
   - For a detailed specification of software requirements, please refer to the [SRS document](https://docs.google.com/document/d/1oAr_QijS8Fjqretm9xS7iyfKsbVHRV6n/edit?usp=sharing&ouid=114503488717471352484&rtpof=true&sd=true).
### Video Demonstration

Check out the video demonstration of the AQIVisualiser application [here](https://drive.google.com/file/d/1z2VrYYeviYhDN3_n6Io48j3oQ_Y_g8L3/view?usp=sharing).

### AQI Color Codes (2023)

The color-coding system for AQI levels in 2023 is as follows:

- **Good (0-50):** Green
- **Moderate (51-100):** Yellow
- **Unhealthy for Sensitive Groups (101-150):** Orange
- **Unhealthy (151-200):** Red
- **Very Unhealthy (201-300):** Purple
- **Hazardous (301 and above):** Maroon

### Usage

1. **Install Dependencies:**
   - Ensure that all required dependencies and libraries are installed.

2. **Run the Application:**
   - Execute the AQIVisualiser application.
   - Load the AQI data file using the FileReader DLL.

3. **Visualize AQI Levels:**
   - Explore the color-coded representation of AQI levels for different states.

4. **Interact with the UI:**
   - Click on individual states to view detailed AQI information.

5. **Video Tutorial:**
   - For a detailed guide on using the AQIVisualiser, watch our [video tutorial](https://drive.google.com/file/d/1z2VrYYeviYhDN3_n6Io48j3oQ_Y_g8L3/view?usp=sharing).
