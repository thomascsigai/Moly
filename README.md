# Moly
![banner_white](https://github.com/user-attachments/assets/f09b2c79-9c44-471e-9e3b-b344309b0d58)

# Overview

The MOLY Engine is a 3D game engine development project made with C++/OpenGL and designed to serve as a foundation for learning and exploring advanced graphics programming and engine development. For the moment the project focuses on building a robust and feature-rich rendering engine tailored for game development. Emphasizing advanced rendering, lighting, and post-processing techniques, the MOLY Engine provides an ideal learning environment to enhance skills in graphics programming and game engine design.

## Table of Contents
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Planned Features and Milestones](#planned-features-and-milestones)
- [Dependencies](#dependencies)
- [Platform](#platform)
- [Contributors](#contributors)
- [License](#license)

## Features
- **Lighting Models**: Includes Blinn-Phong Lighting.
- **Model Loading and Management**: Supports loading and managing complex 3D models.
- **Shadow Mapping**: Implements techniques for realistic surface detail and dynamic shadows.
- **Post-Processing Effects**: Includes various effects to enhance visuals.
- **Scene Hierarchy and Entity Management**: Features a basic Entity Component System (ECS) for efficient management of game objects.
- **Inspector and Camera Controls**: Real-time component modification and flexible camera options, including orthographic and perspective views.

## Installation (Tested on Windows using Visual Studio 2022)
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

## Project Progress

Follow the project progress [here](https://malleable-painter-716.notion.site/5af77282be5345a98af1d837fa9b0cee?v=091c14a0577146fca077928508016910&pvs=4).

## Acknowledgements

- **CMake**: The Build System
- **OpenGL**: The core graphics library used for rendering.
- **GLFW**: For creating windows and handling input.
- **GLEW**: For managing OpenGL extensions.
- **GLM**: For mathematical operations.
- **ImGui**: For the user interface.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Contact

For questions or suggestions, don't hesitate to contact me.

### Social Media

- [LinkedIn](https://www.linkedin.com/in/thomas-csigai/)
- [Itch.io](https://thomas-csigai.itch.io/)
- Email: thomascsigai1@gmail.com
