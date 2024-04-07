#pragma once

#include <vector>

#include "Header.h"

struct ResourceInfo
{
    string name;

    uint32 set = 0;

    uint32 binding = 0;
};

struct ShaderResourceInfos
{
    std::vector<ResourceInfo> UniformBuffer;
};



class ShaderDecoder
{
public:

    static void GetShaderResourceInfos(const std::vector<uint8>& ShaderBinary, ShaderResourceInfos& out);
};
