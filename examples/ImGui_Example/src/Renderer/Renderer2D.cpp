#include "Renderer2D.h"
#include <array>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include "Texture.h"
#include <Window/Window.h>

namespace K9
{
	Renderer2D& Renderer2D::Ref()
	{
		static Renderer2D ref;
		return ref;
	}

	bool Renderer2D::Init(SDL_Window* window)
	{
		/* Create OPENGL Context.*/
		if (!CreateOpenGLContext(window))
		{
			std::cerr << "Renderer2D::Init Failed to create OpenGLContext!\n";
			return false;
		}

		m_ptrShader.reset(new Shader());
		if (!m_ptrShader->Load("assets/shaders/Sprite.vert", "assets/shaders/Sprite.frag"))
		{
			return false;
		}


		constexpr unsigned int unVerticesCount = 4;
		constexpr unsigned int unIndicesCount = 6;
		float arrVertices[] = {
			-0.5f, 0.5f, 0.f, 0.f, 0.f, /* Top-Left */
			 0.5f, 0.5f, 0.f, 1.f, 0.f,	/* Top-Right */
			 0.5f,-0.5f, 0.f, 1.f, 1.f,	/* Bottom-Right */
			-0.5f,-0.5f, 0.f, 0.f, 1.f  /* Bottom-Left */
		};

		unsigned int arrIndices[] = {
			0, 1, 2,
			2, 3, 0
		};

		m_ptrVertexArray.reset(new VertexArray(arrVertices, unVerticesCount,
			VertexArray::ELayout::ePosTex, arrIndices, unIndicesCount));

		return true;
	}

	void Renderer2D::Shutdown()
	{
		DestroyOpenGLContext();
	}

	SDL_GLContext Renderer2D::GetContext()
	{
		return m_ptrContext;
	}

	void Renderer2D::PrepareDraw()
	{
		glEnable(GL_BLEND);
		glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

		m_ptrShader->SetActive();
		m_ptrVertexArray->SetActive();
	}

	void Renderer2D::SetScreenSize(const SDL_Rect& screenSize)
	{
		m_screenSize = screenSize;
		glViewport(m_screenSize.x, m_screenSize.y, m_screenSize.w, m_screenSize.h);
		m_projectionMatrix = glm::ortho(static_cast<float>(m_screenSize.x),
			static_cast<float>(m_screenSize.w),
			static_cast<float>(m_screenSize.h),
			static_cast<float>(m_screenSize.y),
			-1.0f,
			1.0f);
	}

	void Renderer2D::DrawTexture(const Texture& texture, const SDL_Rect& destRect,
								const SDL_RendererFlip& flipFormat)
	{
		/* Set identity matrix. */
		auto trans = glm::mat4(1.0f);
		/* Draw from top left. */
		trans = glm::translate(trans, glm::vec3(texture.GetWidth() * 0.5f, texture.GetHeight() * 0.5f, 0.0f));

		/* Translate to x, y. */
		trans = glm::translate(trans, glm::vec3(destRect.x, destRect.y, 0.0f));

		/* Scale by w, y */
		auto scale = glm::vec3(destRect.w, destRect.h, 1.0f);
		
		/* Set flip format. */
		switch (flipFormat)
		{
		case SDL_RendererFlip::SDL_FLIP_HORIZONTAL: scale.x *= -1; break;
		case SDL_RendererFlip::SDL_FLIP_VERTICAL: scale.y *= -1; break;
		default: break;
		}
		trans = glm::scale(trans, scale);

		 /* Set world transform */
		m_ptrShader->SetMatrixUniform("u_WorldTransform", trans);
		m_ptrShader->SetMatrixUniform("u_ViewProj", m_projectionMatrix);

		/* Set current texture */
		texture.SetActive();

		/* Draw quad */
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}

	Renderer2D::Renderer2D()
		: m_ptrShader{ nullptr }, m_ptrVertexArray{ nullptr },
		m_screenSize{}, m_projectionMatrix{1.0f}, m_ptrContext{nullptr}
	{
	}


	bool Renderer2D::CreateOpenGLContext(SDL_Window* window)
	{
		/* Set OpenGL attributes */
		/* Use the core OpenGL profile */
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		/* Specify version 3.3 */
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		/* Request a color buffer with 8-bits per RGBA channel */
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		/* Enable double buffering */
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		/* Force OpenGL to use hardware acceleration */
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

		// Create an OpenGL context (so we can use OpenGL functions)
		m_ptrContext = SDL_GL_CreateContext(window);

		// if we failed to create a context
		if (!m_ptrContext)
		{
			// we'll print out an error message and exit
			std::cerr << "Renderer2D::CreateOpenGLContext Failed to create a context! SDL error: " << SDL_GetError() << "\n";
			return false;
		}

		/* Make this context the current one*/
		int nStatus = SDL_GL_MakeCurrent(window, m_ptrContext);
		if (nStatus != 0)
		{
			std::cerr << "Renderer2D::CreateOpenGLContext Failed to make this context curret. SDL error: "
				<< SDL_GetError() << "\n";
			return false;
		}

		/* Initialize GLAD after creating the OpenGL context. */
		nStatus = gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
		if (nStatus == 0)
		{
			std::cerr << "Renderer2D::CreateOpenGLContext Failed to initialize Glad! SDL error: " << SDL_GetError() << "\n";
			return false;
		}

		return true;
	}
	void Renderer2D::DestroyOpenGLContext()
	{
		// Destroy the context
		SDL_GL_DeleteContext(m_ptrContext);
	}
}