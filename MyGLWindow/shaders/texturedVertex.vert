#version 450
layout(location = 0) in vec3 posAttr;
layout(location = 1) in vec3 normalAttr;
layout(location = 2) in vec2 textCoordAttr;

layout(location = 0) uniform mat4 VM;
layout(location = 1) uniform mat4 PVM;
layout(location = 2) uniform mat4 NormalMat;

out vec3 fNormal;
out vec3 fPosition;
out vec2 fTextCoord;

void main(void) {
    gl_Position = PVM * vec4(posAttr, 1.0);
    // The lighting calculations will be in veiw space.
    fPosition = vec3(VM * vec4(posAttr, 1.0));
    // NormalMat needs to be in view space too
    fNormal = vec3(NormalMat * vec4(normalAttr, 0.0));
    fTextCoord = textCoordAttr;
}
