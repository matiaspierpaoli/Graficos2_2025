#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 viewPos;

// Luz puntual única
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightConstant;
uniform float lightLinear;
uniform float lightQuadratic;

// Luz direccional única
uniform vec3 dirLightDir;
uniform vec3 dirLightColor;

// Spotlight única
uniform vec3 spotLightPos;
uniform vec3 spotLightDir;
uniform vec3 spotLightColor;
uniform float spotLightCutOff;      // cos(angle interno)
uniform float spotLightOuterCutOff; // cos(angle externo)
uniform float spotLightConstant;
uniform float spotLightLinear;
uniform float spotLightQuadratic;

// Material
uniform vec3 materialAmbient;
uniform vec3 materialDiffuse;
uniform vec3 materialSpecular;
uniform float materialShininess;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // ----------- Point Light -------------
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    // Atenuación point light
    float distance = length(lightPos - FragPos);
    float attenuation = 1.0 / (lightConstant + lightLinear * distance + lightQuadratic * (distance * distance));

    vec3 ambientPoint = 0.1 * lightColor * materialAmbient;
    float diffPoint = max(dot(norm, lightDir), 0.0);
    vec3 diffusePoint = diffPoint * lightColor * materialDiffuse;
    float specPoint = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
    vec3 specularPoint = specPoint * lightColor * materialSpecular;

    vec3 pointResult = (ambientPoint + diffusePoint + specularPoint) * attenuation;

    // ----------- Directional Light -------------
    vec3 dir = normalize(-dirLightDir);
    vec3 reflectDirDir = reflect(-dir, norm);

    vec3 ambientDir = 0.1 * dirLightColor * materialAmbient;
    float diffDir = max(dot(norm, dir), 0.0);
    vec3 diffuseDir = diffDir * dirLightColor * materialDiffuse;
    float specDir = pow(max(dot(viewDir, reflectDirDir), 0.0), materialShininess);
    vec3 specularDir = specDir * dirLightColor * materialSpecular;

    vec3 dirResult = ambientDir + diffuseDir + specularDir;

    // ----------- Spotlight -------------
    vec3 spotDir = normalize(spotLightPos - FragPos);
    vec3 spotLightDirNorm = normalize(-spotLightDir);

    float theta = dot(spotDir, spotLightDirNorm);
    float epsilon = spotLightCutOff - spotLightOuterCutOff;
    float intensity = clamp((theta - spotLightOuterCutOff) / epsilon, 0.0, 1.0);

    float distSpot = length(spotLightPos - FragPos);
    float attenuationSpot = 1.0 / (spotLightConstant + spotLightLinear * distSpot + spotLightQuadratic * (distSpot * distSpot));

    vec3 reflectDirSpot = reflect(-spotDir, norm);

    vec3 ambientSpot = 0.1 * spotLightColor * materialAmbient;
    float diffSpot = max(dot(norm, spotDir), 0.0);
    vec3 diffuseSpot = diffSpot * spotLightColor * materialDiffuse;
    float specSpot = pow(max(dot(viewDir, reflectDirSpot), 0.0), materialShininess);
    vec3 specularSpot = specSpot * spotLightColor * materialSpecular;

    vec3 spotResult = (ambientSpot + diffuseSpot + specularSpot) * attenuationSpot * intensity;

    // Total iluminación
    vec3 result = pointResult + dirResult + spotResult;
    FragColor = vec4(result, 1.0);
}
