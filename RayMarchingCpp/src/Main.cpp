#include "OpenGL/GLCore.h"
#include "OpenGL/VertexBuffer.h"
#include "OpenGL/IndexBuffer.h"
#include "OpenGL/VertexArray.h"
#include "OpenGL/VertexLayout.h"
#include "OpenGL/ShaderProgram.h"
#include "OpenGL/Renderer.h"
#include "Window.h"

class RayMarchingWindow : public Window {
public:
    using Window::Window;

    virtual ~RayMarchingWindow()
    {
        shader->Delete();
        ibo->Delete();
        vbo->Delete();
        vao->Delete();
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
            vao = std::make_unique<OpenGL::VertexArray>();
            vbo = std::make_unique<OpenGL::VertexBuffer>(mVertices.data(), mVertices.size() * sizeof(float));

            OpenGL::VertexLayout layout;
            layout.AddAttribute<float>(2);
            vao->LinkLayout(*vbo, layout);

            ibo = std::make_unique<OpenGL::IndexBuffer>(mIndices.data(), mIndices.size());

            shader = std::make_unique<OpenGL::ShaderProgram>("res/shaders/Vertex.shader", "res/shaders/Fragment.shader");
            shader->Bind();
            shader->SetUniform2f("u_Resolution", static_cast<float>(mWidth), static_cast<float>(mHeight));

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
        try {
            mRenderer.Draw(*vao, *ibo, *shader);
            return true;
        }
        catch (std::exception & ex) {
            std::cerr << ex.what() << std::endl;
            return false;
        }
    }
private:
    std::vector<float> mVertices;
    std::vector<uint32_t> mIndices;

    std::unique_ptr<OpenGL::VertexArray> vao;
    std::unique_ptr<OpenGL::VertexBuffer> vbo;
    std::unique_ptr<OpenGL::IndexBuffer> ibo;
    std::unique_ptr<OpenGL::ShaderProgram> shader;
};

int main(void)
{
    std::unique_ptr<Window> window = Window::Create<RayMarchingWindow>("Ray Marching", 1280, 720);
    window->Run();

    return(0);
}