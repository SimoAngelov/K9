#include "MainLoop.h"
#include <iostream>
#include <imgui.h>

#include <Renderer/Renderer2D.h>
#include <Audio/Audio.h>

namespace K9
{
	MainLoop::MainLoop() : m_event{}, m_isRunning{true}, m_imguiColor{},
		m_srcRect{}, m_destRect{}, m_flipFormat{ SDL_RendererFlip::SDL_FLIP_NONE },
		m_nDrawIndex{ 0 }, m_nFlipFormatIndex{ 0 }, m_font{}, m_text{ nullptr },
		m_strText{ "" }, m_textColor{ 255, 255, 255, 255 }, m_textRect{}
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
		
		if (!Music::Ref().Init("assets/sounds/music.csv"))
		{
			std::cerr << "MainLoop::Init: Failed to init Audio\n";
			return false;
		}

		if (!m_texFox.Load("assets/textures/fox.jpg"))
		{
			std::cerr << "MainLoop::Init: Failed to load m_texFox!\n";
			return false;
		}
		if (!m_font.Load("assets/fonts/BLKCHCRY.TTF"))
		{
			std::cerr << "MainLoop::Init: Failed to init m_font!\n";
			return false;
		}
		m_srcRect = { 0, 0, m_texFox.GetWidth(), m_texFox.GetHeight() };
		m_destRect = m_srcRect;

		m_strText = "Foxy Fox";
		UpdateText(true);

