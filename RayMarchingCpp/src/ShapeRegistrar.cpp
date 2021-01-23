#include "ShapeRegistrar.h"

#include <stack>

std::string_view ShapeRegistrar::sUniformMarker = "/*<uniforms>*/";
std::string_view ShapeRegistrar::sDistFunctionsMarker = "/*<dist_functions>*/";
std::string_view ShapeRegistrar::sSceneDistFunctionCodeMarker = "/*<scene_dist_code>*/";

void ShapeRegistrar::RegisterObjects(const std::vector<std::shared_ptr<IShapedObject>>& objects, OpenGL::ShaderSource& source)
{
	for (unsigned int i = 0; i < objects.size(); i++) {
		std::shared_ptr<IShapedObject> obj = objects[i];
		std::string uniforms = obj->UniformsDefinitions() + '\n' + sUniformMarker.data();

		source.Substitute(sUniformMarker, uniforms);
		mRegisteredFunctions.push_front(obj->DistFunctionCall("cameraPos"));
	}
}

void ShapeRegistrar::GenerateSceneDistanceFunction(OpenGL::ShaderSource& source)
{
	if (!mRegisteredFunctions.empty()) {
		std::stack<std::string> functionStack(mRegisteredFunctions);

		while (true) {
			std::string func1 = functionStack.top();
			functionStack.pop();

			if (functionStack.empty()) {
				source.Substitute(sSceneDistFunctionCodeMarker, "return " + func1 + ';');
				return;
			}
			std::string func2 = functionStack.top();
			functionStack.pop();
			functionStack.push("smin("+func1+','+func2+", u_SmoothMinValue)");
		}
	}
}
