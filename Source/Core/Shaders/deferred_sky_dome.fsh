// Atmospheric scattering fragment shader
// Based on work by Sean O'Neil

#version 400
layout(location = 0) out vec4 diffuseColor;
layout(location = 1) out vec4 specularColor;
layout(location = 2) out vec3 normal;
layout(location = 3) out float depth;

in Fragment {
    vec3 v3Direction;
    vec4 frontColor;
    vec4 frontSecondaryColor;
    float depth;
} fragment;

uniform vec3 v3LightPos;
uniform float g;
uniform float g2;

void main (void)
{
	float fCos = dot(v3LightPos, fragment.v3Direction) / length(fragment.v3Direction);
	float fRayleighPhase = 1.0 + fCos * fCos;
	float fMiePhase = (1.0 - g2) / (2.0 + g2) * (1.0 + fCos * fCos) / pow(1.0 + g2 - 2.0 * g * fCos, 1.5);
	diffuseColor = vec4(1.0 - exp(-1.5 * (fRayleighPhase * fragment.frontColor.rgb + fMiePhase * fragment.frontSecondaryColor.rgb)), 1.0);
    specularColor = diffuseColor;
    normal = vec3(0.0);
    depth = fragment.depth;
//    diffuseColor += vec4(1.5);
//    specularColor = vec4(1);
//    normal = vec3(1);
//    depth = fragment.depth;
}
