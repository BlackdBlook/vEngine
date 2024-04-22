#include "DrawTexCube.h"

#include <stdexcept>

#include "Engine/vEngine.h"
#include "Engine/Core/Component/Component.h"
#include "Engine/Core/GlobalUniformBuffer/GlobalUniformBufferManager.h"
#include "Engine/Core/Material/Material.h"
#include "Engine/Core/Object/Object.h"
#include "Engine/Core/Texture2D/Texture2D.h"
#include "Meshs/Box/BoxVertices.h"
#include "ThirdParty/stb/stb_image.h"

namespace
{
    struct UniformBufferObject
    {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    };
    
    class TexCube : public Component
    {
        SPtr<Material> material;

        MeshVertexBuffer buffer;

        Texture2D texture;
    public:
        TexCube();

        ~TexCube() override;

        virtual void Update(float DeltaTime) override;

        virtual void Draw(const RenderInfo& RenderInfo) override; 
    };
    
    class MaterialRenderPipelineInfoTexCube : public MaterialRenderPipelineInfo
    {
    public:
        MaterialRenderPipelineInfoTexCube(const string& shaderName, ShaderCodeType codeType = ShaderCodeType::HLSL);
        VkDescriptorSetLayout MakeVkDescriptorSetLayout() const override;
    };
    
}


TexCube::TexCube() :
    buffer(sizeof(BoxVertices), BoxVertices),
    texture("container2.png")
{
    UniformBufferObject ubo{};

    {
        SPtr<MaterialRenderPipelineInfo> info = NewSPtr<MaterialRenderPipelineInfoTexCube>("DrawTexCube");
     
        material = std::make_shared<Material>(info);
    }
    
    ubo.view = glm::lookAt(glm::vec3(0.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    ubo.proj = glm::perspective(glm::radians(90.0f), (float)Engine::ins->WindowX / (float)Engine::ins->WindowY, 1.f, 10.0f);

    material->SetAllUniformData("ModelBuffer", "model", MAT4());
    // material->SetAllUniformData("GlobalUniformBuffer", "u_View", ubo.view);
    // material->SetAllUniformData("GlobalUniformBuffer", "u_Projection", ubo.proj);

    GlobalUniformBufferManager::Get()->SetAllBuffer("u_View", ubo.view);
    GlobalUniformBufferManager::Get()->SetAllBuffer("u_Projection", ubo.proj);
}

TexCube::~TexCube()
{
    
}

void TexCube::Update(float DeltaTime)
{
    Component::Update(DeltaTime);
        
    MAT4(m);

    // 平移
    m = glm::translate(m, GetPos());
    
    // 旋转
    glm::quat quat = VEC3_ZERO;
    
    quat *= glm::angleAxis((float)glfwGetTime(), glm::vec3{1, 0, 0});

    glm::mat4 rotationMatrix = glm::mat4_cast(quat);
    m *= rotationMatrix;
    
    material->SetCurrentUniformData("ModelBuffer", "model", m);
    
    if(GFrameCount % 240 == 0)
    {
        material->SetTexture("texture0", "default.jpg");
    }

    if(GFrameCount % 240 == 120)
    {
        material->SetTexture("texture0", "container2.png");
    }
}

void TexCube::Draw(const RenderInfo& RenderInfo)
{
    Component::Draw(RenderInfo);

    material->Draw(RenderInfo);

    buffer.CmdBind(RenderInfo.CommmandBuffer);
    
    vkCmdDraw(RenderInfo.CommmandBuffer, sizeof(BoxVertices) / 8, 1, 0, 0);
}

MaterialRenderPipelineInfoTexCube::MaterialRenderPipelineInfoTexCube(const string& shaderName, ShaderCodeType codeType)
    :MaterialRenderPipelineInfo(shaderName, codeType)
{
    
}

VkDescriptorSetLayout MaterialRenderPipelineInfoTexCube::MakeVkDescriptorSetLayout() const
{
    if(descriptorSetLayout)
    {
        return descriptorSetLayout;
    }

    std::vector<VkDescriptorSetLayoutBinding> out;
    VertShader->FillDescriptorSetLayoutBinding(out);
    FragShader->FillDescriptorSetLayoutBinding(out);
 
    VkDescriptorSetLayoutCreateInfo layoutInfo = {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = (uint32_t)out.size();
    layoutInfo.pBindings = out.data();
 
    if (vkCreateDescriptorSetLayout(GDevice, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
    
    return descriptorSetLayout;
}


void DrawTexCube::Init()
{
    Level::Init();

    {
        auto obj = NewObject();
        obj->Attach(NewSPtr<TexCube>());
    }
}
