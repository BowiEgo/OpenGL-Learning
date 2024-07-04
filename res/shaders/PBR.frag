#version 330 core

out vec4 FragColor;

in vec3 v_FragPosition;
in vec3 v_Normal;
in vec2 v_TexCoords;
in vec4 v_FragPosLightSpace;
in mat3 v_TBN;
in vec3 v_TangentCamPos;
in vec3 v_TangentFragPos;

// material parameters
uniform vec3  u_Albedo;
uniform float u_Metallic;
uniform float u_Roughness;
uniform float u_AO;

uniform bool u_Map_Disabled;
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;
// IBL
uniform samplerCube irradianceMap;
uniform bool u_IrradianceMapEnabled;

// lights
#define MAX_LIGHTS 32
struct PointLight {
    bool enable;
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform int u_NR_PointLights;
uniform PointLight u_PointLights[MAX_LIGHTS];

uniform vec3 u_CameraPosition;

const float PI = 3.14159265359;
// ----------------------------------------------------------------------------
vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(normalMap, v_TexCoords).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(v_FragPosition);
    vec3 Q2  = dFdy(v_FragPosition);
    vec2 st1 = dFdx(v_TexCoords);
    vec2 st2 = dFdy(v_TexCoords);

    vec3 N   = normalize(v_Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}
// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}   
// ----------------------------------------------------------------------------
void main()
{
    vec3  albedo;
    float metallic;
    float roughness;
    float ao;

    if (u_Map_Disabled) {
        albedo    = pow(texture(albedoMap, v_TexCoords).rgb, vec3(2.2));
        metallic  = texture(metallicMap, v_TexCoords).r;
        roughness = texture(roughnessMap, v_TexCoords).r;
        ao        = texture(aoMap, v_TexCoords).r;
    } else {
        albedo    = u_Albedo;
        metallic  = u_Metallic;
        roughness = u_Roughness;
        ao        = u_AO;
    }
    
    vec3 N = normalize(v_Normal);
    vec3 V = normalize(u_CameraPosition - v_FragPosition);

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < 4; ++i)
    {
        // calculate per-light radiance
        vec3 L = normalize(u_PointLights[i].position - v_FragPosition);
        vec3 H = normalize(V + L);
        float distance    = length(u_PointLights[i].position - v_FragPosition);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance     = u_PointLights[i].color * attenuation;

        // cook-torrance brdf
        float NDF = DistributionGGX(N, H, roughness);
        float G   = GeometrySmith(N, V, L, roughness);
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 numerator    = NDF * G * F; 
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
        vec3 specular = numerator / denominator;

        // kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;

        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);

        // add to outgoing radiance Lo
        Lo += (kD * albedo / PI + specular) * radiance * NdotL; // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    }

    // this ambient lighting with environment lighting).
    vec3 ambient = vec3(0.1) * albedo * ao;
    if (u_IrradianceMapEnabled)
    {
        vec3 kS = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
        vec3 kD = vec3(1.0) - kS;
        vec3 irradiance = vec3(0.1);
        if (textureSize(irradianceMap, 0).x > 0) {
            irradiance = texture(irradianceMap, N).rgb;
        }
        vec3 diffuse    = irradiance * albedo;
        ambient         = (kD * diffuse) * ao;
    }

    vec3 color = ambient + Lo;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2)); 

    FragColor = vec4(color, 1.0);
}