project(Moly)

# Add dynamic library Moly
add_library(Moly SHARED
    "src/Application.cpp"
    "src/Log.cpp"
)

target_include_directories(Moly PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Include thirdparty libraries
target_include_directories(Moly PUBLIC
    ${PROJECT_SOURCE_DIR}/vendor/spdlog/include
    ${PROJECT_SOURCE_DIR}/vendor/glfw/include/GLFW
    ${PROJECT_SOURCE_DIR}/vendor/glad/include
)

# Define macros
target_compile_definitions(Moly PRIVATE ML_PLATFORM_WINDOWS ML_BUILD_DLL)
