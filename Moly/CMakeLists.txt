project(Moly)

# Add dynamic library Moly
add_library(Moly SHARED
    src/Core/Application.cpp
    src/Core/Log.cpp
    src/Core/Window.cpp
 "src/Renderer/Shader.h"  "src/Renderer/Camera.h" "src/Renderer/Renderer.h")

add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/glfw)
add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/glad)

target_include_directories(Moly PRIVATE 
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/Core
)

# Include thirdparty libraries
target_include_directories(Moly PUBLIC
    ${PROJECT_SOURCE_DIR}/vendor/spdlog/include
    ${PROJECT_SOURCE_DIR}/vendor/glfw/include/GLFW
    ${PROJECT_SOURCE_DIR}/vendor/glad/include
    ${PROJECT_SOURCE_DIR}/vendor/glm
)

find_package(OpenGL REQUIRED)

target_link_libraries(Moly PRIVATE glad glfw ${OPENGL_LIBRARIES})
# Define macros
target_compile_definitions(Moly PRIVATE ML_PLATFORM_WINDOWS ML_BUILD_DLL)
