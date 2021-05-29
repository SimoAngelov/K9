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

	bool ImGUIWrapper::Init(SDL_Window* ptrWindow, SDL_GLContext ptrContext)
	{
		m_ptrWindow = ptrWindow;
		m_ptrContext = ptrContext;

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
} // namespace K9