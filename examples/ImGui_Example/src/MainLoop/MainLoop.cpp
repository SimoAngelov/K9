#include "MainLoop.h"
#include <iostream>
#include <imgui.h>

#include <Renderer/Renderer2D.h>

namespace K9
{
	MainLoop::MainLoop() : m_event{}, m_isRunning{true}, m_imguiColor{}
	{
	}

	bool MainLoop::Init()
	{
		int nWidth = 1280;
		int nHeight = 720;

		if (!Renderer2D::Ref().Init("ImGUI Example", nWidth, nHeight))
		{
			std::cerr << "MainLoop::Init: Failed to init Renderer2D\n";
			return false;
		}

		if (!m_texFox.Load("assets/textures/fox.jpg"))
		{
			std::cerr << "MainLoop::Init: Failed to load m_texFox!\n";
			return false;
		}

		return true;
	}

	void MainLoop::Shutdown()
	{
		Renderer2D::Ref().Shutdown();		
	}

	void MainLoop::Run()
	{
		while (m_isRunning)
		{
			HandleEvent();
			Draw();
		}
	}

	void MainLoop::HandleEvent()
	{
		// poll for events from SDL
		while (SDL_PollEvent(&m_event))
		{
			// determine if the user still wants to have the window open
			// (this basically checks if the user has pressed 'X')
			m_isRunning = (m_event.type != SDL_QUIT);
			Renderer2D::Ref().HandleEvent(m_event);
		}
	}
	void MainLoop::Draw()
	{
		Renderer2D::Ref().BeginFrame();
		Renderer2D::Ref().DrawTexture(m_texFox, { 0, 0, 200, 200 }, SDL_RendererFlip::SDL_FLIP_VERTICAL);
		OnImGUIRender();
		Renderer2D::Ref().EndFrame();
	}
	void MainLoop::OnImGUIRender()
	{
		Renderer2D::Ref().BeginImGUIFrame();
		ImGui::ShowDemoWindow(&m_isRunning);

		ImGui::Text("MY CUSTOM COLOR PICKER WITH AN AMAZING PALETTE!");
		ImGui::Separator();
		if (ImGui::ColorPicker4("##picker", (float*)&m_imguiColor))
		{
			Renderer2D::Ref().SetBackgroundColor(m_imguiColor);
		}
		ImGui::SameLine();

		Renderer2D::Ref().EndImGUIFrame();
	}
}