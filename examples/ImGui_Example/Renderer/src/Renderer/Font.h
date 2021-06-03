#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <SDL_ttf.h>
#include <glm/glm.hpp>
#include <Renderer/Texture.h>

namespace K9
{
	class Font
	{
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		Font();
		~Font();

		/// <summary>
		/// Load a font from a file.
		/// </summary>
		bool Load(const std::string& strFileName);

		/// <summary>
		/// Unload the font.
		/// </summary>
		void Unload();

		/* Given a string and this font, draw to a texture */
		std::shared_ptr<Texture> RenderText(
			const std::string& strText,
			const SDL_Color& color = {255, 255, 255, 255},
			int nPointSize = 30);

	private:
		/// <summary>
		///  Map of point sizes to font data.
		/// </summary>
		std::unordered_map<int, TTF_Font*> m_mapFontData;

		/// <summary>
		/// Name of the font.
		/// </summary>
		std::string m_strFont;
	};
}
