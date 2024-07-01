#version 330 core
layout (location = 0) in vec4 vertex; // the position variable has attribute position 0
layout (location = 1) in vec3 normal; // the position variable has attribute position 0

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos;
void main()
{
    FragPos = mat3(model) * vec3(vertex.xyz);
    mat4 mvp = projection * view * model;
    Normal = transpose(inverse(mat3(model))) * normal;

    gl_Position = mvp * (vertex);
}