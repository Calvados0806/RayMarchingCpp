#pragma once

#include <string>

namespace OpenGL {

	class Texture {
	public:
		Texture(const std::string& path);
		~Texture() {}

		void Bind(unsigned int slot = 0) const;
		void Unbind() const;
		void Delete() const;

		int Width() const { return mWidth; }
		int Height() const { return mHeight; }
		int BitDepth() const { return mBitDepth; }
	private:
		unsigned int mOpenGLID;
		std::string mFilePath;
		unsigned char* mLocalBuffer;
		int mWidth, mHeight, mBitDepth;
	};

}