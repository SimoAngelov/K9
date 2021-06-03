#pragma once
#include <memory>
#include "Shader.h"
#include "VertexArray.h"
#include <SDL.h>
#include <SDL_image.h>

namespace K9
{
	class Texture;

	/// <summary>
	/// Singleton Renderer, used to draw stuff.
	/// </summary>
	class Renderer2D
	{
	public:
		/** Delete the copy constructor, move constructor and assignment operators. */
		Renderer2D(const Renderer2D&) = delete;
		Renderer2D(Renderer2D&&) = delete;
		Renderer2D& operator=(const Renderer2D&) = delete;
		Renderer2D& operator=(Renderer2D&) = delete;

		/// <summary>
		/// Return a static reference to the singleton instance.
		/// </summary>
		/// <returns> A singleton instance. </returns>
		static Renderer2D& Ref();

		/// <summary>
		/// Init the renderer.
		/// </summary>
		/// <param name="window"> Window reference </param>
		/// <returns> True, if the initialization was successful. </returns>
		bool Init(const std::string& strTitle, int nWidth, int nHeight);

		/// <summary>
		/// Free up resources.
		/// </summary>
		void Shutdown();

		/// <summary>
		/// Retrieve the OpenGL context.
		/// </summary>
		/// <returns> m_ptrContext </returns>
		SDL_GLContext GetContext();

		/// <summary>
		/// Retrieve the SDL window.
		/// </summary>
		/// <returns> m_ptrWindow </returns>
		SDL_Window* GetWindow();

		/// <summary>
		/// Must be called at the start of the draw loop.
		/// </summary>
		void BeginFrame();

		/// <summary>
		/// Must be called at the end of the draw loop.
		/// </summary>
		void EndFrame();

		/// <summary>
		/// Begin an ImGUI frame.
		/// </summary>
		void BeginImGUIFrame();

		/// <summary>
		/// End and ImGUI frame.
		/// </summary>
		void EndImGUIFrame();

		/// <summary>
		/// Handle input events. 
		/// </summary>
		/// <param name="event"> Event to be handled. </param>
		void HandleEvent(const SDL_Event& event);

		/// <summary>
		/// Set the screen size of the renderer.
		/// </summary>
		/// <param name="screenSize"> Screen size to be set. </param>
		void SetScreenSize(const SDL_Rect& screenSize);

		/// <summary>
		/// Retrieve the screen size.
		/// </summary>
		/// <returns> m_screenSize. </returns>
		const SDL_Rect& GetScreenSize() const;

		/// <summary>
		/// Set the window background color.
		/// </summary>
		/// <param name="bgrColor"> Background color to be set. </param>
		void SetBackgroundColor(const glm::vec4& bgrColor);

		/* Draw methods. */
		/// <summary>
		/// Draw a texture with a destination rect and flip format.
		/// </summary>
		void DrawTexture(const Texture& texture, const SDL_Rect& destRect,
						const SDL_Color& color = { 255, 255, 255, 255 },
						const SDL_RendererFlip& flipFormat = SDL_RendererFlip::SDL_FLIP_NONE);
		void DrawTexture(const std::shared_ptr<Texture>& texture,
			const SDL_Rect& destRect,
			const SDL_Color& color = { 255, 255, 255, 255 },
			const SDL_RendererFlip& flipFormat = SDL_RendererFlip::SDL_FLIP_NONE);

		/// <summary>
		/// Draw a texture with a source rect, destination rect and flip format.
		/// </summary>
		void DrawTexture(const Texture& texture, const SDL_Rect& srcRect, const SDL_Rect& destRect,
			const SDL_Color& color = {255, 255, 255, 255},
			const SDL_RendererFlip& flipFormat = SDL_RendererFlip::SDL_FLIP_NONE);
		void DrawTexture(const std::shared_ptr<Texture>& texture,
			const SDL_Rect& srcRect,
			const SDL_Rect& destRect,
			const SDL_Color& color = { 255, 255, 255, 255 },
			const SDL_RendererFlip& flipFormat = SDL_RendererFlip::SDL_FLIP_NONE);

	private:
		Renderer2D();
		virtual ~Renderer2D() = default;

		/// <summary>
		/// Initialize SDL.
		/// </summary>
		/// <returns> True, if the initialization was successful. </returns>
		bool InitSDL();

		/// <summary>
		/// Destroy SDL.
		/// </summary>
		void DestroySDL();

		/// <summary>
		/// Init SDL Image.
		/// </summary>
		/// <returns> True, if the initialization was successful. </returns>
		bool InitSDLImage();

		/// <summary>
		/// Destroy SDL Image.
		/// </summary>
		void DestroySDLImage();

		/// <summary>
		/// Init SDL ttf.
		/// </summary>
		bool InitSDLttf();

		/// <summary>
		/// Destroy SDL ttf.
		/// </summary>
		void DestroySDLttf();

		/// <summary>
		/// Create a window for rendering.
		/// </summary>
		/// <param name="strTitle"> Title of the window. </param>
		/// <param name="nWidth"> Width of the window in pixels. </param>
		/// <param name="nHeight"> Height of the window in pixels. </param>
		/// <returns> True, if the window was created successfully. </returns>
		bool InitWindow(const std::string& strTitle, int nWidth, int nHeight);

		/// <summary>
		/// Destroy the rendering window.
		/// </summary>
		void DestroyWindow();

		/// <summary>
		/// Create an OpenGL context.
		/// </summary>
		/// <returns> True, if the context was created successfully. </returns>
		bool CreateOpenGLContext();

		/// <summary>
		/// Destroy the OpenGL context.
		/// </summary>
		void DestroyOpenGLContext();

		/// <summary>
		/// Create the ImGUI.
		/// </summary>
		/// <returns> True, if the ImGUI was created successfully. </returns>
		bool CreateImGUI();

		/// <summary>
		/// Free up ImGUI resources.
		/// </summary>
		void DestroyImGUI();

		/// <summary>
		/// Load shaders and geometry.
		/// </summary>
		/// <returns> True, if the resources were loaded successfully. </returns>
		bool LoadResources();

	private:
		/// <summary>
		/// Main rendering window.
		/// </summary>
		SDL_Window* m_ptrWindow;
	
		/// <summary>
		/// OpenGL context.
		/// </summary>
		SDL_GLContext m_ptrContext;

		/// <summary>
		/// Window size.
		/// </summary>
		SDL_Rect m_screenSize;

		/// <summary>
		/// Background color.
		/// </summary>
		glm::vec4 m_bgrColor;

		/// <summary>
		/// Projection matrix.
		/// </summary>
		glm::mat4 m_projectionMatrix;

		/// <summary>
		/// Shader class, used to draw a texture.
		/// </summary>
		std::unique_ptr<Shader> m_ptrShader;

		/// <summary>
		/// Texture geometry.
		/// </summary>
		std::unique_ptr<VertexArray> m_ptrVertexArray;
	};
}