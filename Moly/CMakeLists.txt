project(Moly)

# Add dynamic library Moly
add_library(Moly SHARED
    src/Scene/Scene.cpp    
    src/Core/Application.cpp
    src/Core/Log.cpp
    src/Core/Window.cpp  
    vendor/stb_image/stb_image.cpp 
    src/Renderer/Model.cpp 
    src/Renderer/Renderer.cpp
 "src/Renderer/Camera.cpp")

add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/glfw)
add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/glad)
add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/ImGui)
add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/assimp)

target_include_directories(Moly PUBLIC 
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/Core
    ${PROJECT_SOURCE_DIR}/src/Renderer
    ${PROJECT_SOURCE_DIR}/src/Scene
)

# Include thirdparty libraries
target_include_directories(Moly PUBLIC
    ${PROJECT_SOURCE_DIR}/vendor/spdlog/include
    ${PROJECT_SOURCE_DIR}/vendor/glfw/include/GLFW
    ${PROJECT_SOURCE_DIR}/vendor/glad/include
    ${PROJECT_SOURCE_DIR}/vendor/glm
    ${PROJECT_SOURCE_DIR}/vendor/ImGUI
    ${PROJECT_SOURCE_DIR}/vendor/ImGUI/backends
    ${PROJECT_SOURCE_DIR}/vendor/assimp/include
    ${PROJECT_BINARY_DIR}/vendor/assimp/include
    ${PROJECT_SOURCE_DIR}/vendor/stb_image
)

find_package(OpenGL REQUIRED)

target_link_libraries(Moly PRIVATE glad glfw ImGui assimp ${OPENGL_LIBRARIES})
# Define macros
target_compile_definitions(Moly PRIVATE ML_PLATFORM_WINDOWS ML_BUILD_DLL)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Debug configuration detected")
    target_compile_definitions(Moly PRIVATE DEBUG_MODE)
endif()
