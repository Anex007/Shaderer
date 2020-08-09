#include "Texture.h"

#include <glad/glad.h>
#include "stb_image.h"

#include <iostream>

unsigned int Texture::s_NumTexs = 0;

static std::pair<bool, unsigned int> loadTexture(const char* imagePath, int* width, int* height)
{
	unsigned int texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//stbi_set_flip_vertically_on_load(true);
	int nrChannels;
	unsigned char* data = stbi_load(imagePath, width, height, &nrChannels, 0);
	bool ret = false;
	GLint internalFormat;


	if (data) {
		
		switch (nrChannels) {
		case 1:	internalFormat = GL_RED; break;
		case 2:	internalFormat = GL_RG; break;
		case 3:	internalFormat = GL_RGB; break;
		case 4:	internalFormat = GL_RGBA; break;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *width, *height, 0, internalFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		ret = true;
	} else {
		std::cerr << "Failed to load texture: " << imagePath << std::endl;
	}
	stbi_image_free(data);
#ifdef DEBUG
	std::cerr << "Loaded " << imagePath << " Width: " << *width << " Height: " << *height << " Channels: " << nrChannels << std::endl;
#endif
	return std::make_pair(ret, texId);
}

Texture::Texture(const char* imagePath, bool enabled)
	: Enabled(enabled)
{
	m_TexIdx = s_NumTexs++;
	auto [_, texId] = loadTexture(imagePath, &m_Width, &m_Height);
	m_Texture = texId;
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_Texture);
}

bool Texture::ReloadTexture(const char* imagePath)
{
	glDeleteTextures(1, &m_Texture);
	auto [status, texId] = loadTexture(imagePath, &m_Width, &m_Height);
	m_Texture = texId;
	return status;
}

void Texture::Bind() const
{
	glActiveTexture(GL_TEXTURE0 + m_TexIdx);
	glBindTexture(GL_TEXTURE_2D, m_Texture);
}

void Texture::ResetTextures()
{
	s_NumTexs = 0;
}
