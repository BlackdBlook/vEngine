#include "CommonRenderCmd.h"

#include <vulkan/vulkan_core.h>

#include "Engine/Core/FrameInfo/RenderInfo.h"

void CommonRenderCmd::CmdSetFullWindow(const RenderInfo& Info)
{
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) Info.extent.width;
    viewport.height = (float) Info.extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(Info.CommmandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = Info.extent;
    vkCmdSetScissor(Info.CommmandBuffer, 0, 1, &scissor);
}
