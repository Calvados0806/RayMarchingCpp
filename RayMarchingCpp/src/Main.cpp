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
        try {
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

            shader = OpenGL::ShaderProgram("res/shaders/Vertex.shader", "res/shaders/Fragment.shader");
            shader.Bind();
            shader.SetUniform2f("u_Resolution", static_cast<float>(mWidth), static_cast<float>(mHeight));

            mLightPos.x() += std::sin(40) * 3;
            mLightPos.y() += std::cos(40) * 3;

            mKeyHandlers = {
                {GLFW_KEY_D, [this](int action, int mods) {
                    UpdateXTranslation(action, -mMoveVelocity);
                }},
                {GLFW_KEY_A, [this](int action, int mods) {
                    UpdateXTranslation(action, mMoveVelocity);
                }},
                {GLFW_KEY_LEFT_SHIFT, [this](int action, int mods) {
                    UpdateYTranslation(action, -mMoveVelocity);
                }},
                {GLFW_KEY_LEFT_CONTROL, [this](int action, int mods) {
                    UpdateYTranslation(action, mMoveVelocity);
                }},
                {GLFW_KEY_W, [this](int action, int mods) {
                    UpdateZTranslation(action, -mMoveVelocity);
                }},
                {GLFW_KEY_S, [this](int action, int mods) {
                    UpdateZTranslation(action, mMoveVelocity);
                }},
                {GLFW_KEY_LEFT, [this](int action, int mods) {
                    UpdateYRotation(action, sPI / 2);
                }},
                {GLFW_KEY_RIGHT, [this](int action, int mods) {
                    UpdateYRotation(action, -sPI / 2);
                }},
            };

            return true;
        }
        catch (std::exception & ex) {
            std::cerr << ex.what() << std::endl;
            return false;
        }
    }

    virtual bool OnUpdate(FrameDuration elapsedTime) override
    {
        float elapsed = elapsedTime.count();

        Math::Mat4 viewMatrix = Math::Mat4::CreateTranslation(mTranslation.x() * elapsed,
                                                              mTranslation.y() * elapsed,
                                                              mTranslation.z() * elapsed);
        viewMatrix = viewMatrix.Dot(Math::Mat4::CreateRotationY(mRotation.y() * elapsed));

        mSphereCoords = mSphereCoords.Dot(viewMatrix);
        mLightPos = mLightPos.Dot(viewMatrix);
        mPlaneCoord = mPlaneCoord.Dot(viewMatrix);

        shader.Bind();
        shader.SetUniform4f("u_SphereObj", mSphereCoords.x(), mSphereCoords.y(), mSphereCoords.z(), 1.0f);
        shader.SetUniform1f("u_PlaneObj", mPlaneCoord.y());
        shader.SetUniform3f("u_LightPos", mLightPos.x(), mLightPos.y(), mLightPos.z());
        mRenderer.Draw(vao, ibo, shader);

        return true;
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
    Math::Vec4 mLightPos     = { 0.0f, 5.0f, 6.0f, 1.0f };

    Math::Vec4 mTranslation = { 0.0f, 0.0f, 0.0f, 0.0f };
    Math::Vec4 mRotation    = { 0.0f, 0.0f, 0.0f, 0.0f };
    float mMoveVelocity     = 7.5f;
};

int main(void)
{
    std::unique_ptr<Window> window = Window::Create<RayMarchingWindow>("Ray Marching", 1280, 720);
    window->Run();

    return(0);
}