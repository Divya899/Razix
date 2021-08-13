#include "Precompiled.h"
#include "AIComponent.h"

#include <imgui/imgui.h>

namespace Razix
{
    AIComponent::AIComponent()
    {
        m_AINode = CreateRef<AINode>();
    }

    AIComponent::AIComponent(Ref<AINode>& aiNode)
        : m_AINode(aiNode)
    {
    }

    void AIComponent::OnImGui()
    {
    }

}
