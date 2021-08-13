#include "Precompiled.h"
#include "RenderDevice.h"

namespace Razix
{
    namespace Graphics
    {
        RenderDevice* (*RenderDevice::CreateFunc)() = nullptr;

        RenderDevice* RenderDevice::s_Instance = nullptr;

        void RenderDevice::Create()
        {
            Razix_ASSERT(CreateFunc, "No RenderDevice Create Function");

            s_Instance = CreateFunc();
        }

        void RenderDevice::Release()
        {
            delete s_Instance;
        };
    }
}
