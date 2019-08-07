#version 450
layout(location = 3) uniform float uAlpha;
layout(location = 4) uniform sampler2D uColorMap;

in vec3 fNormal;
in vec2 fTextCoord;

out vec4 fragColor;

void main(void) {
    //Since we are in view space
    vec3 v = vec3(0.0);
    vec3 l = normalize(vec3(0.0, 0.75, 1.0));
    vec3 n = normalize(fNormal);
    vec3 r = normalize(reflect(-l, n));
    vec3 h = normalize(l + v);
    //Material from texture
    vec3 Ka = 0.1 * texture2D(uColorMap, fTextCoord).rgb;
    vec3 Ks = 0.9 * texture2D(uColorMap, fTextCoord).rgb;
    vec3 Kd = vec3(0.85); //Use always light gray to the specular bright
    float alpha = uAlpha;
    //Light's color (all components are white)
    vec3 La = vec3(1.0);
    vec3 Ls = vec3(1.0);
    vec3 Ld = vec3(1.0);
    //Phong's shading
    vec3 ambient = Ka * La;
    vec3 specular = Ks * Ls * max(0.0, dot(n, l));
    //Well, technically it is Blin - Phong
    vec3 diffuse = Kd * Ld * pow(max(0.0, dot(n, h)), alpha);
    //Final color for this fragment
    fragColor = vec4(ambient + specular + diffuse, 1.0);
}
