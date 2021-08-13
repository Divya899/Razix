#include "Precompiled.h"
#include "GraphicsContext.h"

#ifdef Razix_RENDER_API_OPENGL
#include "Platform/OpenGL/GLContext.h"
#include "Platform/OpenGL/GLFunctions.h"
#endif
#ifdef Razix_RENDER_API_VULKAN
#include "Platform/Vulkan/VKContext.h"
#include "Platform/Vulkan/VKFunctions.h"
#endif
#ifdef Razix_RENDER_API_DIRECT3D
#include "Graphics/DirectX/DXContext.h"
#include "Graphics/DirectX/DXFunctions.h"
#endif

namespace Razix
{
    namespace Graphics
    {
        GraphicsContext* (*GraphicsContext::CreateFunc)(const WindowProperties&, Window*) = nullptr;

        GraphicsContext* GraphicsContext::s_Context = nullptr;
        RenderAPI GraphicsContext::s_RenderAPI;

        void GraphicsContext::Create(const WindowProperties& properties, Window* window)
        {
            Razix_ASSERT(CreateFunc, "No GraphicsContext Create Function");

            s_Context = CreateFunc(properties, window);
        }

        void GraphicsContext::Release()
        {
            delete s_Context;
        }

        GraphicsContext::~GraphicsContext()
        {
        }

        void GraphicsContext::SetRenderAPI(RenderAPI api)
        {
            s_RenderAPI = api;

            switch(s_RenderAPI)
            {
#ifdef Razix_RENDER_API_VULKAN
            case RenderAPI::VULKAN:
                Graphics::Vulkan::MakeDefault();
                break;
#endif

#ifdef Razix_RENDER_API_OPENGL
            case RenderAPI::OPENGL:
                Graphics::GL::MakeDefault();
                break;
#endif

#ifdef Razix_RENDER_API_DIRECT3D
            case RenderAPI::DIRECT3D:
                Graphics::DIRECT3D::MakeDefault();
                break;
#endif
            default:
                break;
            }
        }
    }
}
