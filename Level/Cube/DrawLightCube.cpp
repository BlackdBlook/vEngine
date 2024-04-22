#include "DrawLightCube.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Engine/vEngine.h"
#include "Engine/Core/Component/Component.h"
#include "Engine/Core/GlobalUniformBuffer/GlobalUniformBufferManager.h"
#include "Engine/Core/Material/Material.h"
#include "Engine/Core/Object/Object.h"
#include "Engine/Core/Texture2D/Texture2D.h"
#include "Meshs/Box/BoxVertices.h"
#include "Meshs/Box/CubeComponent.h"


namespace
{
    class Light : public Component
    {
    public:
        
    };
}

void DrawLightCube::Init()
{
    Level::Init();

    {
        auto obj = NewObject();
        obj->CreateAttach<CubeComponent>("drawtexcube");
    }
    
}
LevelRegister(DrawLightCube);
