#ifndef HW_HPP
#define HW_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <string>

namespace GVSS24 {
	namespace Hardware {

		using VertexShader = GLuint;
		using FragmentShader = GLuint;

		using ShaderProgram = GLuint;

		struct Object {
			GLuint vao;
			int nTris;
		};

#include "api.hpp"

	}
}

#endif
