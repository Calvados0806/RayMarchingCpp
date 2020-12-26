#pragma once

namespace OpenGL {

	class VertexArray;
	class IndexBuffer;
	class ShaderProgram;

	class Renderer {
	public:
		void Draw(const VertexArray& va, const IndexBuffer& ib, const ShaderProgram& shader) const;
		void Clear() const;
	};

}