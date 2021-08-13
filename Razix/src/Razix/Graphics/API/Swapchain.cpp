#include "Precompiled.h"
#include "Swapchain.h"

namespace Razix
{
    namespace Graphics
    {
        Swapchain* (*Swapchain::CreateFunc)(uint32_t, uint32_t) = nullptr;

        Swapchain* Swapchain::Create(uint32_t width, uint32_t height)
        {
            Razix_ASSERT(CreateFunc, "No Swapchain Create Function");

            return CreateFunc(width, height);
        }
    }
}
