#pragma once
#include <memory>
#include "Shader.h"
#include "VertexArray.h"
#include <SDL.h>

namespace K9
{
	class Texture;
	class Window;

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
		bool Init(SDL_Window* window);

		/// <summary>
		/// Free up resources.
		/// </summary>
		void Shutdown();

		SDL_GLContext GetContext();

		/// <summary>
		/// Must be called before drawing any textures.
		/// </summary>
		void PrepareDraw();

		/// <summary>
		/// Set the screen size of the renderer.
		/// </summary>
		/// <param name="screenSize"> Screen size to be set. </param>
		void SetScreenSize(const SDL_Rect& screenSize);


		void DrawTexture(const Texture& texture, const SDL_Rect& destRect,
						const SDL_RendererFlip& flipFormat = SDL_RendererFlip::SDL_FLIP_NONE);

	private:
		Renderer2D();
		virtual ~Renderer2D() = default;

		/// <summary>
		/// Create an OpenGL context.
		/// </summary>
		bool CreateOpenGLContext(SDL_Window* window);

		/// <summary>
		/// Destroy the OpenGL context.
		/// </summary>
		void DestroyOpenGLContext();

	private:
		/// <summary>
		/// Shader class, used to draw a texture.
		/// </summary>
		std::unique_ptr<Shader> m_ptrShader;

		/// <summary>
		/// Texture geometry.
		/// </summary>
		std::unique_ptr<VertexArray> m_ptrVertexArray;

		/// <summary>
		/// Screen size in pixels.
		/// </summary>
		SDL_Rect m_screenSize;

		/// <summary>
		/// Projection matrix.
		/// </summary>
		glm::mat4 m_projectionMatrix;

		/* OpenGL context. */
		SDL_GLContext m_ptrContext;
	};
}