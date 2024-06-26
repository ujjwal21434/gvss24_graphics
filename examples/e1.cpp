#include "../src/a1.hpp"
#include <glm/gtc/matrix_transform.hpp>

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
        vec4(-1.0,  1.0, 0.0, 1),
        vec4(1.0, 1.0, 0.0,  1),
        vec4( 1.0,  -1.0, 0.0, 1),
        vec4(-1.0, -1.0, 0.0, 1),

        
    };
        mat4 objectTransformation = mat4(1.0f);//rotate(mat4(1.0f), radians(90.0f), vec3(0.0f,0.0f,1.0f));
        mat4 model = mat4(1.0);
        // mat4 view = rotate(translate(mat4(1.0f), vec3(0.0f, 0.0f, -3.0f)), radians(10.0f), vec3(0.0f,0.0f,1.0f)); 
        mat4 view = translate(mat4(1.0f), vec3(0.0f, 0.0f, -3.0f)); 

        mat4 projection = perspective(radians(60.0f), (float)width/(float)height, 0.1f, 100.0f);

	ivec3 triangles[] = {
		ivec3(0, 1, 2),
		ivec3(0, 2, 3)
	};
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
