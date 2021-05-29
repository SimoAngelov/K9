#include "MainLoop.h"
#include <iostream>
#include <imgui.h>
#include <SDL_image.h>
#include <Renderer/Renderer2D.h>

namespace K9
{
	MainLoop::MainLoop() : m_window{}, m_imGUI{}, m_event{}, m_isRunning{true}, m_imguiColor{}
	{
	}

	bool MainLoop::Init()
	{
		// Initialize SDL with video
		SDL_Init(SDL_INIT_VIDEO);

		int nWidth = 1280;
		int nHeight = 720;

		if (!m_window.Init("ImGUI Example", nWidth, nHeight))
		{
			std::cerr << "MainLoop: Failed to init m_window!\n";
			return false;
		}

		if (!Renderer2D::Ref().Init(m_window.GetWindow()))
		{
			std::cerr << "MainLoop::Init: Failed to init Renderer2D\n";
			return false;
		}
		Renderer2D::Ref().SetScreenSize(m_window.GetSize());

		if (!m_imGUI.Init(m_window.GetWindow(), Renderer2D::Ref().GetContext()))
		{
			std::cerr << "MainLoop: Failed to init m_window!\n";
			return false;
		}

		// load support for the JPG and PNG image formats
		int flags = IMG_INIT_JPG | IMG_INIT_PNG;
		int initted = IMG_Init(flags);
		if ((initted & flags) != flags)
		{
			std::cerr << "IMG_Init: Failed to init required jpg and png support!\nIMG_Init: "
				<< IMG_GetError() << "\n";
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
		m_window.Shutdown();
		m_imGUI.Shutdown();
		Renderer2D::Ref().Shutdown();

		// And quit SDL
		SDL_Quit();
		IMG_Quit();
	}

	void MainLoop::Run()
	{
		while (m_isRunning)
		{
			HandleEvent();
			Draw();
			m_window.Update();
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
			m_imGUI.HandleEvent(m_event);
		}
	}
	void MainLoop::Draw()
	{
		m_window.ClearScreen();
		Renderer2D::Ref().PrepareDraw();
		Renderer2D::Ref().DrawTexture(m_texFox, { 0, 0, 200, 200 }, SDL_RendererFlip::SDL_FLIP_VERTICAL);
		OnImGUIRender();
	}
	void MainLoop::OnImGUIRender()
	{
		m_imGUI.BeginFrame();
		ImGui::ShowDemoWindow(&m_isRunning);

		ImGui::Text("MY CUSTOM COLOR PICKER WITH AN AMAZING PALETTE!");
		ImGui::Separator();
		if (ImGui::ColorPicker4("##picker", (float*)&m_imguiColor))
		{
			m_window.SetBackgroundColor(m_imguiColor);
		}
		ImGui::SameLine();

		m_imGUI.EndFrame();
	}
}