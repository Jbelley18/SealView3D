# SealView3D

**SealView3D** is a lightweight 3D viewer designed to visualize neuron morphology from **SWC** files. It provides an interactive interface for exploring neuron structures and is currently focused on loading and rendering SWC files in real-time.

## Features
- **SWC File Loading**: Load and visualize neuron morphology files in the SWC format.
- **Real-Time 3D Visualization**: Explore neuron structures with pan, zoom, and rotate controls.

## Usage
1. **Clone the Repository**:
   ```bash
   git clone https://github.com/yourusername/SealView3D.git
   cd SealView3D
   ```

2. **Build the Project**:
   Ensure you have all required dependencies (C++, OpenGL, Qt) installed, and follow the steps to build the project:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

3. **Run the Application**:
   Execute the application and load your SWC file:
   ```bash
   ./SealView3D
   ```

4. **Load SWC File**:  
   Use the "Load SWC File" button in the UI to select an SWC file from your file system. The neuron morphology will be rendered in the 3D viewer.

## Future Features
- Depth perception with perspective projection.
- Basic lighting and shading.
- Interactive neuron data display (e.g., branch stats).
- Neuron data analysis (total branch length, soma size, etc.).
- Color-coded neuron components.
- Error-handling for file loading.
- Customizable display options (thickness, transparency, wireframe).

## Requirements
- **C++** (version 17 or greater)
- **Qt Framework** (version 6.7.X or greater)
- **OpenGL** (version 4.6 or greater)

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
![image](https://github.com/user-attachments/assets/e56f26ea-366b-4a59-ad88-a1ba46399d29)
![image](https://github.com/user-attachments/assets/ce49c2f1-9448-4fbf-94a5-877a385a667b)

