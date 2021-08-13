#pragma once
#include "Renderer.h"
#include "BufferLayout.h"

namespace Razix
{
    namespace Graphics
    {
        class Shader;
        class RenderPass;
        class CommandBuffer;
        class DescriptorSet;
        struct VertexInputDescription;
        struct DescriptorLayoutInfo;
        struct DescriptorPoolInfo;

        enum class CullMode
        {
            FRONT,
            BACK,
            FRONTANDBACK,
            NONE
        };

        enum class PolygonMode
        {
            FILL,
            LINE,
            POINT
        };

        struct PipelineInfo
        {
            Ref<RenderPass> renderpass;
            Ref<Shader> shader;

            CullMode cullMode = CullMode::BACK;
            PolygonMode polygonMode = PolygonMode::FILL;
            DrawType drawType = DrawType::TRIANGLE;

            bool transparencyEnabled;
            bool depthBiasEnabled = false;
        };

        class Razix_EXPORT Pipeline
        {
        public:
            static Pipeline* Create(const PipelineInfo& pipelineInfo);
            static Ref<Pipeline> Get(const PipelineInfo& pipelineInfo);
            static void ClearCache();
            static void DeleteUnusedCache();

            virtual ~Pipeline() = default;

            virtual void Bind(CommandBuffer* cmdBuffer) = 0;

            virtual Shader* GetShader() const = 0;

        protected:
            static Pipeline* (*CreateFunc)(const PipelineInfo&);
        };
    }
}
