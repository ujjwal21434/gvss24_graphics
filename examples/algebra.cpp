#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<iostream>  

// using namespace glm;

int main() {

    // rotation
    glm::mat4 rotation_transform = glm::mat4(1.0f);
    rotation_transform = glm::rotate(rotation_transform, glm::radians(90.0f), glm::vec3(0.0,0.0,1.0));
    
    // for(int i=0; i<4; i++) {
    //     for(int j=0; j<4; j++) {
    //         std::cout << transform[i][j] << " ";
    //     }
    //         std::cout <<  " \n";

    // }

    glm::vec4 vec = glm::vec4(1.0f,0.0f,0.0f,1.0f);


    // glm::mat4 rotation_scale_transform = glm::scale(rotation_transform, glm::vec3(10.0,10.0,1.0));

    // glm::mat4 rotate_scale_translate = glm::translate(rotation_scale_transform, glm::vec3(0.0f,10.0f,0.0f));
    glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,10.0f, 0.0f));

    vec = translate * vec;

    std::cout << vec.x << "," << vec.y;
    return 0;
}

// glm::vec4 v = glm::vec4(1.0f,0.0f,0.0f,1.0f);
    // translation
    // mat4 trans = glm::mat4(1.0f);
    // trans = translate(trans, glm::vec3(1.0f,1.0f,0.0f));
    // vec = trans * vec;

    