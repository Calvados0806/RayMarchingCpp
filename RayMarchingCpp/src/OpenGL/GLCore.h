#pragma once

#include <GL/glew.h>

void GLClearError();
bool GLCheckForErrors();


#ifdef _DEBUG

#define ASSERT(x) if (!(x)) __debugbreak()

#define GLCall(funccall) \
    GLClearError(); \
    funccall; \
    ASSERT(GLCheckForErrors())

#else

#define ASSERT(x) ((void)0)

#define GLCall(funccall) funccall

#endif // _DEBUG