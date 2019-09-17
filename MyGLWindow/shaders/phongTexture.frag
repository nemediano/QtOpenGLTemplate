#version 450
layout(location = 3) uniform float uAlpha;
layout(location = 4) uniform sampler2D uDiffuseMap;
layout(location = 5) uniform sampler2D uSpecularMap;

in vec3 fPosition;
in vec3 fNormal;
in vec2 fTextCoord;

out vec4 fragColor;

void main(void) {
    //Since we are in view space: v = (0.0, 0.0, 0.0) - fPosition
    vec3 v = normalize(-fPosition);
    // This is a directional light in view space (comes from behind
    // of the camera focus and towards the object)
    vec3 l = normalize(vec3(0.0, 0.0, 1.0));
    vec3 n = normalize(fNormal);
    //vec3 r = normalize(reflect(-l, n));
    vec3 h = normalize(l + v);
    //Material from texture
    vec3 Ka = 0.1 * texture2D(uDiffuseMap, fTextCoord).rgb;
    vec3 Ks = 0.9 * texture2D(uDiffuseMap, fTextCoord).rgb;
    vec3 Kd = texture2D(uSpecularMap, fTextCoord).rgb;
    float alpha = uAlpha;
    //Light's color (all components are white)
    vec3 La = vec3(1.0);
    vec3 Ls = vec3(1.0);
    vec3 Ld = vec3(1.0);
    //Phong's shading
    vec3 ambient = Ka * La;
    vec3 diffuse = Kd * Ld * max(0.0, dot(n, l));
    //Well, technically it is Blin - Phong
    vec3 specular = Ks * Ls * pow(max(0.0, dot(n, h)), alpha);
    //Final color for this fragment
    fragColor = vec4(ambient + specular + diffuse, 1.0);
}
