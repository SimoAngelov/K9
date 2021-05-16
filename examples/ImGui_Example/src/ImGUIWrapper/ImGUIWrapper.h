#pragma once

/* Forward Declarations. */
struct SDL_Window;
typedef union SDL_Event;
typedef void* SDL_GLContext;

namespace K9
{
	class ImGUIWrapper
	{
	public:
		/// <summary>
		/// Default constructor
		/// </summary>
		ImGUIWrapper();
		virtual ~ImGUIWrapper() = default;
	
		/// <summary>
		/// Initialize member fields.
		/// </summary>
		/// <param name="ptrWindow"> Window reference. </param>
		/// <returns> True if the initialization was successful. </returns>
		bool Init(SDL_Window* ptrWindow);

		/// <summary>
		/// Shutdown and free resources.
		/// </summary>
		void Shutdown();

		/// <summary>
		/// Begin an ImGUI frame.
		/// </summary>
		void BeginFrame();

		/// <summary>
		/// End and ImGUI frame.
		/// </summary>
		void EndFrame();

		/// <summary>
		/// Handle input events. 
		/// </summary>
		/// <param name="event"> Event to be handled. </param>
		void HandleEvent(const SDL_Event& event);

	private:
		/// <summary>
		/// Create an OpenGL context.
		/// </summary>
		bool CreateOpenGLContext();

		/// <summary>
		/// Destroy the OpenGL context.
		/// </summary>
		void DestroyOpenGLContext();

	private:
		/* Window reference. */
		SDL_Window* m_ptrWindow;

		/* OpenGL context. */
		SDL_GLContext m_ptrContext;
	};
} // namespace K9