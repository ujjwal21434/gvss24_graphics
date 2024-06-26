#include "../src/a1.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<iostream>
namespace R = GVSS24::Hardware;
using namespace glm;

int main() {
    int width = 640, height = 480;

	R::Rasterizer r;
    if (!r.initialize("Example 1", width, height))
        return EXIT_FAILURE;
    R::ShaderProgram program = r.createShaderProgram(
        r.vsIdentity(),
        r.fsConstant()
    );
    vec4 vertices[] = {
		vec4(-0.8,  0.0, 0.0, 1.0),
        vec4(-0.4, -0.8, 0.0, 1.0),
        vec4( 0.8,  0.8, 0.0, 1.0),
        vec4(-0.4, -0.4, 0.0, 1.0)
    };
	ivec3 triangles[] = {
		ivec3(0, 1, 3),
		ivec3(1, 2, 3)
	};

    mat4 objectTransformation = mat4(1.0f);//rotate(mat4(1.0f), radians(90.0f), vec3(0.0f,0.0f,1.0f));
    mat4 model = mat4(1.0);

    mat4 view = mat4(1.0f);
    mat4 projection = mat4(1.0f);

	R::Object tickmark = r.createObject();
	r.setVertexAttribs(tickmark, 0, 4, vertices);
	r.setTriangleIndices(tickmark, 2, triangles);
    // Enable dcepth test.
    r.enableDepthTest();
    
    vec4 tempvtex[] = {vertices[0], vertices[1], vertices[2], vertices[3]};
    while (!r.shouldQuit()) {
        r.clear(vec4(1.0, 1.0, 1.0, 1.0));
        r.useShaderProgram(program);

        
        objectTransformation = mat4(1.0f);
        // objectTransformation = rotate(objectTransformation, radians(-55.0f), vec3(1.0f,0.0f,0.0f));
        for(int i=0; i<4; i++) {
            tempvtex[i] = projection* view * model *objectTransformation *   vertices[i]; 
        }
        r.setUniform<vec4>(program, "color", vec4(0.0, 0.6, 0.0, 1.0));
		r.setVertexAttribs(tickmark, 0, 4, tempvtex);
	    r.setTriangleIndices(tickmark, 2, triangles);
    
        r.drawObject(tickmark);
    
        r.show();
    }
    r.deleteShaderProgram(program);
    return EXIT_SUCCESS;
}
