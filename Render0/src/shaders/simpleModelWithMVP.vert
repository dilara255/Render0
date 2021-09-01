#version 330 core

layout( location = 0 ) in vec4 vPosition;
layout( location = 1 ) in vec4 vColor;
layout( location = 2 ) in vec4 vNormal;
layout( location = 3 ) in vec4 vAmbient;
layout( location = 4 ) in vec4 vSpecular;
layout( location = 5 ) in vec4 vSpcCoef;

uniform mat4 vPV_matrix;
uniform mat4 vModel_matrix;
uniform vec4 vColorUniform;

out vec4 fColor;

void main()
{
    gl_Position = vPV_matrix * vModel_matrix * vPosition;

    vec4 lightSource = normalize(vec4(-1, -1, -1, 1));

    float diffuse = dot(vNormal, lightSource);
    diffuse = diffuse * max(diffuse, 0);

    fColor = vColorUniform * (diffuse + 0.075);
}
