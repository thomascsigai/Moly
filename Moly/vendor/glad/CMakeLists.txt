# Ajoute le fichier glad.c en tant que biblioth�que statique
add_library(glad STATIC src/glad.c)

# D�finir les chemins d'inclusion pour GLAD
target_include_directories(glad PUBLIC include)