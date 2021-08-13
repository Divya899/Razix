#include "Precompiled.h"
#include "DescriptorSet.h"

namespace Razix
{
    namespace Graphics
    {
        DescriptorSet* (*DescriptorSet::CreateFunc)(const DescriptorInfo&) = nullptr;

        DescriptorSet* DescriptorSet::Create(const DescriptorInfo& info)
        {
            Razix_ASSERT(CreateFunc, "No DescriptorSet Create Function");

            return CreateFunc(info);
        }
    }
}
