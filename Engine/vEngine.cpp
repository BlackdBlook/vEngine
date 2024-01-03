#include "vEngine.h"

#include <chrono>

#include "Core/CommonRenderCmd/CommonRenderCmd.h"
#include "Core/Level/Level.h"
#include "Level/Triangle0/Triangle0.h"
#include "LogPrinter/Log.h"
#include "Toolkit/Timer/Timer.h"
#include "VulkanHelper/VkHelper.h"

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

vEngine::vEngine(): vkHelper(this)
{
    ins = this;
    window = vkHelper.InitWindow(2560, 1440);
    vkHelper.InitVulkan();
    InitLevelList();
    levelList[0]();


    
    Run();
}

inline float convertToSeconds(std::chrono::microseconds microseconds) {
    return static_cast<float>(microseconds.count()) / 1e6f;
}

void vEngine::processInput()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    for (int i = GLFW_KEY_0; i < GLFW_KEY_0 + levelList.size() + 1; i++)
    {
        if (glfwGetKey(window, i) == GLFW_PRESS)
        {
            SetLevel(i - GLFW_KEY_0); 
        }
    }
}

void vEngine::InitLevelList()
{
    addLevel(Triangle0);
}

void vEngine::UpdateLevel()
{
    
}

void vEngine::DrawLevel()
{
    
}

void vEngine::Run()
{
    constexpr int64 TargetFreamRate = 60;
    constexpr std::chrono::microseconds MinFreamTime =
        std::chrono::microseconds(1000000 / TargetFreamRate);
    Timer t{"MainLoop"};
    
    while(!glfwWindowShouldClose(window))
    {
        t.Reset();

        glfwPollEvents();
        
        processInput();
        
        CurrentLevel->Update(DeltaTime);

        CurrentLevel->LateUpdate(DeltaTime);
        
        DrawFrame();
        
        // MaxFpsControl
        {
            std::chrono::microseconds timeSpan = t.GetTimeSpan();
            while(timeSpan < MinFreamTime)
            {
                timeSpan = t.GetTimeSpan();
            }
            DeltaTime = convertToSeconds(timeSpan);
        }
    }

    vkHelper.WaitDeviceIdle();

    CurrentLevel.reset();
    
    vkHelper.CleanVk();
}

void vEngine::DrawFrame()
{
    const auto RenderInfo = vkHelper.BeginRecordCommandBuffer();

    CommonRenderCmd::CmdSetFullWindow(RenderInfo);

    CurrentLevel->Draw(RenderInfo);

    vkHelper.EndRecordCommandBuffer(RenderInfo);
    
}

void vEngine::SetLevel(int i)
{
    if(levelList.size() + 1 < i)
    {
        return;
    }
    levelList[i - 1](); 
}
