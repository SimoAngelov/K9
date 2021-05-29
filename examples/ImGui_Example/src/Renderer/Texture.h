#pragma once
#include <string>

using GLenum = unsigned int;
struct SDL_Surface;
namespace K9
{
	class Texture
	{
	public:
		Texture();
		~Texture();

		bool Load(const std::string& strFileName);
		void Unload();
		void CreateFromSurface(SDL_Surface* ptrSurface);
		void CreateForRendering(int nWidth, int nHeight, int nFormat);

		void SetActive(int nIndex = 0) const;

		int GetWidth() const { return m_nWidth; }
		int GetHeight() const { return m_nHeight; }
		unsigned int GetTextureID() const { return m_unTextureID; }
		const std::string& GetFileName() const { return m_strFileName; }
	private:
		bool GetFormat(SDL_Surface* ptrSurface, int& nChannelCount, int& nFormat);
		void FlipSurface(SDL_Surface* surface);
	private:
		/* File name of this texture */
		std::string m_strFileName;

		/* OpenGL ID for this texture */
		unsigned int m_unTextureID;

		/* Width/Height of the texture */
		int m_nWidth;
		int m_nHeight;
		GLenum m_eImageInternalFormat;
		GLenum m_eImageDataFormat;
	};
}