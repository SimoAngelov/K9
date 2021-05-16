#include "ImGUIWrapper.h"
#include <iostream>
#include <SDL.h>

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <imgui.h>
#include <examples/imgui_impl_sdl.h>
#include <examples/imgui_impl_opengl3.h>

#include <examples/imgui_impl_opengl3.cpp>
#include <examples/imgui_impl_sdl.cpp>

namespace K9
{
	ImGUIWrapper::ImGUIWrapper() : m_ptrWindow{nullptr}, m_ptrContext{nullptr}
	{
	}

	bool ImGUIWrapper::Init(SDL_Window* ptrWindow)
	{
		m_ptrWindow = ptrWindow;

		/* Create OPENGL Context.*/
		if (!CreateOpenGLContext())
		{
			std::cerr << "ImGuiWrapper::Init Failed to create OpenGLContext!\n";
			return false;
		}

		/* Set up Dear ImGui context */
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		std::cout << "this: " << this << "\n";
		/* Set Platform/Renderer bindings.
		 * Window is the SDL_Window*, Context is the SDL_GLContext. */
		ImGui_ImplSDL2_InitForOpenGL(m_ptrWindow, m_ptrContext);
		ImGui_ImplOpenGL3_Init("#version 330");

		/* Setup Dear ImGui style */
		ImGui::StyleColorsDark();
		return true;
	}
	void ImGUIWrapper::Shutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
		DestroyOpenGLContext();
	}
	void ImGUIWrapper::BeginFrame()
	{
		/* Feeds input to dear imgui, starts new frame */
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(m_ptrWindow);
		ImGui::NewFrame();
	}
	void ImGUIWrapper::EndFrame()
	{
		/* Render dear imgui into screen */
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGUIWrapper::HandleEvent(const SDL_Event& event)
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
	}

	bool ImGUIWrapper::CreateOpenGLContext()
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
			std::cerr << "ImGUIWrapper::CreateOpenGLContext Failed to create a context! SDL error: " << SDL_GetError() << "\n";
			return false;
		}

		/* Make this context the current one*/
		int nStatus = SDL_GL_MakeCurrent(m_ptrWindow, m_ptrContext);
		if (nStatus != 0)
		{
			std::cerr << "ImGUIWrapper::CreateOpenGLContext Failed to make this context curret. SDL error: "
					<< SDL_GetError() << "\n";
			return false;
		}

		/* Initialize GLAD after creating the OpenGL context. */
		nStatus = gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
		if (nStatus == 0)
		{
			std::cerr << "ImGUIWrapper::CreateOpenGLContext Failed to initialize Glad! SDL error: " << SDL_GetError() << "\n";
			return false;
		}

		return true;
	}
	void ImGUIWrapper::DestroyOpenGLContext()
	{
		// Destroy the context
		SDL_GL_DeleteContext(m_ptrContext);
	}
} // namespace K9