#include "Texture.h"
#include "GLCore.h"

#include <stb_image.h>

using namespace OpenGL;

Texture::Texture(const std::string& path)
	: mOpenGLID(0), mFilePath(path), mLocalBuffer(nullptr), 
	  mWidth(0), mHeight(0), mBitDepth(0)
{
	stbi_set_flip_vertically_on_load(1);
	mLocalBuffer = stbi_load(path.c_str(), &mWidth, &mHeight, &mBitDepth, 1);

	GLCall(glGenTextures(1, &mOpenGLID));
	Bind();

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, mWidth, mHeight, 0, GL_RED, GL_UNSIGNED_BYTE, mLocalBuffer));
	Unbind();

	if (mLocalBuffer) {
		stbi_image_free(mLocalBuffer);
	}
}

void Texture::Delete() const
{
	GLCall(glDeleteTextures(1, &mOpenGLID));
}

void Texture::Bind(unsigned int slot /* = 0 */) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, mOpenGLID));
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
