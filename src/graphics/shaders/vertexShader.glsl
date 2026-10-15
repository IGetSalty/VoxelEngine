#version 460 core
uniform mat4 view,projection;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aInstancePosition;

out vec3 vertexColor;

void main()
{
    vec3 worldPosition = aPos+aInstancePosition;
    vertexColor = aColor;
    gl_Position = projection * view * vec4(worldPosition, 1.0);
}