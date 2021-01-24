#include "OpenGL/GLCore.h"
#include "OpenGL/VertexBuffer.h"
#include "OpenGL/IndexBuffer.h"
#include "OpenGL/VertexArray.h"
#include "OpenGL/VertexLayout.h"
#include "OpenGL/ShaderSource.h"
#include "OpenGL/ShaderProgram.h"
#include "OpenGL/Renderer.h"
#include "Window.h"
#include "Math/Math.h"
#include "IShapedObject.h"
#include "IImGuiEditable.h"
#include "CubeShape.h"
#include "SphereShape.h"
#include "PlaneShape.h"
#include "ShapeRegistrar.h"

#include <functional>
#include <vector>

#include <imgui.h>


class RayMarchingWindow : public Window {
    using Window::Window;
public:
    virtual ~RayMarchingWindow()
    {
        shader.Delete();
        ibo.Delete();
        vbo.Delete();
        vao.Delete();
    }
protected:
    virtual bool OnCreate() override
    {
        mVertices = {
            -1.0f, -1.0f,
             1.0f, -1.0f,
             1.0f,  1.0f,
            -1.0f,  1.0f
        };
        mIndices = { 0, 1, 2, 2, 3, 0 };
        vbo = OpenGL::VertexBuffer(mVertices.data(), mVertices.size() * sizeof(float));

        OpenGL::VertexLayout layout;
        layout.AddAttribute<float>(2);
        vao.LinkLayout(vbo, layout);

        ibo = OpenGL::IndexBuffer(mIndices.data(), mIndices.size());

        mVShaderSource = OpenGL::ShaderSource::LoadFrom("res/shaders/Vertex.shader");
        mFShaderSource = OpenGL::ShaderSource::LoadFrom("res/shaders/Fragment.shader");

        std::shared_ptr<PlaneShape> plane(new PlaneShape(0.0f, "u_PlaneObj"));
        std::shared_ptr<SphereShape> sphere(new SphereShape(Math::Vec4(0.0f, 1.0f, 6.0f, 1.0f), "u_SphereObj"));
        std::shared_ptr<CubeShape> cube(new CubeShape(Math::Vec4(-3.0f, 0.75f, 6.0f, 0.75f), "u_CubeObj"));

        mShapes.push_back(sphere);
        mShapes.push_back(cube);
        mShapes.push_back(plane);

        mEditableObjects.push_back(sphere);
        mEditableObjects.push_back(cube);

        ShapeRegistrar registrar;
        registrar.RegisterShape<PlaneShape>(*mFShaderSource);
        registrar.RegisterShape<SphereShape>(*mFShaderSource);
        registrar.RegisterShape<CubeShape>(*mFShaderSource);

        registrar.RegisterObjects(mShapes, *mFShaderSource);
        registrar.GenerateSceneDistanceFunction(*mFShaderSource);

        std::shared_ptr<OpenGL::ShaderProgram> shader_ptr = OpenGL::ShaderProgram::FromSources(mVShaderSource, mFShaderSource);

        if (!shader_ptr) {
            return false;
        }

        shader = *shader_ptr;

        shader.Bind();
        shader.SetUniform2f("u_Resolution", static_cast<float>(mWidth), static_cast<float>(mHeight));
        shader.SetUniform3f("u_LightPos", mLightPos.x(), mLightPos.y(), mLightPos.z());

        mKeyHandlers = {
            {GLFW_KEY_D, [this](int action, int mods) {
                mCameraDir[0] = (action != GLFW_RELEASE ? mMoveVelocity : 0.0f);
            }},
            {GLFW_KEY_A, [this](int action, int mods) {
                mCameraDir[0] = (action != GLFW_RELEASE ? -mMoveVelocity : 0.0f);
            }},
            {GLFW_KEY_LEFT_SHIFT, [this](int action, int mods) {
                mCameraDir[1] = (action != GLFW_RELEASE ? mMoveVelocity : 0.0f);
            }},
            {GLFW_KEY_LEFT_CONTROL, [this](int action, int mods) {
                mCameraDir[1] = (action != GLFW_RELEASE ? -mMoveVelocity : 0.0f);
            }},
            {GLFW_KEY_W, [this](int action, int mods) {
                mCameraDir[2] = (action != GLFW_RELEASE ? mMoveVelocity : 0.0f);
            }},
            {GLFW_KEY_S, [this](int action, int mods) {
                mCameraDir[2] = (action != GLFW_RELEASE ? -mMoveVelocity : 0.0f);
            }},
            {GLFW_KEY_LEFT, [this](int action, int mods) {
                mRotationY = (action != GLFW_RELEASE ? (-sPI / 2) : 0.0f);
            }},
            {GLFW_KEY_RIGHT, [this](int action, int mods) {
                mRotationY = (action != GLFW_RELEASE ? (sPI / 2) : 0.0f);
            }},
        };

        return true;
    }

