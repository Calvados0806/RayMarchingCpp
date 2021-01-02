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
        (void)elapsedTime;
        mRenderer.Draw(vao, ibo, shader);
        return true;
    }

    virtual void OnKeyEvent(int key, int action, int mods) override
    {
        const char* action_str;
        switch (action)
        {
        case GLFW_PRESS:
            action_str = "PRESS";
            break;
        case GLFW_RELEASE:
            action_str = "RELEASE";
            break;
        case GLFW_REPEAT:
            action_str = "REPEAT";
            break;
        default:
            action_str = "UNKNOWN";
            break;
        }
        std::cout << action_str << " event for key: " << key << std::endl;
    }
private:
    std::vector<float> mVertices;
    std::vector<uint32_t> mIndices;

    OpenGL::VertexArray vao;
    OpenGL::VertexBuffer vbo;
    OpenGL::IndexBuffer ibo;
    OpenGL::ShaderProgram shader;
};

int main(void)
{
    std::unique_ptr<Window> window = Window::Create<RayMarchingWindow>("Ray Marching", 1280, 720);
    window->Run();

    return(0);
}