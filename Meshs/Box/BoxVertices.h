#pragma once

class Box
{
public:
    static float BoxVertices[288];
    static float SkyBoxVertices[108];
};

#define BoxVertices Box::BoxVertices
#define SkyBoxVertices Box::SkyBoxVertices 