#include "Triangle0.h"

#include <stdexcept>

#include "Engine/vEngine.h"
#include "Engine/Core/Component/Component.h"
#include "Engine/Core/Object/Object.h"
#include "Engine/Core/ShaderModule/PipelineShader.h"
#include "Engine/SubSystem/AssetSystem/AssetSystem.h"
#include "Engine/Toolkit/FileToolKit/FileToolKit.h"


class Triangle0_Object : public Component
{
public:
    Triangle0_Object();
    void Draw() override;
};

Triangle0_Object::Triangle0_Object()
{
    
}

void Triangle0_Object::Draw()
{
    Component::Draw();

    
}

void Triangle0::Init()
{
    Level::Init();



    {
        auto obj = NewObject();
        obj->AttachComponent(NewSPtr<Triangle0_Object>());
    }
}
