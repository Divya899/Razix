#include "Precompiled.h"
#include "CommandBuffer.h"

namespace Razix
{
    namespace Graphics
    {
        CommandBuffer* (*CommandBuffer::CreateFunc)() = nullptr;

        CommandBuffer* CommandBuffer::Create()
        {
            Razix_ASSERT(CreateFunc, "No CommandBuffer Create Function");

            return CreateFunc();
        }
    }
}
