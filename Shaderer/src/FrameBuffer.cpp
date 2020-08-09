#include "FrameBuffer.h"

#include <glad/glad.h>
#include <iostream>

FrameBuffer::FrameBuffer(int width, int height, DummyFrameBuffer* next)
    :DummyFrameBuffer(width, height, next)
{
    glGenFramebuffers(1, &m_RendererId);
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId); /* Maybe change read or write target here */

    glGenTextures(1, &m_ColorAttachment);
    glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "Failed to setup the framebuffer completely" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &m_RendererId);
    glDeleteTextures(1, &m_ColorAttachment);
}

void FrameBuffer::SetFrameBufferResize(unsigned int width, unsigned int height)
{
    m_Width = width;
    m_Height = height;

    if (m_RendererId != 0) {
        glDeleteFramebuffers(1, &m_RendererId);
        glDeleteTextures(1, &m_ColorAttachment);
    }
    glGenFramebuffers(1, &m_RendererId);
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId); /* Maybe change read or write target here */

    glGenTextures(1, &m_ColorAttachment);
    glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "Failed to setup the framebuffer completely" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

ScopedFrameBuffer FrameBuffer::GetScopedFrameBuffer() const
{
    return ScopedFrameBuffer(m_Width, m_Height, m_RendererId, m_Previous);
}

ScopedFrameBuffer::ScopedFrameBuffer(unsigned int width, unsigned int height, unsigned int rendererId,
    DummyFrameBuffer* prev)
    : m_Prev(prev)
{
    glBindFramebuffer(GL_FRAMEBUFFER, rendererId);
    glViewport(0, 0, width, height);
}

ScopedFrameBuffer::~ScopedFrameBuffer()
{
    if (m_Prev == nullptr)
        return;
    glBindFramebuffer(GL_FRAMEBUFFER, m_Prev->m_RendererId);
    glViewport(0, 0, m_Prev->m_Width, m_Prev->m_Height);
}
