#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <stb_image.h>
#include <algorithm>
#include "Texture.h"

bool Texture2D::LoadTexture2D(const char* path, ETexType type, int GL_TEXTURE_num)
{
    int width, height, nrComponents;
    glGenTextures(1, &this->id);
    // bind Texture
    glActiveTexture(GL_TEXTURE_num);
    glBindTexture(GL_TEXTURE_2D, this->id);

    // 为当前绑定的纹理对象设置环绕、过滤方式
    // 这里只用了一种方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    std::string path_s = path;
    ////replace(FacePath.begin(), FacePath.end(), '\\', '/');   //替换'\'为'/'
    std::replace(path_s.begin(), path_s.end(), '\\', '/');
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path_s.c_str(), &width, &height, &nrComponents, 0);

    if (data)
    {
        GLenum format;
        this->path = path_s;
        this->width = width;
        this->height = height;
        this->nrChannels = nrComponents;

        //判断通道数
        if (nrChannels == 1)
        {
            tex_type = ETexType::RED;
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
        }
        else if (nrChannels == 3)
        {
            if (type == ETexType::SRGB || type == ETexType::SRGBA)
            {
                tex_type = ETexType::SRGB;
                glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            }
            else
            {
                tex_type = ETexType::RGB;
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            }
        }
        else if (nrChannels == 4)
        {
            if (type == ETexType::SRGB || type == ETexType::SRGBA)
            {
                tex_type = ETexType::SRGBA;
                glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            }
            else
            {
                tex_type = ETexType::RGBA;
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            }
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
        stbi_image_free(data);
        return false;
    }
    stbi_image_free(data);
    return true;
}
