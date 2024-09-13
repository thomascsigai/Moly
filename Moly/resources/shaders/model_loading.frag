#version 450 core

out vec4 FragColor;

// Structure Material
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;  // Normal map
    float shininess;
}; 

// Structure Lumière Directionnelle
struct DirLight {
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// Structure Lumière Ponctuelle
struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// Structure Spotlight
struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

#define MAX_LIGHTS 32

// Entrée du shader vertex
in VS_OUT {
    vec3 FragPos;        // Position du fragment en espace monde
    vec2 TexCoords;      // Coordonnées de texture
    mat3 TBN;            // Matrice Tangente-Bitangente-Normale
} fs_in;

// Uniformes
uniform vec3 viewPos;
uniform DirLight dirLight;
uniform int NB_POINT_LIGHTS;
uniform PointLight pointLights[MAX_LIGHTS];
uniform int NB_SPOT_LIGHTS;
uniform SpotLight spotLights[MAX_LIGHTS];
uniform Material material;
uniform float gamma;
uniform bool visualizeDepth;

// Prototypes des fonctions
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

float near = 0.1f;
float far = 90.0f;

// Fonction pour linéariser la profondeur
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Retour à NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}

// Fonction pour récupérer la normale depuis la normal map
vec3 GetNormalFromMap()
{
    // Échantillonne la normale de la normal map dans la plage [0,1]
    vec3 normal = texture(material.normal, fs_in.TexCoords).rgb;
    // Transforme le vecteur normal dans la plage [-1,1]
    normal = normalize(normal * 2.0 - 1.0);
    return normal;
}

void main()
{    
    // Récupère la normale depuis la normal map et la transforme en espace monde
    vec3 normal = GetNormalFromMap();
    normal = normalize(fs_in.TBN * normal);

    // Vecteur de vue en espace monde
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);

    // Initialisation du résultat avec l'éclairage directionnel
    vec3 result = CalcDirLight(dirLight, normal, viewDir);

    // Ajoute les contributions des lumières ponctuelles
    for(int i = 0; i < NB_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], normal, fs_in.FragPos, viewDir);

    // Ajoute les contributions des spotlights
    for(int i = 0; i < NB_SPOT_LIGHTS; i++)
        result += CalcSpotLight(spotLights[i], normal, fs_in.FragPos, viewDir);
    
    // Application de la correction gamma
    vec4 fragColor = vec4(result, 1.0);
    fragColor.rgb = pow(fragColor.rgb, vec3(1.0 / gamma));

    // Option de visualisation de la profondeur
    if (visualizeDepth)
        FragColor = vec4(vec3(LinearizeDepth(gl_FragCoord.z) / far), 1.0);
    else
        FragColor = fragColor;
}

// Fonction de calcul pour la lumière directionnelle
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    // Éclairage diffus
    float diff = max(dot(normal, lightDir), 0.0);

    // Éclairage spéculaire
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // Combinaison des résultats
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, fs_in.TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fs_in.TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, fs_in.TexCoords));

    // Gestion de la transparence
    if (texture(material.diffuse, fs_in.TexCoords).a < 0.5) discard;

    return (ambient + diffuse + specular);
}

// Fonction de calcul pour les lumières ponctuelles
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // Éclairage diffus
    float diff = max(dot(normal, lightDir), 0.0);

    // Éclairage spéculaire
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // Atténuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    // Combinaison des résultats
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, fs_in.TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fs_in.TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, fs_in.TexCoords));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

// Fonction de calcul pour les spotlights
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // Éclairage diffus
    float diff = max(dot(normal, lightDir), 0.0);

    // Éclairage spéculaire
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // Atténuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    // Intensité du spotlight
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // Combinaison des résultats
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, fs_in.TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fs_in.TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, fs_in.TexCoords));

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return (ambient + diffuse + specular);
}
