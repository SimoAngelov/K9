#include "Window.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <iostream>

namespace K9
{

	bool Window::Init(const std::string& strTitle, int nWidth, int nHeight)
	{
		// Create an SDL window
		m_ptrWindow = SDL_CreateWindow(strTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, nWidth, nHeight, SDL_WINDOW_OPENGL);
		if (!m_ptrWindow)
		{
			std::cerr << "Error failed to create window!\n";
			return false;
		}
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