# Ajoute le fichier glad.c en tant que bibliothèque statique
add_library(glad STATIC src/glad.c)

# Définir les chemins d'inclusion pour GLAD
target_include_directories(glad PUBLIC include)