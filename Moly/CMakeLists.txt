# Définir le projet Moly
project(Moly)

# Ajouter la bibliothèque dynamique Moly
add_library(Moly SHARED
    "src/Application.cpp"
)

# Ajouter les chemins d'inclusion pour Moly
target_include_directories(Moly PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Include thirdparty libraries
target_include_directories(Moly PUBLIC
    ${PROJECT_SOURCE_DIR}/vendor/spdlog/include
)

# Définir les macros spécifiques pour Moly
target_compile_definitions(Moly PRIVATE ML_PLATFORM_WINDOWS ML_BUILD_DLL)
