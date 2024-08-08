# Définir le projet Moly
project(Sandbox)

# Ajouter l'exécutable de l'application
add_executable(Sandbox src/SandboxApp.cpp)

# Ajouter les chemins d'inclusion pour l'application
target_include_directories(Sandbox PUBLIC ${PROJECT_SOURCE_DIR}/../Moly/include)

# Lier l'application à la bibliothèque Moly
target_link_libraries(Sandbox Moly)

# Définir la macro spécifique pour Sandbox
target_compile_definitions(Sandbox PRIVATE ML_PLATFORM_WINDOWS)

# Règle post-build pour copier la DLL dans le répertoire de sortie
add_custom_command(TARGET Sandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "$<TARGET_FILE:Moly>"
    $<TARGET_FILE_DIR:Sandbox>
)