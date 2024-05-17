#include "vEngine.h"

#include <array>
#include <chrono>
#include <SDL_events.h>

#include "Header.h"
#include "backends/imgui_impl_sdl2.h"
#include "Core/Camera/Camera.h"
#include "Core/CommonRenderCmd/CommonRenderCmd.h"
#include "Core/FrameInfo/FrameInfo.h"
#include "Core/Level/Level.h"
#include "Core/Material/Material.h"
#include "Core/Render/RenderCommandQueue.h"
#include "Core/Render/SceneComponentRenderInfo.h"
#include "Core/UniformBuffer/GlobalUniformBuffer/GlobalUniformBufferManager.h"
#include "Level/Cube/DrawCube.h"
#include "Level/Cube/DrawTexCube.h"
#include "LogPrinter/Log.h"
#include "SubSystem/InputSystem/InputSysten.h"
#include "Toolkit/Timer/Timer.h"
#include "VulkanHelper/VkHelper.h"

class SceneComponentRenderInfo;
uint32 vEngine::WindowX = 2560;
uint32 vEngine::WindowY = 1440;

#define makeLevel(s) \
CurrentLevel.reset();\
CurrentLevel = std::shared_ptr<s>(new s());\
CurrentLevel->Init();\
CurrentLevel->Start()

#define addLevel(s) \
levelList.emplace_back([this]() \
{                               \
    makeLevel(s);               \
})     

vEngine* vEngine::ins = nullptr;


vEngine::vEngine(): vkHelper()
{
    ins = this;

    vkHelper.Init();
    InputSystem::GetInstance()->Init(GetWindow());

    InitLevelList();

    (*levelList)[0](CurrentLevel);
    
    Run();
}

inline float convertToSeconds(std::chrono::microseconds microseconds) {
    return static_cast<float>(microseconds.count()) / 1e6f;
}

void vEngine::processInput()
{
    // if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    //     glfwSetWindowShouldClose(window, true);
    //
    // for (int i = GLFW_KEY_0; i < GLFW_KEY_0 + levelList->size() + 1; i++)
    // {
    //     if (glfwGetKey(window, i) == GLFW_PRESS)
    //     {
    //         SetLevel(i - GLFW_KEY_0);
    //     }
    // }
}

void vEngine::InitLevelList()
{
    levelList = Level::GlobalLevelRegisterList;
}

void vEngine::UpdateLevel(float DeltaTime)
{
    this->DeltaTime = DeltaTime;
    
    CurrentLevel->Update(DeltaTime);

    CurrentLevel->LateUpdate(DeltaTime);
}

