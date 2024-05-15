#include "DrawSkybox.h"

#include "Component/Common/BasicMove/BasicCameraMove.h"
#include "Component/Common/EscExit/PressEscExit.h"
#include "Component/Envriment/Skybox/Skybox.h"
#include "Engine/vEngine.h"
#include "Engine/Core/Component/Component.h"
#include "Engine/Core/FrameInfo/RenderInfo.h"
#include "Engine/Core/Object/Object.h"
#include "Engine/Runtime/Component/CubeComponent.h"
#include "Meshs/Box/BoxVertices.h"


void DrawSkybox::Init()
{
    Level::Init();

    {
        auto obj = NewObject();
        obj->CreateAttach<BasicCameraMove>();
        obj->CreateAttach<PressEscExit>();
    }
    
    {
        auto obj = NewObject();

        obj->CreateAttach<Skybox>("CommonSkyBox");
    }
}

LevelRegister(DrawSkybox);
