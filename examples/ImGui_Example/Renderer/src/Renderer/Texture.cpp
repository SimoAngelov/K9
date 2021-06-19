#include "Texture.h"

#include <glad/glad.h>
#include <SDL_image.h>
#include <iostream>

namespace K9
{
	Texture::Texture()
		: m_unTextureID(0),
		m_nWidth(0),
		m_nHeight(0),
		m_eImageInternalFormat(GL_ZERO),
		m_eImageDataFormat(GL_ZERO)
	{
	}

	Texture::~Texture()
	{
		Unload();
	}

	bool Texture::Load(const std::string& strFileName)
	{
		m_strFileName = strFileName;

		/* OpenGl reads the format from bottom to top */
		SDL_Surface* surface = IMG_Load(strFileName.c_str());
		if (!surface)
		{
			std::cerr << "Failed to load texture " << strFileName << "\n";
			return false;
		}
		//FlipSurface(surface);


		m_nWidth = surface->w;
		m_nHeight = surface->h;
		int nChannelsInFile = 0, nFormat = 0;
		if (!GetFormat(surface, nChannelsInFile, nFormat))
		{
			std::cerr << "Failed to get format for " << strFileName << "\n";
			return false;
		}


		glGenTextures(1, &m_unTextureID);
		glBindTexture(GL_TEXTURE_2D, m_unTextureID);

		glTexImage2D(GL_TEXTURE_2D, 0, nFormat, m_nWidth, m_nHeight, 0, nFormat,
			GL_UNSIGNED_BYTE, surface->pixels);

		std::cout << " Loading texture: " << m_strFileName << ", width: " << m_nWidth << ", height: " << m_nHeight
			<< ", nFormat: " << nFormat
			<< ", nChannelsInFile: " << nChannelsInFile << "\n";

		SDL_FreeSurface(surface);
		surface = nullptr;

		/* Generate mipmaps for texture */
		glGenerateMipmap(GL_TEXTURE_2D);
		/* Enable bilinear filtering */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		/* Enable anisotropic filtering, if supported */
		/* Get the maximum anisotropy value */
		GLfloat fMaxAnisotropyValue;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &fMaxAnisotropyValue);
		/* Enable anisotropy */
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, fMaxAnisotropyValue);

		return true;
	}

	void Texture::Unload()
	{
		glDeleteTextures(1, &m_unTextureID);
	}

	void Texture::CreateFromSurface(SDL_Surface* ptrSurface)
	{
		m_nWidth = ptrSurface->w;
		m_nHeight = ptrSurface->h;

		/* Generate a GL Texture */
		glGenTextures(1, &m_unTextureID);
		glBindTexture(GL_TEXTURE_2D, m_unTextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_nWidth, m_nHeight, 0, GL_RGBA,
			GL_UNSIGNED_BYTE, ptrSurface->pixels);
		/* Use linear filtering */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	void Texture::CreateForRendering(int nWidth, int nHeight, int nFormat)
	{
		m_nWidth = nWidth;
		m_nHeight = nHeight;
		/* Create the texture ID */
		glGenTextures(1, &m_unTextureID);
		glBindTexture(GL_TEXTURE_2D, m_unTextureID);
		/* Set the image width/height with nullptr initial data */
		glTexImage2D(GL_TEXTURE_2D, 0, nFormat, m_nWidth, m_nHeight, 0, GL_RGB,
			GL_FLOAT, nullptr);

		/* For a texture we'll render to, just use nearest neighbor */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}

	void Texture::SetActive(int nIndex /*= 0 */) const
	{
		glActiveTexture(GL_TEXTURE0 + nIndex);
		glBindTexture(GL_TEXTURE_2D, m_unTextureID);
	}

	bool Texture::GetFormat(SDL_Surface* ptrSurface, int& nChannelCount, int& nFormat)
	{
		// get the number of channels in the SDL surface
		nChannelCount = ptrSurface->format->BytesPerPixel;
		if (nChannelCount == 4)     // contains an alpha channel
		{
			if (ptrSurface->format->Rmask == 0x000000ff)
			{
				nFormat = GL_RGBA;
			}
			else
			{
				nFormat = GL_BGRA;
			}
		}
		else if (nChannelCount == 3)     // no alpha channel
		{
			if (ptrSurface->format->Rmask == 0x000000ff)
				nFormat = GL_RGB;
			else
				nFormat = GL_BGR;
		}
		else if (nChannelCount == 1)
		{
			if (ptrSurface->format->Rmask == 0x000000ff)
				nFormat = GL_RGBA;
			else
				nFormat = GL_BGRA;
		}
		else {
			std::cerr << "Warning: the image is not truecolor..  this will probably break!\n";
			return false;
		}
		return true;
	}

	void Texture::FlipSurface(SDL_Surface* surface)
	{
		SDL_LockSurface(surface);

		int pitch = surface->pitch; // row size
		char* temp = new char[pitch]; // intermediate buffer
		char* pixels = (char*)surface->pixels;

		for (int i = 0; i < surface->h / 2; ++i) {
			// get pointers to the two rows to swap
			char* row1 = pixels + i * pitch;
			char* row2 = pixels + (surface->h - i - 1) * pitch;

			// swap rows
			memcpy(temp, row1, pitch);
			memcpy(row1, row2, pitch);
			memcpy(row2, temp, pitch);
		}

		delete[] temp;

		SDL_UnlockSurface(surface);
	}
}