void vEngine::DrawLevel(ImGui_ImplVulkanH_Frame* fd)
{
    RenderCommandQueue queue;

    ImGui_ImplVulkanH_Window* wd = &vkHelper.MainWindowData;
    
    {   // Prepass
        {
            std::array<VkClearValue, 2> clearValues = {};
            clearValues[0].color = {0.0f, 0.0f, 0.0f, 1.0f};
            clearValues[1].depthStencil = {1.0f, 0};
            
            VkRenderPassBeginInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            info.renderPass = vkHelper.PreRenderPass;
            info.framebuffer = vkHelper.PreRenderPassFramebuffers[wd->FrameIndex];
            // info.framebuffer = fd->Framebuffer;
            info.renderArea.extent.width = wd->Width;
            info.renderArea.extent.height = wd->Height;
            info.clearValueCount = static_cast<uint32_t>(clearValues.size());
            info.pClearValues = clearValues.data();
        
            vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
        }
        FrameInfo RenderInfo {
            fd->CommandBuffer,
            vkHelper.swapChainExtent,
            {}
        };

        CommonRenderCmd::CmdSetFullWindow(RenderInfo);

        CurrentLevel->Draw(RenderInfo);
        queue.PushRenderCommand(RenderInfo);

        if(queue.SkyRenderInfo)
        {
        
            auto mt = queue.SkyRenderInfo->material;
            mt->Draw(RenderInfo);
        
            queue.SkyRenderInfo->VertexBuffer->CmdBind(RenderInfo.CommmandBuffer);

            vkCmdDraw(RenderInfo.CommmandBuffer, queue.SkyRenderInfo->VertexBuffer->GetVertexNumber(),
                1, 0, 0);
        }

        // Submit command buffer
        vkCmdEndRenderPass(fd->CommandBuffer);
    }

    if(!queue.OpaqueOrderInfo.empty())
    {   // Opaque
        {
            std::array<VkClearValue, 2> clearValues = {};
            clearValues[0].color = {0.0f, 0.0f, 0.0f, 1.0f};
            clearValues[1].depthStencil = {1.0f, 0};
            
            VkRenderPassBeginInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            info.renderPass = vkHelper.OpaqueRenderPass;
            info.framebuffer = vkHelper.OpaqueRenderPassFramebuffers[wd->FrameIndex];
            // info.framebuffer = fd->Framebuffer;
            info.renderArea.extent.width = wd->Width;
            info.renderArea.extent.height = wd->Height;
            info.clearValueCount = static_cast<uint32_t>(clearValues.size());
            info.pClearValues = clearValues.data();
        
            vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
        }

        FrameInfo RenderInfo {
            fd->CommandBuffer,
            vkHelper.swapChainExtent,
            {}
        };

        for(auto it = queue.OpaqueOrderInfo.begin(); it != queue.OpaqueOrderInfo.end(); ++it)
        {
            auto info = it->second;
            auto mt = info->material;
            mt->Draw(RenderInfo);
        
            info->VertexBuffer->CmdBind(fd->CommandBuffer);

            vkCmdDraw(fd->CommandBuffer, info->VertexBuffer->GetVertexNumber(),
                1, 0, 0);
        }

        // Submit command buffer
        vkCmdEndRenderPass(fd->CommandBuffer);
    }

    if(!queue.TranslucentOrderInfo.empty())
    {
        // Translucent
        {
            VkRenderPassBeginInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            info.renderPass = vkHelper.TranslucentRenderPass;
            info.framebuffer = vkHelper.TranslucentRenderPassFramebuffers[wd->FrameIndex];
            // info.framebuffer = fd->Framebuffer;
            info.renderArea.extent.width = wd->Width;
            info.renderArea.extent.height = wd->Height;
            info.clearValueCount = 0;
            info.pClearValues = nullptr;
        
            vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
        }
        FrameInfo RenderInfo {
            fd->CommandBuffer,
            vkHelper.swapChainExtent,
            {}
        };

        for(auto it = queue.TranslucentOrderInfo.cbegin(); it != queue.TranslucentOrderInfo.cbegin(); ++it)
        {
            auto info = it->second;
            auto mt = info->material;
            mt->Draw(RenderInfo);
        
            info->VertexBuffer->CmdBind(fd->CommandBuffer);

            vkCmdDraw(fd->CommandBuffer, info->VertexBuffer->GetVertexNumber(),
                1, 0, 0);
        }

        // Submit command buffer
        vkCmdEndRenderPass(fd->CommandBuffer);
    }
    
}

void vEngine::FrameRender(ImGui_ImplVulkanH_Window* wd, ImDrawData* draw_data)
{
    VkResult err;

    VkSemaphore image_acquired_semaphore  = wd->FrameSemaphores[wd->SemaphoreIndex].ImageAcquiredSemaphore;
    VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
    err = vkAcquireNextImageKHR(GDevice, wd->Swapchain, UINT64_MAX, image_acquired_semaphore, VK_NULL_HANDLE, &wd->FrameIndex);
    if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
    {
        SwapChainRebuild = true;
        return;
    }
    VkHelper::VkHelper::check_vk_result(err);

    ImGui_ImplVulkanH_Frame* fd = &wd->Frames[wd->FrameIndex];
    {
        err = vkWaitForFences(GDevice, 1, &fd->Fence, VK_TRUE, UINT64_MAX);    // wait indefinitely instead of periodically checking
        VkHelper::check_vk_result(err);

        err = vkResetFences(GDevice, 1, &fd->Fence);
        VkHelper::check_vk_result(err);
    }
    {
        err = vkResetCommandPool(GDevice, fd->CommandPool, 0);
        VkHelper::check_vk_result(err);
        VkCommandBufferBeginInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        err = vkBeginCommandBuffer(fd->CommandBuffer, &info);
        VkHelper::check_vk_result(err);
    }

    DrawLevel(fd);
    
    {
        VkRenderPassBeginInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        info.renderPass = wd->RenderPass;
        info.framebuffer = fd->Framebuffer;
        info.renderArea.extent.width = wd->Width;
        info.renderArea.extent.height = wd->Height;
        // info.clearValueCount = 1;
        // info.pClearValues = &wd->ClearValue;
        info.clearValueCount = 0;
        info.pClearValues = nullptr;
        vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
    }

    // Record dear imgui primitives into command buffer
    ImGui_ImplVulkan_RenderDrawData(draw_data, fd->CommandBuffer);
    // Submit command buffer
    vkCmdEndRenderPass(fd->CommandBuffer);

    {
        VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        VkSubmitInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        info.waitSemaphoreCount = 1;
        info.pWaitSemaphores = &image_acquired_semaphore;
        info.pWaitDstStageMask = &wait_stage;
        info.commandBufferCount = 1;
        info.pCommandBuffers = &fd->CommandBuffer;
        info.signalSemaphoreCount = 1;
        info.pSignalSemaphores = &render_complete_semaphore;

        err = vkEndCommandBuffer(fd->CommandBuffer);
        VkHelper::check_vk_result(err);
        err = vkQueueSubmit(vkHelper.Queue, 1, &info, fd->Fence);
        VkHelper::check_vk_result(err);
    }
}

