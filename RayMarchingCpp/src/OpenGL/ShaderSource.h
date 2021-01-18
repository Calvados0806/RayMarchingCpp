#pragma once

#include <string>
#include <string_view>
#include <memory>

namespace OpenGL {

	class ShaderSource {
	public:
		ShaderSource(const std::string& source);

		bool Substitute(const std::string_view marker, const std::string& code);
		const std::string& Get() const;

		static std::shared_ptr<ShaderSource> LoadFrom(const std::string_view path);
	private:
		std::string mSource;
	};
}