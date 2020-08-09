#pragma once

#include <glad/glad.h>
#include <cstddef>
#include <cstdint>
#include <utility>

enum class ShaderDataType
{
    Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
};

struct BufferElement
{
    bool Normalized;
    ShaderDataType Type;

    BufferElement(ShaderDataType type, bool normalized = false)
        :Type(type), Normalized(normalized)
    {

    }

    unsigned int GetCount() const
    {
        switch (Type)
        {
        case ShaderDataType::Float:		return 1;
        case ShaderDataType::Float2:	return 2;
        case ShaderDataType::Float3:	return 3;
        case ShaderDataType::Float4:	return 4;
        case ShaderDataType::Mat3:		return 3 * 3;
        case ShaderDataType::Mat4:		return 4 * 4;
        case ShaderDataType::Int:		return 1;
        case ShaderDataType::Int2:		return 2;
        case ShaderDataType::Int3:		return 3;
        case ShaderDataType::Int4:		return 4;
        case ShaderDataType::Bool:		return 1;
        }
        return 0;
    }
};

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    void Bind() const;
    void UnBind() const;

    void SetLayout(const std::initializer_list<BufferElement>& elements);

private:
    unsigned int m_RendererId;
};


class VertexBuffer
{
public:
    VertexBuffer();
    ~VertexBuffer();

    void Bind() const;
    void UnBind() const;

    void UploadData(const void* data, size_t size, unsigned int usage = GL_STATIC_DRAW);
    void UpdateData(long offset, const void* data, size_t size);

    void Draw(unsigned int numVerticies) const;

private:
    unsigned int m_RendererId;
};

class IndexBuffer
{
public:
    IndexBuffer(uint32_t* indicies, size_t nmemb, unsigned int usage = GL_STATIC_DRAW);
    ~IndexBuffer();

    void Bind() const;
    void UnBind() const;
private:
    unsigned int m_RendererId;
};
