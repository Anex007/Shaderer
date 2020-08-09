#pragma once

class Texture
{
public:
	Texture(const char* imagePath, bool enabled = false);
	~Texture();

	bool ReloadTexture(const char* imagePath);

	void Bind() const;

	inline unsigned int GetTexId() const { return m_Texture; }
	inline unsigned int GetTexNum() const { return m_TexIdx; }

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }

	static void ResetTextures();

public:
	bool Enabled;

private:
	static unsigned int s_NumTexs;
	unsigned int m_TexIdx;
	unsigned int m_Texture;
	int m_Width;
	int m_Height;
};
