#include "Triangle0.h"

#include <stdexcept>

#include "Engine/vEngine.h"
#include "Engine/Core/Component/Component.h"
#include "Engine/Core/FrameInfo/RenderInfo.h"
#include "Engine/Core/Object/Object.h"
#include "Engine/Core/ShaderModule/PipelineShader.h"
#include "Engine/SubSystem/AssetSystem/AssetSystem.h"
#include "Engine/Toolkit/FileToolKit/FileToolKit.h"


class Triangle0_Object : public Component
{
public:
    Triangle0_Object();
    void Draw(const RenderInfo& RenderInfo) override;

    RenderPipeline pipeline;
};

Triangle0_Object::Triangle0_Object() : pipeline("Triangle0")
{
    
}

void Triangle0_Object::Draw(const RenderInfo& RenderInfo)
{
    Component::Draw(RenderInfo);

    pipeline.CmdBind(RenderInfo.CommmandBuffer);

    vkCmdDraw(RenderInfo.CommmandBuffer, 3, 1, 0, 0);
    
}

void Triangle0::Init()
{
    Level::Init();



    {
        auto obj = NewObject();
        obj->AttachComponent(NewSPtr<Triangle0_Object>());
    }
}
