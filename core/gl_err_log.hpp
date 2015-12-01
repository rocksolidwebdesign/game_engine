#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <GL/glew.h>

#include <GL/glfw.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#ifndef GAME_GL_ERR_LOG_H
#define GAME_GL_ERR_LOG_H

bool GameEngine_log_gl_error(int lineNumber, const char * fileName);
bool GameEngine_log_gl_err(GLenum e);
const char * GameEngine_get_gl_err_msg(GLenum e);

// #define GLERR(x) do { x; GLenum e; while ((e = glGetError()) != GL_NO_ERROR) { \
//   fprintf(stderr, "OpenGL Error %X at line %d in file %s\n", e, __LINE__, __FILE__); \
//   GameEngine_log_gl_err(e); \
// } } while(0);

#define GLERR(x) do{ x; GameEngine_log_gl_error(__LINE__, __FILE__); }while(0);

#endif
