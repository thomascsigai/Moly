#version 450 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
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
in vec3 Normal;  
in vec2 TexCoords;
in vec4 FragPosLightSpace; // Position du fragment dans l'espace de la lumière (ajouté)

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform int NB_POINT_LIGHTS;
uniform PointLight pointLights[MAX_LIGHTS];
uniform int NB_SPOT_LIGHTS;
uniform SpotLight spotLights[MAX_LIGHTS];
uniform Material material;
uniform float gamma;
uniform bool visualizeDepth;

uniform sampler2D shadowMap; // Shadow map (ajouté)

// Prototypes des fonctions
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, float shadow); // Modifié pour inclure le shadow
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

float near = 0.1f;
float far = 90.0f;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Retour à NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}

// Fonction de calcul de l'ombre
float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
    // Projection perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Transformation vers l'espace [0,1]
    projCoords = projCoords * 0.5 + 0.5;
    // Vérifier si le fragment est à l'extérieur de la shadow map
    if(projCoords.z > 1.0)
        return 0.0;
    // Récupérer la profondeur la plus proche depuis la perspective de la lumière
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // Récupérer la profondeur actuelle du fragment depuis la perspective de la lumière
    float currentDepth = projCoords.z;
    // Calcul du biais pour éviter l'acné des ombres
    float bias = max(0.005 * (1.0 - dot(normal, lightDir)), 0.0005);
    // Vérifier si le fragment est dans l'ombre
    float shadow = 0.0;
    vec2 texelSize = 2.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    return shadow;
}

void main()
{    
    // Propriétés du fragment
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Calcul de la direction de la lumière directionnelle
    vec3 lightDir = normalize(-dirLight.direction);

    // Calcul de l'ombre
    float shadow = ShadowCalculation(FragPosLightSpace, norm, lightDir);

    // Phase 1 : éclairage directionnel avec ombre
    vec3 result = CalcDirLight(dirLight, norm, viewDir, shadow);

    // Phase 2 : éclairage des lumières ponctuelles
    for(int i = 0; i < NB_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    

    // Phase 3 : éclairage des spots
    for(int i = 0; i < NB_SPOT_LIGHTS; i++)
        result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);    
    
    // Correction gamma
    vec4 fragColor = vec4(result, 1.0);
    fragColor.rgb = pow(fragColor.rgb, vec3(1.0 / gamma));

    // Visualisation de la profondeur si activée
    if (visualizeDepth)
        FragColor = vec4(vec3(LinearizeDepth(gl_FragCoord.z) / far), 1.0);
    else
        FragColor = fragColor;
}

// Calcul de l'éclairage directionnel avec ombre
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, float shadow)
{
    vec3 lightDir = normalize(-light.direction);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    // Spéculaire
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    // Composantes de lumière
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * pow(texture(material.diffuse, TexCoords).rgb, vec3(gamma));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    // Application de l'ombre sur les composantes diffuse et spéculaire
    diffuse *= (1.0 - shadow);
    specular *= (1.0 - shadow);
    // Si le matériau est transparent, on le discarde
    if (texture(material.diffuse, TexCoords).a < 0.5) discard;
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * pow(texture(material.diffuse, TexCoords).rgb, vec3(gamma));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    //vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * pow(texture(material.diffuse, TexCoords).rgb, vec3(gamma));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}