void vEngine::FramePresent(ImGui_ImplVulkanH_Window* wd)
{
    if (SwapChainRebuild)
        return;
    VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
    VkPresentInfoKHR info = {};
    info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    info.waitSemaphoreCount = 1;
    info.pWaitSemaphores = &render_complete_semaphore;
    info.swapchainCount = 1;
    info.pSwapchains = &wd->Swapchain;
    info.pImageIndices = &wd->FrameIndex;
    VkResult err = vkQueuePresentKHR(vkHelper.Queue, &info);
    if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
    {
        SwapChainRebuild = true;
        return;
    }
    
    wd->SemaphoreIndex = (wd->SemaphoreIndex + 1) % wd->SemaphoreCount; // Now we can use the next set of semaphores
}

void vEngine::RebuildSwapChain()
{
    vkHelper.RebuildSwapChain(SwapChainRebuild);
    WindowX = vkHelper.swapChainExtent.width;
    WindowY = vkHelper.swapChainExtent.height;
    Camera::GetCamera()->UpdateProjection();
    GlobalUniformBufferManager::Get()->UpdateCameraData();
}

void vEngine::DrawFrameRateInfoWindow(float DeltaTime)
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize;
    ImGui::Begin("FrameRateInfo", nullptr, window_flags);

    ImGui::Text("DeltaTime: %f", DeltaTime);

    float Fps = 1.0f / DeltaTime;

    ImGui::Text("FPS: %f", Fps);

    ImGui::SetWindowPos(ImVec2{0.0f, 0.0f});
    
    ImGui::End();
}

void vEngine::Run()
{
    constexpr int64 TargetFreamRate = 60;
    constexpr std::chrono::microseconds MinFreamTime =
        std::chrono::microseconds(1000000 / TargetFreamRate);
    Timer t{"MainLoop"};
    // Main loop

    InputSystem * input_system = InputSystem::GetInstance();
    Camera* cam = Camera::GetCamera();
    while(!ExitEngine)
    {
// Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                ExitEngine = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE
                && event.window.windowID == SDL_GetWindowID(vkHelper.window))
                ExitEngine = true;
        }

        // Resize swap chain?
        if (SwapChainRebuild)
        {
            RebuildSwapChain();
        }

        // Start the Dear ImGui frame
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        DrawFrameRateInfoWindow(DeltaTime);

        input_system->Update(DeltaTime);
        
        UpdateLevel(DeltaTime);
        
        cam->Update(DeltaTime);
        
        // Rendering
        ImGui::Render();
        ImDrawData* draw_data = ImGui::GetDrawData();
        const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);
        if (!is_minimized)
        {
            FrameRender(&vkHelper.MainWindowData, draw_data);
            FramePresent(&vkHelper.MainWindowData);
        }

        {
            // MaxFpsControl
            std::chrono::microseconds timeSpan = t.GetTimeSpan();
            while(timeSpan < MinFreamTime)
            {
                timeSpan = t.GetTimeSpan();
            }
            DeltaTime = convertToSeconds(timeSpan);
            FrameCount += 1;
            t.Reset();
        }
    }

    vkHelper.WaitDeviceIdle();

    CurrentLevel.reset();
    
    vkHelper.cleanup();
}

void vEngine::SetLevel(int i)
{
    vkHelper.WaitDeviceIdle();
    if(levelList->size() + 1 < i)
    {
        return;
    }
    (*levelList)[i - 1](CurrentLevel); 
}
