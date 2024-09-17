cmake_minimum_required(VERSION 3.27)
project(MolyEngine)

# Define cpp standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Ajouter les sous-répertoires pour les différents modules du moteur
add_subdirectory(Moly)
add_subdirectory(Sandbox)
