#pragma once

#include "Graphics/API/Pipeline.h"

namespace Razix
{
    namespace Graphics
    {
        class GLRenderPass;
        class CommandBuffer;

        class GLPipeline : public Pipeline
        {
        public:
            GLPipeline(const PipelineInfo& pipelineCreateInfo);
            ~GLPipeline();

            bool Init(const PipelineInfo& pipelineCreateInfo);
            void Bind(Graphics::CommandBuffer* cmdBuffer) override;
            void BindVertexArray();

            Shader* GetShader() const override { return m_Shader; }

            static void MakeDefault();

        protected:
            static Pipeline* CreateFuncGL(const PipelineInfo& pipelineCreateInfo);

        private:
            Shader* m_Shader = nullptr;
            GLRenderPass* m_RenderPass;
            std::string pipelineName;
            bool m_TransparencyEnabled = false;
            uint32_t m_VertexArray = -1;
            BufferLayout m_VertexBufferLayout;
            CullMode m_CullMode;
        };
    }
}
