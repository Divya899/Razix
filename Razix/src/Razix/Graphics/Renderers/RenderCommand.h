#pragma once

#include "Graphics/Mesh.h"
#include "Graphics/API/Shader.h"

namespace Razix
{

    namespace Graphics
    {
        class Material;

        struct Razix_EXPORT RenderCommand
        {
            Mesh* mesh = nullptr;
            Material* material = nullptr;
            Maths::Matrix4 transform;
            Maths::Matrix4 textureMatrix;
            bool animated = false;
        };
    }
}
