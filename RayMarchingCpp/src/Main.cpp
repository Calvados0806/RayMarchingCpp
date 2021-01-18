#include "OpenGL/GLCore.h"
#include "OpenGL/VertexBuffer.h"
#include "OpenGL/IndexBuffer.h"
#include "OpenGL/VertexArray.h"
#include "OpenGL/VertexLayout.h"
#include "OpenGL/ShaderProgram.h"
#include "OpenGL/Renderer.h"
#include "Window.h"
#include "Math/Math.h"

#include <functional>

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

        std::shared_ptr<OpenGL::ShaderProgram> shader_ptr = 
            OpenGL::ShaderProgram::LoadFromFiles("res/shaders/Vertex.shader", "res/shaders/Fragment.shader");

        if (!shader_ptr) {
            return false;
        }

        shader = *shader_ptr;
        shader.Bind();
        shader.SetUniform2f("u_Resolution", static_cast<float>(mWidth), static_cast<float>(mHeight));
        shader.SetUniform4f("u_SphereObj", mSphereCoords.x(), mSphereCoords.y(), mSphereCoords.z(), 1.0f);
        shader.SetUniform1f("u_PlaneObj", mPlaneCoord.y());
        shader.SetUniform3f("u_LightPos", mLightPos.x(), mLightPos.y(), mLightPos.z());
        shader.SetUniform4f("u_CubeObj", mCubeCoords.x(), mCubeCoords.y(), mCubeCoords.z(), 0.75);

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

        shader.SetUniform4f("u_SphereObj", mSphereCoords.x(), mSphereCoords.y(), mSphereCoords.z(), mSphereCoords.w());
        shader.SetUniform1f("u_PlaneObj", mPlaneCoord.y());
        shader.SetUniform3f("u_LightPos", mLightPos.x(), mLightPos.y(), mLightPos.z());
        shader.SetUniform4f("u_CubeObj", mCubeCoords.x(), mCubeCoords.y(), mCubeCoords.z(), mCubeCoords.w());
        mRenderer.Draw(vao, ibo, shader);

        return true;
    }

    enum Object {
        None,
        Sphere,
        Cube
    };

    virtual void OnImGuiUpdate()
    {
        static Object sCurrentObject = None;
        ImGui::Begin("Object Editor");

        if (ImGui::Button("Sphere")) {
            sCurrentObject = Sphere;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cube")) {
            sCurrentObject = Cube;
        }

        switch (sCurrentObject)
        {
        case Sphere: {
            ImGui::SliderFloat("x", &mSphereCoords.x(), -10.0f, 10.0f);
            ImGui::SliderFloat("y", &mSphereCoords.y(), -10.0f, 10.0f);
            ImGui::SliderFloat("z", &mSphereCoords.z(), -10.0f, 10.0f);
            ImGui::SliderFloat("radius", &mSphereCoords.w(), 0.0f, 5.0f);
            break;
        }
        case Cube: {
            ImGui::SliderFloat("x", &mCubeCoords.x(), -10.0f, 10.0f);
            ImGui::SliderFloat("y", &mCubeCoords.y(), -10.0f, 10.0f);
            ImGui::SliderFloat("z", &mCubeCoords.z(), -10.0f, 10.0f);
            ImGui::SliderFloat("size", &mCubeCoords.w(), 0.0f, 5.0f);
            break;
        }
        default:
            break;
        }

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
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

    std::unordered_map<int, std::function<void(int, int)>> mKeyHandlers;

    Math::Vec4 mSphereCoords = { 0.0f, 1.0f, 6.0f, 1.0f };
    Math::Vec4 mPlaneCoord   = { 0.0f, 0.0f, 0.0f, 1.0f };
    Math::Vec4 mLightPos     = { (float)std::sin(40)*3, 5.0f + (float)std::cos(40)*3, 6.0f, 1.0f };
    Math::Vec4 mCubeCoords   = { -3.0f, 0.75f, 6.0f, 0.75f };

    Math::Vec4 mCameraPos = { 0.0f, 1.0f, 0.0f, 1.0f };
    float mCameraRotationY = 0.0f;

    Math::Vec3 mTranslation = { 0.0f, 0.0f, 0.0f };
    Math::Vec3 mRotation    = { 0.0f, 0.0f, 0.0f };
    float mMoveVelocity     = 7.5f;
};

int main(void)
{
    std::unique_ptr<Window> window = Window::Create<RayMarchingWindow>("Ray Marching", 1280, 720);
    window->Run();

    return(0);
}