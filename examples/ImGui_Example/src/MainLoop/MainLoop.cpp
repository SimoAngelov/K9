#include "MainLoop.h"
#include <iostream>
#include <imgui.h>

#include <Renderer/Renderer2D.h>

namespace K9
{
	MainLoop::MainLoop() : m_event{}, m_isRunning{true}, m_imguiColor{},
		m_srcRect{}, m_destRect{}, m_flipFormat{ SDL_RendererFlip::SDL_FLIP_NONE },
		m_nDrawIndex{0}, m_nFlipFormatIndex{0}
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
		m_srcRect = { 0, 0, m_texFox.GetWidth(), m_texFox.GetHeight() };
		m_destRect = m_srcRect;

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

		if (m_nDrawIndex == 0)
		{
			Renderer2D::Ref().DrawTexture(m_texFox, m_destRect, m_flipFormat);
		}
		else if (m_nDrawIndex == 1)
		{
			Renderer2D::Ref().DrawTexture(m_texFox, m_srcRect, m_destRect, m_flipFormat);
		}

		
		OnImGUIRender();
		Renderer2D::Ref().EndFrame();
	}

	void MainLoop::OnImGUIRender()
	{
		Renderer2D::Ref().BeginImGUIFrame();
		ImGui::ShowDemoWindow(&m_isRunning);

		DrawFoxWidgets();
		DrawColorPickWidget();

		Renderer2D::Ref().EndImGUIFrame();
	}

	void MainLoop::DrawColorPickWidget()
	{
		ImGui::NewLine();
		ImGui::Text("MY CUSTOM COLOR PICKER WITH AN AMAZING PALETTE!");
		ImGui::Separator();
		if (ImGui::ColorPicker4("##picker", (float*)&m_imguiColor))
		{
			Renderer2D::Ref().SetBackgroundColor(m_imguiColor);
		}
	}

	void MainLoop::DrawFoxWidgets()
	{
		ImGui::NewLine();
		ImGui::Separator();
		DrawSelectDrawWidget();
		ImGui::Separator();
		DrawFlipFormatWidget();
		ImGui::Separator();
		DrawSrcRectWidget();
		ImGui::Separator();
		DrawDestRectWidget();

		ImGui::NewLine();
		ImGui::Separator();
		if (ImGui::Button("Reset fox dimensions"))
		{
			m_srcRect = { 0, 0, m_texFox.GetWidth(), m_texFox.GetHeight() };
			m_destRect = m_srcRect;
			m_flipFormat = SDL_RendererFlip::SDL_FLIP_NONE;
		}
	}

	void MainLoop::DrawSelectDrawWidget()
	{
		// List box
		const char* items[] = { "Draw with dest rect", "Draw with source rect and dest rect" };
		if (ImGui::ListBox("Select Draw Method", &m_nDrawIndex, items, IM_ARRAYSIZE(items), 4))
		{
			std::cout << " current draw is: " << m_nDrawIndex << "\n";
		}
	}

	void MainLoop::DrawSrcRectWidget()
	{
		ImGui::NewLine();
		ImGui::Text("Fox Source Rect");
		ImGui::SliderInt("##foxSourceX", &m_srcRect.x, 0, m_texFox.GetWidth(), "m_srcRect.x %d");
		ImGui::SliderInt("##foxSourceY", &m_srcRect.y, 0, m_texFox.GetHeight(), "m_srcRect.y %d");
		ImGui::SliderInt("##foxSourceW", &m_srcRect.w, 0, m_texFox.GetWidth(), "m_srcRect.w %d");
		ImGui::SliderInt("##foxSourceH", &m_srcRect.h, 0, m_texFox.GetHeight(), "m_srcRect.h %d");
	}

	void MainLoop::DrawDestRectWidget()
	{
		ImGui::Text("Fox Dest Rect");
		const auto& screenSize = Renderer2D::Ref().GetScreenSize();
		ImGui::SliderInt("##foxDestX", &m_destRect.x, 0, screenSize.w, "m_destRect.x %d");
		ImGui::SliderInt("##foxDestY", &m_destRect.y, 0, screenSize.h, "m_destRect.y %d");
		ImGui::SliderInt("##foxDestW", &m_destRect.w, 0, screenSize.w, "m_destRect.w %d");
		ImGui::SliderInt("##foxDestH", &m_destRect.h, 0, screenSize.h, "m_destRect.h %d");
	}

	void MainLoop::DrawFlipFormatWidget()
	{
	}
}