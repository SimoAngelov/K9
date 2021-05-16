#pragma once
#include <string>
#include <glm/glm.hpp>

/* Forward Declarations.*/
struct SDL_Window;

namespace K9
{
	/// <summary>
	/// Handle window creation and update.
	/// </summary>
	class Window
	{
	public:
		/// <summary>
		/// Initialize the main window.
		/// </summary>
		/// <param name="strTitle"> Window Title. </param>
		/// <param name="nWidth"> Window Width. </param>
		/// <param name="nHeight"> Window Height</param>
		/// <returns> True, if the window was created successfully. </returns>
		bool Init(const std::string& strTitle, int nWidth, int nHeight);

		/// <summary>
		/// Destroy the window.
		/// </summary>
		void Shutdown();

		/// <summary>
		/// Update the window.
		/// </summary>
		void Update();

		/// <summary>
		/// Retrieve the SDL window.
		/// </summary>
		/// <returns> m_ptrWindow </returns>
		SDL_Window* GetWindow();

		/// <summary>
		/// Set the window background color.
		/// </summary>
		/// <param name="bgrColor"> Background color to be set. </param>
		void SetBackgroundColor(const glm::vec4& bgrColor);

		/// <summary>
		/// Clear the screen with the background color.
		/// </summary>
		void ClearScreen();

	private:
		/* Main window. */
		SDL_Window* m_ptrWindow;

		/// <summary>
		/// Background color.
		/// </summary>
		glm::vec4 m_bgrColor;
	};
} // namespace K9