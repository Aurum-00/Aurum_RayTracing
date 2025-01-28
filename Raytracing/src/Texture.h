#pragma once

#pragma once
#include <filesystem>
#include <string>

enum ETexType
{
    RED,
    RGB,
    RGBA,
    SRGB,
    SRGBA
};

class Texture2D
{
public:
    unsigned int id;
    int             width;
    int             height;
    int             nrChannels;
    ETexType        tex_type = ETexType::SRGBA;
    std::string     path;
    std::string     name = "texture";

public:
    bool LoadTexture2D(const char* path, ETexType type, int GL_TEXTURE_num);

};


