#pragma once
//Using SDL and standard IO
#include <SDL.h>
#include <SDL_opengl.h>
#include <glm/glm.hpp>

#include <Renderer/Texture.h>

namespace K9
{
	/// <summary>
	/// Main Game Loop
	/// </summary>
	class MainLoop
	{
	public:
		MainLoop();
		virtual ~MainLoop() = default;

		/// <summary>
		/// Initialize member fields.
		/// </summary>
		/// <returns> True, if the initialization was successful. </returns>
		bool Init();

		/// <summary>
		/// Shutdown the main loop and its fields.
		/// </summary>
		void Shutdown();

		/// <summary>
		/// Execute the main loop
		/// </summary>
		void Run();

	private:
		/// <summary>
		/// Handle input events
		/// </summary>
		void HandleEvent();

		/// <summary>
		/// Draw stuff onto the screen and update the window.
		/// </summary>
		void Draw();

		/// <summary>
		/// Draw ImGUI elements.
		/// </summary>
		void OnImGUIRender();

		void DrawColorPickWidget();
		void DrawFoxWidgets();
		void DrawSelectDrawWidget();
		void DrawSrcRectWidget();
		void DrawDestRectWidget();
		void DrawFlipFormatWidget();

	private:

		/// <summary>
		/// Used to store any events from the OS
		/// </summary>
		SDL_Event m_event;

		/// <summary>
		/// Used to determine if we're running the game.
		/// </summary>
		bool m_isRunning; 

		/// <summary>
		/// ImGui color value for the window.
		/// </summary>
		glm::vec4 m_imguiColor;

		Texture m_texFox;

		SDL_Rect m_srcRect;
		SDL_Rect m_destRect;
		SDL_RendererFlip m_flipFormat;
		int m_nDrawIndex;
		int m_nFlipFormatIndex;
	};
} // namespace K9