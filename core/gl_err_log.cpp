#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <GL/glew.h>

#include <GL/glfw.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "gl_err_log.hpp"

bool GameEngine_log_gl_error(int lineNumber, const char * fileName)
{
        GLenum e; 

        while ((e = glGetError()) != GL_NO_ERROR)
        {
                fprintf( stderr, "(line %d in %s) Open GL ERROR %X: %s\n", lineNumber, fileName, e, GameEngine_get_gl_err_msg(e) );
        }
}

bool GameEngine_log_gl_err(GLenum e)
{
        fprintf( stderr, "GL Error Message: %s\n", GameEngine_get_gl_err_msg(e) );
}

const char * GameEngine_get_gl_err_msg(GLenum e)
{
        switch (e)
        {
                case GL_NO_ERROR:
                        return "No error has been recorded.  The value of this symbolic constant is guaranteed to be 0.";
                        break;

                case GL_INVALID_ENUM:
                        return "An unacceptable value is specified for an enumerated argument.  The offending command is ignored and has no other side effect than to set the error flag.";
                        break;

                case GL_INVALID_VALUE:
                        return "A numeric argument is out of range.  The offending command is ignored and has no other side effect than to set the error flag.";
                        break;

                case GL_INVALID_OPERATION:
                        return "The specified operation is not allowed in the current state.  The offending command is ignored and has no other side effect than to set the error flag.";
                        break;

                case GL_INVALID_FRAMEBUFFER_OPERATION:
                        return "The framebuffer object is not complete. The offending command is ignored and has no other side effect than to set the error flag.";
                        break;

                case GL_OUT_OF_MEMORY:
                        return "There is not enough memory left to execute the command.  The state of the GL is undefined, except for the state of the error flags, after this error is recorded.";
                        break;

                case GL_STACK_UNDERFLOW:
                        return "An attempt has been made to perform an operation that would cause an internal stack to underflow.";
                        break;

                case GL_STACK_OVERFLOW:
                        return "An attempt has been made to perform an operation that would cause an internal stack to overflow.";
                        break;
        }
}
