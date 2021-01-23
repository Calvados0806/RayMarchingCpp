#pragma once

#include <deque>
#include <memory>
#include <string_view>

#include "IShapedObject.h"
#include "OpenGL/ShaderSource.h"

class ShapeRegistrar {
public:
	template <typename ShapeType>
	void RegisterShape(OpenGL::ShaderSource& source)
	{
		source.Substitute(sDistFunctionsMarker, ShapeType::DistFunctionDefinition() + '\n' + sDistFunctionsMarker.data());
	}

	void RegisterObjects(const std::vector<std::shared_ptr<IShapedObject>>& objects, OpenGL::ShaderSource& source);
	void GenerateSceneDistanceFunction(OpenGL::ShaderSource& source);
private:
	std::deque<std::string> mRegisteredFunctions;
	static std::string_view sUniformMarker;
	static std::string_view sDistFunctionsMarker;
	static std::string_view sSceneDistFunctionCodeMarker;
};