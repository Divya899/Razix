#include "Precompiled.h"
#include "Query.h"

#ifdef Razix_RENDER_API_OPENGL
#include "Platform/OpenGL/GLQuery.h"
#endif

#ifdef Razix_RENDER_API_DIRECT3D
#include "Graphics/directx/DXQuery.h"
#endif

#include "Graphics/API/GraphicsContext.h"

namespace Razix
{
    namespace Graphics
    {
        Query* (*Query::CreateFunc)(QueryType) = nullptr;

        Query* Query::Create(QueryType type)
        {
            Razix_ASSERT(CreateFunc, "No Query Create Function");

            return CreateFunc(type);
        }
    }
}
