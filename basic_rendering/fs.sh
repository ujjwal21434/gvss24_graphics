#version 330 core
out vec4 FragColor;
  
uniform vec3 objectColor; // we set this variable in the OpenGL code.
uniform vec3 lightColor; // we set this variable in the OpenGL code.
uniform vec3 lightPos; // we set this variable in the OpenGL code.

in vec3 Normal;
in vec3 FragPos;
void main()
{   
    float kd = 0.9;
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 color = kd * lightColor * max(dot(norm,lightDir),0.0) * objectColor;
    
    FragColor = vec4(color, 1.0);
}   