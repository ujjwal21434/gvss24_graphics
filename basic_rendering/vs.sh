#version 330 core
layout (location = 0) in vec4 vertex; // the position variable has attribute position 0
  
void main()
{
    gl_Position = vertex;
}