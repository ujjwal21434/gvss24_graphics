#include "../src/a1.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<iostream>
#include<fstream>
#include <sstream>

namespace R = GVSS24::Hardware;
using namespace glm;
int nvertices = 24;
int ntriangles = 12;
int main() {
    int width = 640, height = 480;

	R::Rasterizer r;
    if (!r.initialize("Example 1", width, height))
        return EXIT_FAILURE;

    // read shader code
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try 
    {
        // open files
        vShaderFile.open("../basic_rendering/vs.sh");
        fShaderFile.open("../basic_rendering/fs.sh");
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();		
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();		
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    R::ShaderProgram program = r.createShaderProgram(
        r.vsCreateShader(vShaderCode),
        r.fsCreateShader(fShaderCode)
        // r.vsIdentity(),
        // r.fsConstant()
    );

    vec4 vertices[] = {
        // Back Face
        vec4(0.0,  0.0, 0.0, 1),
        vec4(0.0, 1.0, 0.0,  1),
        vec4( 1.0,  1.0, 0.0, 1),
        vec4(1.0, 0.0, 0.0, 1),

        // Front Face
        vec4(0.0,  0.0, 1.0, 1),
        vec4(0.0, 1.0, 1.0,  1),
        vec4( 1.0,  1.0, 1.0, 1),
        vec4(1.0, 0.0, 1.0, 1),

        // Right Face 8
        vec4( 1.0,  1.0, 0.0, 1),
        vec4( 1.0,  1.0, 1.0, 1),
        vec4(1.0, 0.0, 0.0, 1),
        vec4(1.0, 0.0, 1.0, 1),

        // Left Face 12
        vec4(0.0,  0.0, 0.0, 1),
        vec4(0.0, 1.0, 1.0,  1),
        vec4(0.0, 1.0, 0.0,  1),
        vec4(0.0,  0.0, 1.0, 1),

        // Top Face 16
        vec4(0.0, 1.0, 0.0,  1),
        vec4(0.0, 1.0, 1.0,  1),
        vec4( 1.0,  1.0, 1.0, 1),
        vec4( 1.0,  1.0, 0.0, 1),

        // Bottom Face 20
        vec4(0.0,  0.0, 0.0, 1),
        vec4(1.0, 0.0, 0.0, 1),
        vec4(1.0, 0.0, 1.0, 1),
        vec4(0.0,  0.0, 1.0, 1)

    };

	ivec3 triangles[] = {
        // Back Face
		ivec3(0, 1, 2),
		ivec3(0, 2, 3),
        // Front Face
        ivec3(4,5,6),
        ivec3(4,6,7),
        // Right Face
        ivec3(8,9,10),
        ivec3(10,9,11),
        // Left Face
        ivec3(12,13,14),
        ivec3(12,15,13),
        // Top Face
        ivec3(16,17,18),
        ivec3(16,18,19),
        // Bottom Face
        ivec3(20,21,22),
        ivec3(20,22,23)
	};

    vec3 normals[] = {
        // Back Face
        vec3(0.0f,0.0f, -1.0f),
        vec3(0.0f,0.0f, -1.0f),
        vec3(0.0f,0.0f, -1.0f),
        vec3(0.0f,0.0f, -1.0f),

        // Front Face
        vec3(0.0f,0.0f, 1.0f),
        vec3(0.0f,0.0f, 1.0f),
        vec3(0.0f,0.0f, 1.0f),
        vec3(0.0f,0.0f, 1.0f),

        // Right Face
        vec3(1.0f,0.0f, 0.0f),
        vec3(1.0f,0.0f, 0.0f),
        vec3(1.0f,0.0f, 0.0f),
        vec3(1.0f,0.0f, 0.0f),

        // Left Face
        vec3(-1.0f,0.0f, 0.0f),
        vec3(-1.0f,0.0f, 0.0f),
        vec3(-1.0f,0.0f, 0.0f),
        vec3(-1.0f,0.0f, 0.0f),

        // Top Face
        vec3(0.0f,1.0f, 0.0f),
        vec3(0.0f,1.0f, 0.0f),
        vec3(0.0f,1.0f, 0.0f),
        vec3(0.0f,1.0f, 0.0f),

        // Bottom Face
        vec3(0.0f,-1.0f, 0.0f),
        vec3(0.0f,-1.0f, 0.0f),
        vec3(0.0f,-1.0f, 0.0f),
        vec3(0.0f,-1.0f, 0.0f),
        

    };  
    mat4 objectTransformation = mat4(1.0f);//rotate(mat4(1.0f), radians(90.0f), vec3(0.0f,0.0f,1.0f));

	R::Object cuboid = r.createObject();
	
    vec4 tempvtex[nvertices];
    for(int i=0; i<nvertices; i++) {
        tempvtex[i] = vertices[i];
    }
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
        vec3 eye = vec3(-1.0f,-1.0f,-2.0f);
        vec3 gaze = vec3(0.0f,0.0f,0.0f);
        vec3 up = vec3(0.0f,1.0f,0.0f);
        view = lookAt(eye, gaze, up);        

        mat4 projection = mat4(1.0);
        // perspective transformation
        // b = -t
        // l = -r
        // t = 0.06
        // r = 0.077

        projection = perspective(radians(60.0f), (float)width/(float)height, 0.1f, 100.0f);


    r.enableDepthTest();
    while (!r.shouldQuit()) {
        r.clear(vec4(1.0, 1.0, 1.0, 1.0));
        r.useShaderProgram(program);

        objectTransformation = mat4(1.0f);
        // objectTransformation = rotate(objectTransformation, radians(60.0f), vec3(1.0f,0.0f,0.0f));
        // objectTransformation = translate(objectTransformation, vec3(1.0f,1.0f,0.0f));
        objectTransformation = translate(objectTransformation, vec3(-0.5f,-0.5f,-0.5f));
        for(int i=0; i<nvertices; i++) {
            tempvtex[i] =projection * view * objectTransformation* vertices[i]; 
        }

        r.setVertexAttribs(cuboid, 0, nvertices, tempvtex);
	    r.setTriangleIndices(cuboid, ntriangles, triangles);
        r.setupFilledFaces();
        r.setUniform<vec4>(program, "color", vec4(1.0f,0.8f,0.8f,1.0f));
        r.drawObject(cuboid);
       
        r.setupWireFrame();
        r.setUniform<vec4>(program, "color", vec4(0.0f, 0.0f, 0.0f, 1.0f));
        r.drawObject(cuboid);
    

        r.show();
    }
    r.deleteShaderProgram(program);
    return EXIT_SUCCESS;
}
