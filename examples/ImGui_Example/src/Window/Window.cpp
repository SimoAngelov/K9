#include "Window.h"
#include <iostream>
#include <Renderer/Renderer2D.h>

namespace K9
{
	Window::Window() : m_ptrWindow{ nullptr }, m_size{}, m_bgrColor{}
	{
	}

	bool Window::Init(const std::string& strTitle, int nWidth, int nHeight)
	{
		// Create an SDL window
		m_ptrWindow = SDL_CreateWindow(strTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, nWidth, nHeight, SDL_WINDOW_OPENGL);
		if (!m_ptrWindow)
		{
			std::cerr << "Error failed to create window!\n";
			return false;
		}
		m_size = SDL_Rect{ 0, 0, nWidth, nHeight };
		return true;
	}

	void Window::Shutdown()
	{
		// Destroy the window
		SDL_DestroyWindow(m_ptrWindow);
	}

	void Window::Update()
	{
		// Swap OpenGL buffers
		SDL_GL_SwapWindow(m_ptrWindow);
	}

	SDL_Window* Window::GetWindow()
	{
		return m_ptrWindow;
	}

	const SDL_Rect& Window::GetSize() const
	{
		return m_size;
	}

	void Window::SetBackgroundColor(const glm::vec4& bgrColor)
	{
		m_bgrColor = bgrColor;
		glClearColor(m_bgrColor.r, m_bgrColor.g, m_bgrColor.b, m_bgrColor.a);
	}
	void Window::ClearScreen()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}
} // namespace K9