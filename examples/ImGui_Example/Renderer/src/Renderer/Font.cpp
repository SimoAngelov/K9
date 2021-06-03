#include "Font.h"
#include <iostream>

namespace K9
{
	Font::Font()
		: m_mapFontData{}, m_strFont{""}
	{
	}

	Font::~Font()
	{
	}

	bool Font::Load(const std::string& strFileName)
	{
		m_mapFontData.clear();
		m_strFont = strFileName;
		/* Support these font sizes */
		std::vector<int> vecFontSizes =
		{
			8, 9, 10, 11, 12, 14, 16, 18, 20, 22, 24, 26, 28,
			30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 52, 56,
			60, 64, 68, 72
		};

		/* Call TTF_OpenFont once per every font size */
		for (auto& nSize : vecFontSizes)
		{
			if (TTF_Font* ptrFont = TTF_OpenFont(m_strFont.c_str(), nSize))
			{
				std::cout << "Loaded font " << m_strFont << " in size " << nSize << "\n";
				m_mapFontData.emplace(nSize, ptrFont);
			}
		}
		if (m_mapFontData.empty())
		{
			std::cerr << "Failed to load font " << m_strFont << "!\n";
			return false;
		}
		return true;
	}

	void Font::Unload()
	{
		for (auto& font : m_mapFontData)
		{
			TTF_CloseFont(font.second);
		}
	}

	std::shared_ptr<Texture> Font::RenderText(const std::string& strText,
												const SDL_Color& color,
												int nPointSize)
	{
		std::shared_ptr<Texture> ptrTexture = nullptr;

		/* Find the font data for this point size */
		auto fontIt = m_mapFontData.find(nPointSize);
		if (fontIt != m_mapFontData.end())
		{
			TTF_Font* ptrFont = fontIt->second;

			/* Draw this to a surface (blended for alpha) */
			SDL_Surface* ptrSurface = TTF_RenderUTF8_Blended(ptrFont, strText.c_str(), color);
			if (ptrSurface)
			{
				/* Convert from surface to texture */
				ptrTexture = std::make_shared<Texture>();
				ptrTexture->CreateFromSurface(ptrSurface);
				SDL_FreeSurface(ptrSurface);
			}
			else
			{
				std::cerr << "Failed to create ttf surface for text  " << strText
					<< ", font: " << m_strFont << "\n";
			}
		}
		else
		{
			std::cerr << "Point size " << nPointSize <<" is unsupported in font: "
				<< m_strFont << "!\n";
		}

		return ptrTexture;
	}
}
