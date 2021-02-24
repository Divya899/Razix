#pragma once

#include "Razix/API/Renderer/GraphicsContext.h"

namespace Razix {namespace Graphics{

	class VulkanContext : public GraphicsContext
	{
	public:
		void Init() override;


		void SwapBuffers() override;

	};

} }

