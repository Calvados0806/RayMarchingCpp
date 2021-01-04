#include "OpenGL/GLCore.h"
#include "OpenGL/VertexBuffer.h"
#include "OpenGL/IndexBuffer.h"
#include "OpenGL/VertexArray.h"
#include "OpenGL/VertexLayout.h"
#include "OpenGL/ShaderProgram.h"
#include "OpenGL/Renderer.h"
#include "Window.h"
#include "Math/Math.h"

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

            return true;
        }
        catch (std::exception & ex) {
            std::cerr << ex.what() << std::endl;
            return false;
        }
    }

    virtual bool OnUpdate(FrameDuration elapsedTime) override
    {
        float frameMoveVelocity = sMoveVelocity * elapsedTime.count();
        Math::Vec2 direction = (mDirection * frameMoveVelocity);

        mSphereCoords = mSphereCoords + direction;
        mPlaneCoord += direction.y();
        mLightPos = mLightPos + direction;

        shader.Bind();
        shader.SetUniform4f("u_SphereObj", mSphereCoords.x(), mSphereCoords.y(), mSphereCoords.z(), mSphereCoords.w());
        shader.SetUniform1f("u_PlaneObj", mPlaneCoord);
        shader.SetUniform3f("u_LightPos", mLightPos.x(), mLightPos.y(), mLightPos.z());
        mRenderer.Draw(vao, ibo, shader);

        return true;
    }

    virtual void OnKeyEvent(int key, int action, int mods) override
    {
        switch (action)
        {
        case GLFW_PRESS: {
            if (key == GLFW_KEY_D) {
                mDirection.x() = -1.0f;
            }
            else if (key == GLFW_KEY_A) {
                mDirection.x() = 1.0f;
            }
            else if (key == GLFW_KEY_W) {
                mDirection.y() = -1.0f;
            }
            else if (key == GLFW_KEY_S) {
                mDirection.y() = 1.0f;
            }

            break;
        }
        case GLFW_RELEASE: {
            if (key == GLFW_KEY_D || key == GLFW_KEY_A) {
                mDirection.x() = 0.0f;
            }
            else if (key == GLFW_KEY_W || key == GLFW_KEY_S) {
                mDirection.y() = 0.0f;
            }
            break;
        }
        case GLFW_REPEAT:
            break;
        default:
            break;
        }
    }
private:
    std::vector<float> mVertices;
    std::vector<uint32_t> mIndices;

    OpenGL::VertexArray vao;
    OpenGL::VertexBuffer vbo;
    OpenGL::IndexBuffer ibo;
    OpenGL::ShaderProgram shader;

    Math::Vec4 mSphereCoords = { 0.0f, 1.0f, 6.0f, 1.0f };
    float mPlaneCoord = 0.0f;
    Math::Vec3 mLightPos = { 0.0f, 5.0f, 6.0f };

    Math::Vec2 mDirection = { 0.0f, 0.0f };
    float sMoveVelocity = 7.5f;
};

int main(void)
{
    std::unique_ptr<Window> window = Window::Create<RayMarchingWindow>("Ray Marching", 1280, 720);
    window->Run();

    return(0);
}