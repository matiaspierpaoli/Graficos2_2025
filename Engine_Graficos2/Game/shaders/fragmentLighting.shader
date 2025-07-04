#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 viewPos;

struct DirectionalLight
{
    vec3 direction;
    vec3 color;
    float intensity;
};
uniform DirectionalLight dirLight;

struct PointLight
{
    vec3 position;
    vec3 color;
    float constant;
    float linear;
    float quadratic;
    float intensity;
};

#define MAX_POINT_LIGHTS 4
uniform int numPointLights;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

struct SpotLight
{
    vec3 position;
    vec3 direction;
    vec3 color;
    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
    float intensity;
};

#define MAX_SPOT_LIGHTS 2
uniform int numSpotLights;
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

// Material
uniform vec3 materialAmbient;
uniform vec3 materialDiffuse;
uniform vec3 materialSpecular;
uniform float materialShininess;

in vec2 TexCoords;
uniform sampler2D diffuseTexture;
uniform bool useTexture;

void main()
{
    vec3 texColor = texture(diffuseTexture, TexCoords).rgb;
    vec3 finalDiffuse = materialDiffuse;
    
    if (useTexture) {
        finalDiffuse *= texColor;
    }

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // ----------- Point Light -------------

    vec3 pointResult = vec3(0);

    for (int i = 0; i < numPointLights; i++)
	{
        vec3 lightDir = normalize(pointLights[i].position - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);

        // Atenuación point light
        float distance = length(pointLights[i].position - FragPos);
        float attenuation = 1.0 / (pointLights[i].constant + pointLights[i].linear * distance + pointLights[i].quadratic * (distance * distance));

        vec3 lightColorIntensity = pointLights[i].color * pointLights[i].intensity;

        vec3 ambientPoint = 0.1 * lightColorIntensity * materialAmbient;
        float diffPoint = max(dot(norm, lightDir), 0.0);
        vec3 diffusePoint = diffPoint * lightColorIntensity * finalDiffuse;
        float specPoint = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
        vec3 specularPoint = specPoint * lightColorIntensity * materialSpecular;

        pointResult += (ambientPoint + diffusePoint + specularPoint) * attenuation;
    }

    // ----------- Directional Light -------------
    vec3 lightColorDir = dirLight.color * dirLight.intensity;
    vec3 dir = normalize(-dirLight.direction);
    vec3 reflectDirDir = reflect(-dir, norm);

    vec3 ambientDir = 0.3 * lightColorDir * materialAmbient;
    float diffDir = max(dot(norm, dir), 0.0);
    vec3 diffuseDir = diffDir * lightColorDir * finalDiffuse;
    float specDir = pow(max(dot(viewDir, reflectDirDir), 0.0), materialShininess);
    vec3 specularDir = specDir * lightColorDir * materialSpecular;

    vec3 dirResult = ambientDir + diffuseDir + specularDir;

    // ----------- Spotlight -------------

    vec3 spotResult = vec3(0);

    for (int i = 0; i < numSpotLights; i++)
    {
        vec3 spotDir = normalize(spotLights[i].position - FragPos);
        vec3 spotLightDirNorm = normalize(-spotLights[i].direction);

        float theta = dot(spotDir, spotLightDirNorm);
        float epsilon = spotLights[i].cutOff - spotLights[i].outerCutOff;
        float intensity = clamp((theta - spotLights[i].outerCutOff) / epsilon, 0.0, 1.0);

        float distSpot = length(spotLights[i].position - FragPos);
        float attenuationSpot = 1.0 / (spotLights[i].constant + spotLights[i].linear * distSpot + spotLights[i].quadratic * (distSpot * distSpot));

        vec3 reflectDirSpot = reflect(-spotDir, norm);

        vec3 lightColorSpot = spotLights[i].color * spotLights[i].intensity;

        vec3 ambientSpot = 0.1 * lightColorSpot * materialAmbient;
        float diffSpot = max(dot(norm, spotDir), 0.0);
        vec3 diffuseSpot = diffSpot * lightColorSpot * finalDiffuse;
        float specSpot = pow(max(dot(viewDir, reflectDirSpot), 0.0), materialShininess);
        vec3 specularSpot = specSpot * lightColorSpot * materialSpecular;

        spotResult += (ambientSpot + diffuseSpot + specularSpot) * attenuationSpot * intensity;
    }


    vec3 result = pointResult + dirResult + spotResult;
    FragColor = vec4(result, 1.0);
}

