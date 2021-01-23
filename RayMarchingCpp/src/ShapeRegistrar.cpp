#include "ShapeRegistrar.h"

#include <stack>

std::string_view ShapeRegistrar::sUniformMarker = "/*<uniforms>*/";
std::string_view ShapeRegistrar::sDistFunctionsMarker = "/*<dist_functions>*/";
std::string_view ShapeRegistrar::sSceneDistFunctionCodeMarker = "/*<scene_dist_code>*/";

void ShapeRegistrar::Register(const std::vector<std::shared_ptr<IShapedObject>>& shapes, OpenGL::ShaderSource& source)
{
	for (unsigned int i = 0; i < shapes.size(); i++) {
		std::shared_ptr<IShapedObject> currShape = shapes[i];

		std::string uniforms = currShape->UniformsDefinitions() + '\n' + sUniformMarker.data();
		std::string distFunction = currShape->DistFunctionDefinition() + '\n' + sDistFunctionsMarker.data();

		source.Substitute(sUniformMarker, uniforms);
		source.Substitute(sDistFunctionsMarker, distFunction);

		mRegisteredFunctions.push_back(currShape->DistFunctionName());
	}
}

void ShapeRegistrar::GenerateSceneDistanceFunction(OpenGL::ShaderSource& source)
{
	if (!mRegisteredFunctions.empty()) {
		std::stack<std::string> functionStack;

		for (auto rit = mRegisteredFunctions.rbegin(); rit != mRegisteredFunctions.rend(); rit++) {
			functionStack.push((*rit) + "(cameraPos)");
		}

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
