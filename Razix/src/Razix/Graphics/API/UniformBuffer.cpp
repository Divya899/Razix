#include "Precompiled.h"
#include "UniformBuffer.h"

namespace Razix
{
    namespace Graphics
    {
        UniformBuffer* (*UniformBuffer::CreateFunc)() = nullptr;
        UniformBuffer* (*UniformBuffer::CreateDataFunc)(uint32_t, const void*) = nullptr;

        UniformBuffer* UniformBuffer::Create()
        {
            Razix_ASSERT(CreateFunc, "No UniformBuffer Create Function");

            return CreateFunc();
        }

        UniformBuffer* UniformBuffer::Create(uint32_t size, const void* data)
        {
            Razix_ASSERT(CreateFunc, "No UniformBuffer Create Function");

            return CreateDataFunc(size, data);
        }
    }
}
