#include "MainLoop.h"
#include <iostream>
#include <imgui.h>

namespace K9
{
	MainLoop::MainLoop() : m_window{}, m_imGUI{}, m_event{}, m_isRunning{true}, m_imguiColor{}
	{
	}

	bool MainLoop::Init()
	{
		// Initialize SDL with video
		SDL_Init(SDL_INIT_VIDEO);

		if (!m_window.Init("ImGUI Example", 1280, 720))
		{
			std::cerr << "MainLoop: Failed to init m_window!\n";
			return false;
		}
		if (!m_imGUI.Init(m_window.GetWindow()))
		{
			std::cerr << "MainLoop: Failed to init m_window!\n";
			return false;
		}
		return true;
	}

	void MainLoop::Shutdown()
	{
		m_window.Shutdown();
		m_imGUI.Shutdown();

		// And quit SDL
		SDL_Quit();
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