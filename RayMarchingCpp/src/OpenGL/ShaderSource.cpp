#include "ShaderSource.h"

#include <fstream>

using namespace OpenGL;

ShaderSource::ShaderSource(const std::string& source) : mSource(source)
{
}

bool ShaderSource::Substitute(const std::string_view marker, const std::string& code)
{
	size_t iter = mSource.find(marker);
	size_t markerLen = marker.size();
	if (iter == std::string::npos) {
		return false;
	}
	mSource.replace(iter, markerLen, code);

	return true;
}

const std::string& ShaderSource::Get() const
{
	return mSource;
}

std::shared_ptr<ShaderSource> ShaderSource::LoadFrom(const std::string_view path)
{
	std::ifstream in(path.data());
	if (!in.is_open()) {
		return nullptr;
	}
	std::string source;
	std::copy(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>(), std::back_inserter(source));
	return std::make_shared<ShaderSource>(source);
}
