project(Sponza)

add_executable(Sponza src/Sponza.cpp)

target_include_directories(Sponza PRIVATE 
    ${PROJECT_SOURCE_DIR}/../Moly/src
    ${PROJECT_SOURCE_DIR}/../Moly/src/Core
    ${PROJECT_SOURCE_DIR}/../Moly/src/Renderer
    ${PROJECT_SOURCE_DIR}/../Moly/src/Scene
)

target_link_libraries(Sponza PRIVATE Moly)

target_compile_definitions(Sponza PRIVATE ML_PLATFORM_WINDOWS)

file(COPY ${PROJECT_SOURCE_DIR}/resources/ DESTINATION ${PROJECT_BINARY_DIR}/resources)
file(COPY ${PROJECT_SOURCE_DIR}/../Moly/resources/ DESTINATION ${PROJECT_BINARY_DIR}/resources)

add_custom_command(TARGET Sponza POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "$<TARGET_FILE:Moly>"
    "$<TARGET_FILE_DIR:Sponza>/"
)