    virtual bool OnUpdate(FrameDuration elapsedTime) override
    {
        float elapsed = elapsedTime.count();

        Math::Vec3 direction = mCameraDir * elapsed;
        float rotationY = mRotationY * elapsed;

        mCameraRotationY += rotationY;

        direction = direction.RotateY(mCameraRotationY);
        mCameraPos = mCameraPos + direction;

        shader.Bind();
        shader.SetUniform3f("u_CameraPos", mCameraPos.x(), mCameraPos.y(), mCameraPos.z());
        shader.SetUniform1f("u_CameraRotY", mCameraRotationY);
        shader.SetUniformBool("u_EnableShadows", mEnableShadows);
        shader.SetUniform1f("u_SmoothMinValue", mSmoothMin);

        for (unsigned int i = 0; i < mShapes.size(); i++) {
            mShapes[i]->PassToShader(shader);
        }

        mRenderer.Draw(vao, ibo, shader);

        return true;
    }

    virtual void OnImGuiUpdate()
    {
        ImGui::Begin("Object Editor");

        for (unsigned int i = 0; i < mEditableObjects.size(); i++) {
            if (ImGui::Button(mEditableObjects[i]->SectionName().data())) {
                mCurrentEditableIndex = i;
            }
            ImGui::SameLine();
        }
        ImGui::Checkbox("Shadows", &mEnableShadows);
        ImGui::SliderFloat("Smooth %", &mSmoothMin, 0.0f, 1.0f);

        if (mCurrentEditableIndex >= 0 && mCurrentEditableIndex < mEditableObjects.size()) {
            mEditableObjects[mCurrentEditableIndex]->RenderImGuiEditor();
        }

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Text("\nUse WASD to move through X and Z axes\nUse Shift/Ctrl to move through Y axis\nUse <-/-> (arrows) to rotate the camera");
        ImGui::End();
    }

    virtual void OnKeyEvent(int key, int action, int mods) override
    {
        auto iter = mKeyHandlers.find(key);
        if (iter != mKeyHandlers.end()) {
            iter->second(action, mods);
        }
        return;
    }
private:
    static constexpr double sPI = 3.14159265358979323846;

    std::vector<float> mVertices;
    std::vector<uint32_t> mIndices;

    OpenGL::VertexArray vao;
    OpenGL::VertexBuffer vbo;
    OpenGL::IndexBuffer ibo;
    OpenGL::ShaderProgram shader;

    std::shared_ptr<OpenGL::ShaderSource> mVShaderSource;
    std::shared_ptr<OpenGL::ShaderSource> mFShaderSource;

    std::unordered_map<int, std::function<void(int, int)>> mKeyHandlers;

    std::vector<std::shared_ptr<IShapedObject>> mShapes;
    std::vector<std::shared_ptr<IImGuiEditable>> mEditableObjects;

    Math::Vec4 mLightPos = { (float)std::sin(40)*3, 5.0f + (float)std::cos(40)*3, 6.0f, 1.0f };

    Math::Vec3 mCameraPos  = { 0.0f, 1.0f, 0.0f, 1.0f };
    Math::Vec3 mCameraDir  = { 0.0f, 0.0f, 0.0f, 1.0f };
    float mCameraRotationY = 0.0f;

    float mRotationY    = 0.0f;
    float mMoveVelocity = 7.5f;

    int mCurrentEditableIndex = -1;
    bool mEnableShadows = false;
    float mSmoothMin = 0.0f;
};

int main(void)
{
    std::unique_ptr<Window> window = Window::Create<RayMarchingWindow>("Ray Marching", 1280, 720);
    window->Run();

    return(0);
}