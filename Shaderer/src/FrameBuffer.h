#pragma once

#include <utility>

class DummyFrameBuffer
{
public:
    DummyFrameBuffer(int width, int height, DummyFrameBuffer* prev)
        :m_Width(width), m_Height(height), m_Previous(prev), m_RendererId(0)
    {
    }
    
    ~DummyFrameBuffer()
    {
    }

    inline virtual void SetFrameBufferResize(unsigned int width, unsigned int height)
    {
        m_Width = width;
        m_Height = height;
    }

    DummyFrameBuffer(const DummyFrameBuffer&) = delete;
protected:
    DummyFrameBuffer* m_Previous;
public:
    unsigned int m_Width, m_Height;
    unsigned int m_RendererId = 0;
};

class ScopedFrameBuffer
{
public:
    ScopedFrameBuffer(unsigned int width, unsigned int height, unsigned int rendererId,
        DummyFrameBuffer* prev);
    ~ScopedFrameBuffer();

    ScopedFrameBuffer(const ScopedFrameBuffer&) = delete;
private:
    DummyFrameBuffer* m_Prev;
};

class FrameBuffer : public DummyFrameBuffer
{
public:
    FrameBuffer(int width, int height, DummyFrameBuffer* prev);
    ~FrameBuffer();

    void SetFrameBufferResize(unsigned int width, unsigned int height) override;
    ScopedFrameBuffer GetScopedFrameBuffer() const;

    FrameBuffer(const FrameBuffer&) = delete;
public:
    unsigned int m_ColorAttachment = 0;
};
