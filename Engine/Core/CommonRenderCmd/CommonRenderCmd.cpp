#include "CommonRenderCmd.h"

#include <vulkan/vulkan_core.h>

#include "Engine/vEngine.h"
#include "Engine/Core/FrameInfo/RenderInfo.h"

void CommonRenderCmd::CmdSetFullWindow(const RenderInfo& Info)
{
    VkViewport viewport{};
    viewport.width = (float) vEngine::WindowX;
    // 奇葩Vk的坐标系，y轴向下，所以这里要取负值来翻转视口
    viewport.height = -(float) vEngine::WindowY;
    viewport.x = 0.0f;
    // viewport.y = 0.0f;
    // 因为y轴被翻转，所以这里要取负值来翻转原点
    viewport.y = (float) vEngine::WindowY;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(Info.CommmandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = Info.extent;
    vkCmdSetScissor(Info.CommmandBuffer, 0, 1, &scissor);
}
