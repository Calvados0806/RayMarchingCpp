#include "GLCore.h"

#include <iostream>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR)
        ;
}

bool GLCheckForErrors()
{
    bool no_errors_occurred = true;
    while (GLenum error = glGetError()) {
        std::cerr << "OpenGL Error occurred: 0x" << std::hex << error << std::endl;
        no_errors_occurred = false;
    }
    return no_errors_occurred;
}