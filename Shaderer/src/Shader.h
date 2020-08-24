#pragma once

#include <glad/glad.h>

#include <string>
#include <map>
#include <vector>

#include "MathVector.h"

typedef const char* FileName;

class Shader
{
public:
    /* Pass in nullptr to skip compiling that shader */
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();

    bool UpdateFragmentSrc(const std::string& fragmentSrc);
    void Bind() const;

    void UploadUniform4f(const char* name, Vec4f& v);
    void UploadUniform3f(const char* name, Vec3f& v);
    void UploadUnifrom2f(const char* name, Vec2f& v);
    void UploadUnifrom2f(const char* name, float x, float y);
    void UploadUniform1f(const char* name, float v);

    void UploadUniform1i(const char* name, int v);
    void UploadUniform1ui(const char* name, unsigned int v);

    void ParseLastError(std::map<int, std::string>& Errs);

private:
    int GetUniformLocation(const char* name);
private:
    std::string m_VertexSrc; /* Could be faster to deal with the shader if we can store the vertexshader */
    unsigned int m_RendererId = 0;
    std::map<std::string, int> m_UniformLocations;
    char m_ErrorStr[512] = { 0 };
};
