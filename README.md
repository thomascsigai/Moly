# Moly
![banner_white](https://github.com/user-attachments/assets/f09b2c79-9c44-471e-9e3b-b344309b0d58)

# Overview
The MOLY Engine is a 3D game engine development project made with C++/OpenGL and designed to serve as a foundation for learning and exploring advanced graphics programming and engine development. For the moment the project focuses on building a robust and feature-rich rendering engine tailored for game development. Emphasizing advanced rendering, lighting, and post-processing techniques, the MOLY Engine provides an ideal learning environment to enhance skills in graphics programming and game engine design.

## Table of Contents
- [Features](#features)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Usage](#usage)
- [Gallery](#gallery)
- [Planned Features and Milestones](#planned-features-and-milestones)
- [Dependencies](#dependencies)
- [Contributors](#contributors)
- [License](#license)

## Features
- **Lighting Models**: Includes Blinn-Phong Lighting.
- **Model Loading and Management**: Supports loading and managing complex 3D models.
- **Shadow Mapping**: Implements techniques for realistic surface detail and dynamic shadows.
- **Post-Processing Effects**: Includes various effects to enhance visuals.
- **Scene Hierarchy and Entity Management**: Features a basic Entity Component System (ECS) for efficient management of game objects.
- **Inspector and Camera Controls**: Real-time component modification and flexible camera options, including orthographic and perspective views.

## Prerequisites
- CMake (version 3.10 or higher)

## Installation (Tested on Windows 11 using Visual Studio 2022)
For the moment, Moly has been tested only on Windows 11 using Visual Studio 2022. You need CMake 
1. Clone the repository (Must be recursively):
   ```bash
   git clone --recursive https://github.com/thomascsigai/Moly.git
   cd Moly
   ```
2. **Generate Visual Studio project files:**
    ```bash
    mkdir build
    cd build
    cmake ..
    ```
3. **Open the generated solution file:**
    - Navigate to the `build` directory.
    - Open `MolyEngine.sln` with Visual Studio.
4. **Build the project:**
    - Set the build configuration to `Release` or `Debug`.
    - Build project `ALL_BUILD` (In Solution explorer Right-click on `ALL_BUILD` / Build).
  
## Usage
You can test the engine by launching the `Sandbox` App :
1. **Set Sandbox as startup project:**
   - In Solution explorer Right-click on `Sandbox` / Set as startup project.
2. **Run the app:**
   - Run the `Sandbox` project (`F5`)

If you ran an app in the `Debug` configuration, you will be able to use the debug UI to display entities data & components. You'll have to move and place the UI panels yourself (Docking is possible if you click on the panel window and move it to the main window sides).

You can create your own app using the engine by following this steps :
1. **Create a new app:**
   - In source directory, launch `project_setup.bat`.
   - Enter the new app name and press `Enter`.
2. **Regenerate the visual studio files:**
   ```bash
    cd build
    cmake ..
    ```
3. **Build and Launch the app:**
   - Build project `ALL_BUILD`.
   - Set the app as startup project*.
   - Launch it.
The  `project_setup.bat` generate a main cpp file for each project. In this file, you can change the window settings (Resolution, name, VSync etc.) in the `CreateApplication` method by modifying the `SetWindowData()` method. In the `OnStart()` method, you can create the entities that exist in your scene and add components to them. For the moment everything is via code. A Scene Editor will come in the future to handle and save every scene change.

## Gallery
![Sandbox 11_09_2024 18_49_31](https://github.com/user-attachments/assets/d02169c5-480d-41b7-a606-eda123b07be8)
![Sandbox 18_09_2024 00_04_34](https://github.com/user-attachments/assets/b5fd4f89-35de-46bc-9020-5686bffe4ce9)

## Planned Features and Milestones
- **Physically Based Rendering (PBR)**: Implement advanced PBR techniques using the Cook-Torrance BRDF model, including support for textures such as albedo, metallic, roughness, normal, and ambient occlusion. This will enhance realism with accurate lighting and surface interactions.
- **Performance Optimizations**: Focus on optimizing the engine’s performance with techniques such as Frustum Culling to render only visible objects, Vertex Buffer Objects (VBO) and Vertex Array Objects (VAO) for efficient data handling, and Level of Detail (LOD) to reduce the complexity of distant objects. These optimizations will ensure smooth and efficient rendering even in complex scenes.

## Project Progress
Follow the project progress [here](https://malleable-painter-716.notion.site/5af77282be5345a98af1d837fa9b0cee?v=091c14a0577146fca077928508016910&pvs=4).

## Dependencies
- **CMake**: The Build System
- **OpenGL**: The core graphics library used for rendering.
- **GLFW**: For creating windows and handling input.
- **Glad**: For managing OpenGL extensions.
- **GLM**: For mathematical operations.
- **ImGui**: For the user interface.
- **SpdLog**: For the logging system
- **Assimp**: For Model Importing
- **StbImage**: For Image Loading

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Contact
For questions or suggestions, don't hesitate to contact me.

### Social Media
- [LinkedIn](https://www.linkedin.com/in/thomas-csigai/)
- [Itch.io](https://thomas-csigai.itch.io/)
- Email: thomascsigai1@gmail.com
