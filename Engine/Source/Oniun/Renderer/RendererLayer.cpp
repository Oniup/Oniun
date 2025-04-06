#include "Oniun/Renderer/RendererLayer.h"

#include <glad/glad.h>

#include "Oniun/Core/Engine.h"
#include "Oniun/Core/Logger.h"
#include "Oniun/Core/String/StringView.h"

namespace Oniun
{
    RendererLayer::RendererLayer(const StringView& winTitle, int32 winWidth, int32 winHeight, int32 winFlags)
        : m_Window(winTitle, winWidth, winHeight, winFlags), m_ImGuiLayer(nullptr), m_Framebuffer(0), m_Renderbuffer(0)
    {
        m_GraphicsContext.Initialize();
    }

    RendererLayer::~RendererLayer()
    {
        m_Shader.Destroy();
        m_Texture.Destroy();
        m_VertexBuffer.Destroy();
        glDeleteFramebuffers(1, &m_Framebuffer);

        m_ImGuiLayer = nullptr;
    }

    void RendererLayer::SetImGuiWindowLayer(ImGuiLayer* layer)
    {
        ASSERT(!m_ImGuiLayer && "Cannot have two ImGui contexts");
        m_ImGuiLayer = layer;
    }

    void RendererLayer::OnStart()
    {
        // TODO: Remove later when finished testing
        Array<Pair<ShaderStage, String>> sources;
        sources.Add(RHI::Shader::GetSourceFromFile(StringView(ONIUN_RESOURCE_DIRECTORY "/Engine/Shaders/Phong.vert")));
        sources.Add(RHI::Shader::GetSourceFromFile(StringView(ONIUN_RESOURCE_DIRECTORY "/Engine/Shaders/Phong.frag")));
        for (auto&[type, src] : sources)
            LOG(Info, "{}:\n{}", type, src);

        m_Shader.LoadFromSource(sources);

        int32 width, height;
        ColorFormat format;
        uint8* imageData = RHI::Texture::ReadImageData(ONIUN_RESOURCE_DIRECTORY "/Engine/Textures/Prototype/Dark/texture_03.png", &width, &height, &format);
        m_Texture.CreateFromImage(imageData, format, width, height, TextureMode::Repeat, TextureFilter::Nearest, TextureFilter::NearestMipmapNearest);
        RHI::Texture::DestroyImageData(imageData);

        float vertices[] = {
            0.5f, 0.5f, 0.0f,   1.0f, 1.0f, // top right
            0.5f, -0.5f, 0.0f,  1.0f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
            -0.5f, 0.5f, 0.0f,  0.0f, 1.0f  // top left
        };

        uint32_t indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3 // second triangle
        };

        m_VertexBuffer.Create(sizeof(float) * 5, BufferDrawType::StaticDraw, true);
        m_VertexBuffer.AddAttribute(3);
        m_VertexBuffer.AddAttribute(2);
        m_VertexBuffer.SetData(GpuBuffer_Vertex, (byte*)vertices, sizeof(vertices));
        m_VertexBuffer.SetData(GpuBuffer_Element, (byte*)indices, sizeof(indices));

        glGenFramebuffers(1, &m_Framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);

        m_FramebufferTexture.CreateEmpty(ColorFormat::RGB, m_Window.GetWidth(), m_Window.GetHeight(), TextureMode::ClampToEdge, TextureFilter::Linear, TextureFilter::None);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_FramebufferTexture.GetGpuId(), 0);

        if (!glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
            LOG(Error, "Failed to setup framebuffer");
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void RendererLayer::OnUpdate()
    {
        if (!m_Window.IsOpen())
            Engine::Quit();

        m_Window.PollEvents();

        // TODO: Remove into the graphics context
        glUseProgram(m_Shader.GetGpuId());
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_Texture.GetGpuId());
        glBindVertexArray(m_VertexBuffer.GetGpuId());
        glDrawElements(GL_TRIANGLES, 6 , GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        if (m_ImGuiLayer)
        {
            m_ImGuiLayer->Render(*this);
            m_ImGuiLayer->RenderPlatformDrawData();
            m_ImGuiLayer->UpdatePlatformWindows();
        }

        m_Window.SwapBuffers();
        glClearColor(0.2f, 0.5f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glViewport(0, 0, m_Window.GetWidth(), m_Window.GetHeight());
    }
}
