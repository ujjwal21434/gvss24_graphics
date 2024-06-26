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
    // vec4 vertices[] = {
	// 	vec4(-0.8,  0.0, 0.0, 1.0),
    //     vec4(-0.4, -0.8, 0.0, 1.0),
    //     vec4( 0.8,  0.8, 0.0, 1.0),
    //     vec4(-0.4, -0.4, 0.0, 1.0)
    // };
	// ivec3 triangles[] = {
	// 	ivec3(0, 1, 3),
	// 	ivec3(1, 2, 3)
	// };

       vec4 vertices[] = {
		vec4(-1.0,  -1.0, 0.0, 1.0),
        vec4(-1.0, 1.0, 0.0, 1.0),
        vec4( 1.0,  1.0, 0.0, 1.0),
        vec4(1.0, -1.0, 0.0, 1.0)
    };
	ivec3 triangles[] = {
		ivec3(0, 1, 3),
		ivec3(1, 2, 3)
	};

    mat4 objectTransformation = mat4(1.0f);//rotate(mat4(1.0f), radians(90.0f), vec3(0.0f,0.0f,1.0f));

	R::Object tickmark = r.createObject();
	r.setVertexAttribs(tickmark, 0, 4, vertices);
	r.setTriangleIndices(tickmark, 2, triangles);
    
    vec4 tempvtex[] = {vertices[0], vertices[1], vertices[2], vertices[3]};
        mat4 view = mat4(1.0f);
        // view transformation
        /*
        vec3 eye = vec3(0.0f,1.0f,1.0f); // camera position
        vec3 t = vec3(0.0f,1.0f,0.0f); // up
        vec3 g = vec3(0.0f, -1.0f,-1.0f); // gaze direction
        // the transformation matrix is given by:
        // [u v w eye] inverse where u,v,w are the camera axes in the world
        // u = txw/||txw||
        // w = -g/||g||
        // v = w x u
        vec3 w = -g / length(g);
        vec3 u = cross(t,w) / length(cross(t,w));
        vec3 v = cross(w,u);
        vec4 e1 = vec4(u,0.0f);
        vec4 e2 = vec4(v,0.0f);
        vec4 e3 = vec4(w,0.0f);
        vec4 o = vec4(eye,1.0f);
        view = inverse(mat4(e1,e2,e3,o));
        */
        view = lookAt(vec3(0.0f,1.0f,1.0f), vec3(0.0f,0.0f,0.0f), vec3(0.0f,1.0f,0.0f));        

    for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++) {
            std::cout << view[i][j] << " ";
        }
        std::cout << std::endl;
    }
    mat4 projection = mat4(1.0);
    projection = perspective(radians(60.0f), (float)width/(float)height, 0.1f, 100.0f);

    while (!r.shouldQuit()) {
        r.clear(vec4(1.0, 1.0, 1.0, 1.0));
        r.useShaderProgram(program);

        objectTransformation = mat4(1.0f);
        // objectTransformation = rotate(objectTransformation, radians(-90.0f), vec3(0.0f,0.0f,1.0f));
        // objectTransformation = translate(objectTransformation, vec3(1.0f,1.0f,0.0f));

        for(int i=0; i<4; i++) {
            tempvtex[i] = projection * view * objectTransformation *   vertices[i]; 
        }
        r.setUniform<vec4>(program, "color", vec4(0.8, 0.8, 0.8, 1.0));
		r.setVertexAttribs(tickmark, 0, 4, tempvtex);
	    r.setTriangleIndices(tickmark, 2, triangles);
        r.setupFilledFaces();
        r.drawObject(tickmark);
        r.setupWireFrame();
        r.setUniform<vec4>(program, "color", vec4(0.0, 0.0, 0.0, 1.0));
        r.drawObject(tickmark);
    

        r.show();
    }
    r.deleteShaderProgram(program);
    return EXIT_SUCCESS;
}