		return true;
	}

	void MainLoop::Shutdown()
	{
		m_font.Unload();
		Renderer2D::Ref().Shutdown();
		Music::Ref().Shutdown();
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
			Renderer2D::Ref().DrawTexture(m_texFox, m_destRect,
				SDL_Color{ 255, 255, 255, 255 }, m_flipFormat);
		}
		else if (m_nDrawIndex == 1)
		{
			Renderer2D::Ref().DrawTexture(m_texFox, m_srcRect, m_destRect,
				SDL_Color{ 255, 255, 255, 255 }, m_flipFormat);
		}


		Renderer2D::Ref().DrawTexture(m_text, m_textRect, m_textColor);
		
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

	void MainLoop::DrawAudioWidget()
	{
		static constexpr auto SOUND_ID{ "Sonic_Blaster" };
		auto& music = Music::Ref();

		if (ImGui::Button("Play sound"))
		{
			music.Play(SOUND_ID);
		}
		if (ImGui::Button("Pause sound"))
		{
			music.Pause();
		}
		if (ImGui::Button("Resume sound"))
		{
			music.Unpause();
		}
		if (ImGui::Button("Stop sound"))
		{
			music.Stop();
		}

		static int nVolume = music.GetVolume();
		if (ImGui::SliderInt("##setVolume", &nVolume, 0,
			music.GetMaxVolume(), "The volume is %d"))
		{
			music.SetVolume(nVolume);
		}
	}

	void MainLoop::DrawColorPickWidget()
	{
		ImGui::NewLine();
		ImGui::Text("MY CUSTOM COLOR PICKER WITH AN AMAZING PALETTE!");
		ImGui::Separator();
		if (ImGui::ColorPicker4("##picker", (float*)&m_imguiColor))
		{
			Renderer2D::Ref().SetBackgroundColor(m_imguiColor);
			m_textColor.r = std::abs(255 - 255 * m_imguiColor.r);
			m_textColor.g = std::abs(255 - 255 * m_imguiColor.g);
			m_textColor.b = std::abs(255 - 255 * m_imguiColor.b);
		}
	}

	void MainLoop::DrawFoxWidgets()
	{
		ImGui::NewLine();
		ImGui::Separator();
		DrawAudioWidget();
		ImGui::NewLine();
		ImGui::Separator();
		DrawSelectDrawWidget();
		ImGui::Separator();
		DrawFlipFormatWidget();
		ImGui::Separator();
		DrawSrcRectWidget();
		ImGui::Separator();
		DrawDestRectWidget();
		ImGui::Separator();
		DrawTextWidget();

		ImGui::NewLine();
		ImGui::Separator();
		if (ImGui::Button("Reset fox dimensions"))
		{
			m_srcRect = { 0, 0, m_texFox.GetWidth(), m_texFox.GetHeight() };
			m_destRect = m_srcRect;
			m_flipFormat = SDL_RendererFlip::SDL_FLIP_NONE;
			if (m_text)
			{
				m_textRect = { 0, 0, m_text->GetWidth(), m_text->GetHeight() };
			}
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
		if (ImGui::SliderInt("##foxSourceX", &m_srcRect.x, 0, m_texFox.GetWidth(), "m_srcRect.x %d"))
		{
			if (m_srcRect.x + m_srcRect.w > m_texFox.GetWidth())
			{
				m_srcRect.w = m_texFox.GetWidth() - m_srcRect.x;
			}
		}

		if (ImGui::SliderInt("##foxSourceY", &m_srcRect.y, 0, m_texFox.GetHeight(), "m_srcRect.y %d"))
		{
			if (m_srcRect.y + m_srcRect.h > m_texFox.GetHeight())
			{
				m_srcRect.h = m_texFox.GetHeight() - m_srcRect.y;
			}
		}
		
		if (ImGui::SliderInt("##foxSourceW", &m_srcRect.w, 0, m_texFox.GetWidth(), "m_srcRect.w %d"))
		{
			if (m_srcRect.x + m_srcRect.w > m_texFox.GetWidth())
			{
				m_srcRect.x = m_texFox.GetWidth() - m_srcRect.w;
			}
		}

		if (ImGui::SliderInt("##foxSourceH", &m_srcRect.h, 0, m_texFox.GetHeight(), "m_srcRect.h %d"))
		{
			if (m_srcRect.y + m_srcRect.h > m_texFox.GetHeight())
			{
				m_srcRect.y = m_texFox.GetHeight() - m_srcRect.h;
			}
		}
		float fDestW = static_cast<float>(m_texFox.GetWidth());
		float fDestH = static_cast<float>(m_texFox.GetHeight());
		float fSrcMinX = static_cast<float>(m_srcRect.x);
		float fSrcMinY = static_cast<float>(m_srcRect.y);
		float fSrcMaxX = static_cast<float>(m_srcRect.x + m_srcRect.w);
		float fSrcMaxY = static_cast<float>(m_srcRect.y + m_srcRect.h);
		VertexArray::SRectParam rectParam;
		rectParam.m_minUV.x = fSrcMinX / fDestW;
		rectParam.m_minUV.y = fSrcMinY / fDestH;
		rectParam.m_maxUV.x = fSrcMaxX / fDestW;
		rectParam.m_maxUV.y = fSrcMaxY / fDestH;
		ImGui::NewLine();
		ImGui::Text("minUV{ x: %f,\t y: %f }\nmaxUV{ x: %f,\t y: %f }",
			rectParam.m_minUV.x, rectParam.m_minUV.y,
			rectParam.m_maxUV.x, rectParam.m_maxUV.y);
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
		const char* items[] = { "Flip None", "Flip Horizontal", "Flip Vertical", "Flip Both"};
		static int nCurrFlip = 0;
		if (ImGui::ListBox("Select Flip Format", &nCurrFlip, items, IM_ARRAYSIZE(items), 4))
		{
			switch (nCurrFlip)
			{
			case 0: m_flipFormat = SDL_FLIP_NONE; break;
			case 1: m_flipFormat = SDL_FLIP_HORIZONTAL; break;
			case 2: m_flipFormat = SDL_FLIP_VERTICAL; break;
			case 3: m_flipFormat = SDL_RendererFlip(SDL_FLIP_VERTICAL | SDL_FLIP_HORIZONTAL); break;
			default: break;
			}
		}
	}

	void MainLoop::DrawTextWidget()
	{
		ImGui::Text("Text Dest Rect");
		const auto& screenSize = Renderer2D::Ref().GetScreenSize();
		ImGui::SliderInt("##textDestX", &m_textRect.x, 0, screenSize.w, "m_textRect.x %d");
		ImGui::SliderInt("##textDestY", &m_textRect.y, 0, screenSize.h, "m_textRect.y %d");
		ImGui::SliderInt("##textDestW", &m_textRect.w, 0, screenSize.w, "m_textRect.w %d");
		ImGui::SliderInt("##textDestH", &m_textRect.h, 0, screenSize.h, "m_textRect.h %d");
	}

	void MainLoop::UpdateText(bool bSetRect)
	{
		m_text = m_font.RenderText(m_strText, m_textColor);
		if (m_text)
		{
			if (bSetRect)
			{
				m_textRect.w = m_text->GetWidth();
				m_textRect.h = m_text->GetHeight();
			}
		}
	}
}