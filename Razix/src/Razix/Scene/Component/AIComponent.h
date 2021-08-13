#pragma once

#include "AI/AINode.h"

namespace Razix
{
    class Razix_EXPORT AIComponent
    {
    public:
        AIComponent();
        explicit AIComponent(Ref<AINode>& aiNode);

        void OnImGui();

    private:
        Ref<AINode> m_AINode;
    };
}
