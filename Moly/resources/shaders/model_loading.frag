#version 450 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normalMap; // Ajout du normal map en espace monde
    float shininess;
}; 

struct DirLight {
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

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

in vec3 FragPos;  
in vec2 TexCoords;
in vec3 Normal;

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

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Retour au NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{    
    // Échantillonnage du normal map en espace monde
    vec3 normalMap = texture(material.normalMap, TexCoords).rgb;
    normalMap = normalMap * 2.0 - 1.0; // Transformation en intervalle [-1, 1]
    vec3 norm = normalize(normalMap); // La normale perturbée est déjà en espace monde

    vec3 viewDir = normalize(viewPos - FragPos);
    
    // Calcul de l'éclairage
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    for(int i = 0; i < NB_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    
    for(int i = 0; i < NB_SPOT_LIGHTS; i++)
        result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);    
    
    vec4 fragColor = vec4(result, 1.0);
    fragColor.rgb = pow(fragColor.rgb, vec3(1.0/gamma));

    if (visualizeDepth)
        FragColor = vec4(vec3(LinearizeDepth(gl_FragCoord.z) / far), 1.0);
    else
        FragColor = fragColor;
}

// Calcul de la lumière directionnelle
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * pow(texture(material.diffuse, TexCoords).rgb, vec3(gamma));
    if (texture(material.diffuse, TexCoords).a < 0.5) discard;
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    return (ambient + diffuse + specular);
}

// Calcul de la lumière ponctuelle
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * pow(texture(material.diffuse, TexCoords).rgb, vec3(gamma));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// Calcul de la lumière spot
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * pow(texture(material.diffuse, TexCoords).rgb, vec3(gamma));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}
