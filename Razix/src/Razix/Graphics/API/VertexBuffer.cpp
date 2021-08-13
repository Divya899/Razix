#include "Precompiled.h"
#include "VertexBuffer.h"

namespace Razix
{
    namespace Graphics
    {
        VertexBuffer* (*VertexBuffer::CreateFunc)(const BufferUsage&) = nullptr;

        VertexBuffer* VertexBuffer::Create(const BufferUsage& usage)
        {
            Razix_ASSERT(CreateFunc, "No VertexBuffer Create Function");

            return CreateFunc(usage);
        }
    }
}
