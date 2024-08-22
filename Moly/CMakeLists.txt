project(Moly)

# Add dynamic library Moly
add_library(Moly SHARED
    src/Core/Application.h
    src/Core/Core.h
    src/Core/Entity.h
    src/Core/EntryPoint.h
    src/Core/Log.h
    src/Core/Moly.h
    src/Core/Scene.cpp
    src/Core/Scene.h

    src/Core/Window.h
    
    src/Renderer/Camera.h
    src/Renderer/Mesh.h
    src/Renderer/Renderer.h
    src/Renderer/Shader.h
    src/Renderer/Vertices.h
    
    src/Core/Application.cpp
    src/Core/Log.cpp
    src/Core/Window.cpp
    src/Core/Entity.cpp
    
    src/Renderer/Mesh.cpp
   "src/Core/TimeManipulation.h" "src/Renderer/Light.h")

add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/glfw)
add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/glad)
add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/ImGui)

target_include_directories(Moly PRIVATE 
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/Core
    ${PROJECT_SOURCE_DIR}/src/Renderer
)

# Include thirdparty libraries
target_include_directories(Moly PUBLIC
    ${PROJECT_SOURCE_DIR}/vendor/spdlog/include
    ${PROJECT_SOURCE_DIR}/vendor/glfw/include/GLFW
    ${PROJECT_SOURCE_DIR}/vendor/glad/include
    ${PROJECT_SOURCE_DIR}/vendor/glm
    ${PROJECT_SOURCE_DIR}/vendor/ImGUI
    ${PROJECT_SOURCE_DIR}/vendor/ImGUI/backends
)

find_package(OpenGL REQUIRED)

target_link_libraries(Moly PRIVATE glad glfw ImGui ${OPENGL_LIBRARIES})
# Define macros
target_compile_definitions(Moly PRIVATE ML_PLATFORM_WINDOWS ML_BUILD_DLL)
