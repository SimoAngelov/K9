#pragma once
//Using SDL and standard IO
#include <SDL.h>
#include <SDL_opengl.h>
#include <glm/glm.hpp>

#include <ImGUIWrapper/ImGUIWrapper.h>
#include <Window/Window.h>

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

	private:
		/// <summary>
		/// Main window.
		/// </summary>
		Window m_window;

		/// <summary>
		/// ImGUI Wrapper
		/// </summary>
		ImGUIWrapper m_imGUI;

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
	};
} // namespace K9