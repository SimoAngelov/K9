#include "Renderer2D.h"
#include <array>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include "Texture.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <imgui.h>
#include <examples/imgui_impl_sdl.h>
#include <examples/imgui_impl_opengl3.h>

#include <examples/imgui_impl_opengl3.cpp>
#include <examples/imgui_impl_sdl.cpp>

namespace K9
{
	Renderer2D& Renderer2D::Ref()
	{
		static Renderer2D ref;
		return ref;
	}

	bool Renderer2D::Init(const std::string& strTitle, int nWidth, int nHeight)
	{
		if (!InitSDL())
		{
			std::cerr << "Renderer2D::Init Failed to init SDL!\n";
			return false;
		}

		if (!InitSDLImage())
		{
			std::cerr << "Renderer2D::Init Failed to init SDL Image!\n";
		}

		if (!InitWindow(strTitle, nWidth, nHeight))
		{
			std::cerr << "Renderer2D::Init Failed to create Window!\n";
			return false;
		}

		/* Create OPENGL Context.*/
		if (!CreateOpenGLContext())
		{
			std::cerr << "Renderer2D::Init Failed to create OpenGLContext!\n";
			return false;
		}

		if (!CreateImGUI())
		{
			std::cerr << "Renderer2D::Init Failed to create ImGUI!\n";
			return false;
		}

		if (!LoadResources())
		{
			std::cerr << "Renderer2D::Init Failed to load resources!\n";
		}

		SetScreenSize({ 0, 0, nWidth, nHeight });

		return true;
	}

	void Renderer2D::Shutdown()
	{
		/* Destroy the window. */
		DestroyWindow();

		/* Destroy the context. */
		DestroyOpenGLContext();

		/* Destroy ImGUI. */
		DestroyImGUI();

		/* Destroy SDL. */
		DestroySDL();

		/* Destroy SDL image. */
		DestroySDLImage();
	}

	SDL_GLContext Renderer2D::GetContext()
	{
		return m_ptrContext;
	}

	SDL_Window* Renderer2D::GetWindow()
	{
		return m_ptrWindow;
	}

	void Renderer2D::BeginFrame()
	{
		/* Clear the screen with the background color. */
		glClear(GL_COLOR_BUFFER_BIT);

		/* Enable Blending. */
		glEnable(GL_BLEND);
		glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

		/* Bind shader and geometry for drawing sprites. */
		m_ptrShader->SetActive();
		m_ptrVertexArray->SetActive();
	}

	void Renderer2D::EndFrame()
	{
		/* Swap OpenGL buffers. */
		SDL_GL_SwapWindow(m_ptrWindow);
	}

	void Renderer2D::BeginImGUIFrame()
	{
		/* Feeds input to dear imgui, starts new frame */
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(m_ptrWindow);
		ImGui::NewFrame();
	}

	void Renderer2D::EndImGUIFrame()
	{
		/* Render dear imgui into screen */
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		ImGuiIO& io = ImGui::GetIO();
		const auto& screenSize = Renderer2D::Ref().GetScreenSize();
		io.DisplaySize = ImVec2(screenSize.w, screenSize.h);

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			auto* currContext = m_ptrContext;
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();

			/* Make this context the current one*/
			int nStatus = SDL_GL_MakeCurrent(m_ptrWindow, currContext);
			if (nStatus != 0)
			{
				std::cerr << "Renderer2D::EndImGUIFrame Failed to make this context curret. SDL error: "
					<< SDL_GetError() << "\n";
			}
		}
	}

	void Renderer2D::HandleEvent(const SDL_Event& event)
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
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

	const SDL_Rect& Renderer2D::GetScreenSize() const
	{
		return m_screenSize;
	}

	void Renderer2D::SetBackgroundColor(const glm::vec4& bgrColor)
	{
		m_bgrColor = bgrColor;
		glClearColor(m_bgrColor.r, m_bgrColor.g, m_bgrColor.b, m_bgrColor.a);
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

	/* Private methods. */
	Renderer2D::Renderer2D()
		: m_ptrWindow{ nullptr }, m_ptrContext{ nullptr }, m_screenSize{},
		m_bgrColor{}, m_projectionMatrix{1.0f}, m_ptrShader{nullptr}, 
		m_ptrVertexArray{nullptr}
	{
	}

	bool Renderer2D::InitSDL()
	{
		/* Initialize SDL with video. */
		int nStatus = SDL_Init(SDL_INIT_VIDEO);
		if (nStatus != 0)
		{
			std::cerr << "Failed to Init SDL!";
			return false;
		}
		return true;
	}

	void Renderer2D::DestroySDL()
	{
		/* Quit SDL. */
		SDL_Quit();
	}

	bool Renderer2D::InitSDLImage()
	{
		/* Load support for the JPGand PNG image formats. */
		int flags = IMG_INIT_JPG | IMG_INIT_PNG;
		int initted = IMG_Init(flags);
		if ((initted & flags) != flags)
		{
			std::cerr << "IMG_Init: Failed to init required jpg and png support!\nIMG_Init: "
				<< IMG_GetError() << "\n";
			return false;
		}
		return true;
	}

	void Renderer2D::DestroySDLImage()
	{
		IMG_Quit();
	}

	bool Renderer2D::InitWindow(const std::string& strTitle, int nWidth, int nHeight)
	{
		m_ptrWindow = SDL_CreateWindow(strTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, nWidth, nHeight, SDL_WINDOW_OPENGL);
		if (!m_ptrWindow)
		{
			std::cerr << "Error failed to create window!\n";
			return false;
		}
		return true;
	}

	void Renderer2D::DestroyWindow()
	{
		/* Destroy the window. */
		SDL_DestroyWindow(m_ptrWindow);
	}

	bool Renderer2D::CreateOpenGLContext()
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
		m_ptrContext = SDL_GL_CreateContext(m_ptrWindow);

		// if we failed to create a context
		if (!m_ptrContext)
		{
			// we'll print out an error message and exit
			std::cerr << "Renderer2D::CreateOpenGLContext Failed to create a context! SDL error: " << SDL_GetError() << "\n";
			return false;
		}

		/* Make this context the current one*/
		int nStatus = SDL_GL_MakeCurrent(m_ptrWindow, m_ptrContext);
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

	bool Renderer2D::CreateImGUI()
	{
		/* Set up Dear ImGui context */
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

		/* Setup Dear ImGui style */
		ImGui::StyleColorsDark();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		std::cout << "this: " << this << "\n";
		/* Set Platform/Renderer bindings.
		 * Window is the SDL_Window*, Context is the SDL_GLContext. */
		ImGui_ImplSDL2_InitForOpenGL(m_ptrWindow, m_ptrContext);
		ImGui_ImplOpenGL3_Init("#version 330");
		return true;
	}

	void Renderer2D::DestroyImGUI()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}

	bool Renderer2D::LoadResources()
	{
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
}