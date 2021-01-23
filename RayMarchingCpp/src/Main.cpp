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
    enum class Coord { X, Y, Z };

    void UpdateTranslation(Coord coord, float shift)
    {
        mTranslation[static_cast<unsigned int>(coord)] = shift;
    }

    void UpdateRotation(Coord coord, float angle)
    {
        mRotation[static_cast<unsigned int>(coord)] = angle;
    }

    void UpdateYRotation(int action, float angle)
    {
        if (action == GLFW_PRESS) {
            UpdateRotation(Coord::Y, angle);
        }
        else if (action == GLFW_RELEASE) {
            UpdateRotation(Coord::Y, 0.0f);
        }
    }

    void UpdateXTranslation(int action, float shift)
    {
        if (action == GLFW_PRESS) {
            UpdateTranslation(Coord::X, shift);
        }
        else if (action == GLFW_RELEASE) {
            UpdateTranslation(Coord::X, 0.0f);
        }
    }

    void UpdateYTranslation(int action, float shift)
    {
        if (action == GLFW_PRESS) {
            UpdateTranslation(Coord::Y, shift);
        }
        else if (action == GLFW_RELEASE) {
            UpdateTranslation(Coord::Y, 0.0f);
        }
    }

    void UpdateZTranslation(int action, float shift)
    {
        if (action == GLFW_PRESS) {
            UpdateTranslation(Coord::Z, shift);
        }
        else if (action == GLFW_RELEASE) {
            UpdateTranslation(Coord::Z, 0.0f);
        }
    }

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

        std::shared_ptr<PlaneShape> plane(new PlaneShape(0.0f));
        std::shared_ptr<SphereShape> sphere(new SphereShape(Math::Vec4(0.0f, 1.0f, 6.0f, 1.0f)));
        std::shared_ptr<CubeShape> cube(new CubeShape(Math::Vec4(-3.0f, 0.75f, 6.0f, 0.75f)));

        mShapes.push_back(sphere);
        mShapes.push_back(cube);
        mShapes.push_back(plane);

        mEditableObjects.push_back(sphere);
        mEditableObjects.push_back(cube);

        ShapeRegistrar registrar;
        registrar.Register(mShapes, *mFShaderSource);
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
                UpdateXTranslation(action, mMoveVelocity);
            }},
            {GLFW_KEY_A, [this](int action, int mods) {
                UpdateXTranslation(action, -mMoveVelocity);
            }},
            {GLFW_KEY_LEFT_SHIFT, [this](int action, int mods) {
                UpdateYTranslation(action, mMoveVelocity);
            }},
            {GLFW_KEY_LEFT_CONTROL, [this](int action, int mods) {
                UpdateYTranslation(action, -mMoveVelocity);
            }},
            {GLFW_KEY_W, [this](int action, int mods) {
                UpdateZTranslation(action, mMoveVelocity);
            }},
            {GLFW_KEY_S, [this](int action, int mods) {
                UpdateZTranslation(action, -mMoveVelocity);
            }},
            {GLFW_KEY_LEFT, [this](int action, int mods) {
                UpdateYRotation(action, -sPI / 2);
            }},
            {GLFW_KEY_RIGHT, [this](int action, int mods) {
                UpdateYRotation(action, sPI / 2);
            }},
        };

        return true;
    }

    virtual bool OnUpdate(FrameDuration elapsedTime) override
    {
        float elapsed = elapsedTime.count();

        Math::Vec3 translation = mTranslation * elapsed;
        Math::Vec3 rotation = mRotation * elapsed;

        mCameraRotationY += rotation.y();
        mCameraPos = mCameraPos + translation.RotateY(mCameraRotationY);

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

    Math::Vec4 mCameraPos = { 0.0f, 1.0f, 0.0f, 1.0f };
    float mCameraRotationY = 0.0f;

    Math::Vec3 mTranslation = { 0.0f, 0.0f, 0.0f };
    Math::Vec3 mRotation    = { 0.0f, 0.0f, 0.0f };
    float mMoveVelocity     = 7.5f;

    int mCurrentEditableIndex = -1;
    bool mEnableShadows = true;
    float mSmoothMin = 0.0f;
};

int main(void)
{
    std::unique_ptr<Window> window = Window::Create<RayMarchingWindow>("Ray Marching", 1280, 720);
    window->Run();

    return(0);
}