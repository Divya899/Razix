#include "Precompiled.h"
#include "Pipeline.h"
#include "Core/Engine.h"

#include "Utilities/CombineHash.h"

namespace Razix
{
    namespace Graphics
    {
        struct PipelineAsset
        {
            Ref<Pipeline> pipeline;
            float timeSinceLastAccessed;
        };
        static std::unordered_map<std::size_t, PipelineAsset> m_PipelineCache;
        static const float m_CacheLifeTime = 3.0f;

        Pipeline* (*Pipeline::CreateFunc)(const PipelineInfo&) = nullptr;

        Pipeline* Pipeline::Create(const PipelineInfo& pipelineInfo)
        {
            Razix_PROFILE_FUNCTION();
            Razix_ASSERT(CreateFunc, "No Pipeline Create Function");
            return CreateFunc(pipelineInfo);
        }

        Ref<Pipeline> Pipeline::Get(const PipelineInfo& pipelineInfo)
        {
            Razix_PROFILE_FUNCTION();
            size_t hash = 0;
            HashCombine(hash, pipelineInfo.shader.get(), pipelineInfo.cullMode, pipelineInfo.depthBiasEnabled, pipelineInfo.drawType, pipelineInfo.polygonMode, pipelineInfo.transparencyEnabled, pipelineInfo.renderpass.get());

            auto found = m_PipelineCache.find(hash);
            if(found != m_PipelineCache.end() && found->second.pipeline)
            {
                found->second.timeSinceLastAccessed = Engine::GetTimeStep().GetElapsedSeconds();
                return found->second.pipeline;
            }

            auto pipeline = Ref<Pipeline>(Create(pipelineInfo));
            m_PipelineCache[hash] = { pipeline, Engine::GetTimeStep().GetElapsedSeconds() };
            return pipeline;
        }

        void Pipeline::ClearCache()
        {
            m_PipelineCache.clear();
        }

        void Pipeline::DeleteUnusedCache()
        {
            Razix_PROFILE_FUNCTION();
            for(const auto& [key, value] : m_PipelineCache)
            {
                if(value.pipeline && value.pipeline.GetCounter()->GetReferenceCount() == 1 && (value.timeSinceLastAccessed - Engine::GetTimeStep().GetElapsedSeconds()) > m_CacheLifeTime)
                    m_PipelineCache.erase(key);
            }
        }
    }
}
