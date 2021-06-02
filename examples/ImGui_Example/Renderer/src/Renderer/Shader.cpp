#include "Shader.h"
#include "Texture.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

namespace K9
{
	Shader::Shader()
		: m_unShaderProgramID(0),
		m_unVertexShaderID(0),
		m_unFragShaderID(0)
	{

	}

	bool Shader::Load(const std::string& strVertexName, const std::string& strFragmentName)
	{
		/* Compile vertex and pixel shaders */
		if (!CompileShader(strVertexName, GL_VERTEX_SHADER, m_unVertexShaderID) ||
			!CompileShader(strFragmentName, GL_FRAGMENT_SHADER, m_unFragShaderID))
		{
			return false;
		}

		/* Now create a shader program that
		 * links together the vertex/frag shaders */
		m_unShaderProgramID = glCreateProgram();
		glAttachShader(m_unShaderProgramID, m_unVertexShaderID);
		glAttachShader(m_unShaderProgramID, m_unFragShaderID);
		glLinkProgram(m_unShaderProgramID);

		/* Verify that the program linked successfully */
		if (!IsValidProgram())
		{
			return false;
		}

		return true;
	}

	void Shader::Unload()
	{
		/* Delete the program/shaders */
		glDeleteProgram(m_unShaderProgramID);
		glDeleteShader(m_unVertexShaderID);
		glDeleteShader(m_unFragShaderID);
	}

	void Shader::SetActive()
	{
		/* Set this program as the active one */
		glUseProgram(m_unShaderProgramID);
	}

	void Shader::SetMatrixUniform(const char* cstrName, const glm::mat4& mat4Value)
	{
		/* Find the uniform by this name */
		GLuint loc = glGetUniformLocation(m_unShaderProgramID, cstrName);
		/* Send the matrix data to the uniform */
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat4Value));
	}

	void Shader::SetMatrixUniforms(const char* cstrName, glm::mat4* arrMatrices, unsigned int unMatrixCount)
	{
		/* Find the uniform by this name */
		GLuint loc = glGetUniformLocation(m_unShaderProgramID, cstrName);
		glUniformMatrix4fv(loc, unMatrixCount, GL_FALSE, glm::value_ptr(arrMatrices[0]));
	}

	void Shader::SetVectorUniform(const char* cstrName, const glm::vec3& vec3DValue)
	{
		GLuint loc = glGetUniformLocation(m_unShaderProgramID, cstrName);
		/* Send the vector data */
		glUniform3fv(loc, 1, glm::value_ptr(vec3DValue));
	}

	void Shader::SetVector2DUniform(const char* cstrName, const glm::vec2& vec2DValue)
	{
		GLuint loc = glGetUniformLocation(m_unShaderProgramID, cstrName);
		/* Send the vector data */
		glUniform2fv(loc, 1, glm::value_ptr(vec2DValue));
	}

	void Shader::SetFloatUniform(const char* cstrName, float fValue)
	{
		GLuint loc = glGetUniformLocation(m_unShaderProgramID, cstrName);
		/* Send the float data */
		glUniform1f(loc, fValue);
	}

	void Shader::SetIntUniform(const char* cstrName, int nValue)
	{
		GLuint loc = glGetUniformLocation(m_unShaderProgramID, cstrName);
		/* Send the integer data */
		glUniform1i(loc, nValue);
	}

	bool Shader::CompileShader(const std::string& strFileName, GLenum eShaderType, GLuint& nOutShader)
	{
		/* Open file */
		std::ifstream shaderFile(strFileName);
		if (shaderFile.is_open())
		{
			/* Read all the text into a string */
			std::stringstream strStream;
			strStream << shaderFile.rdbuf();
			std::string strContents = strStream.str();
			const char* cstrContents = strContents.c_str();

			/* Create a shader of the specified type */
			nOutShader = glCreateShader(eShaderType);
			/* Set the source characters and try to compile */
			glShaderSource(nOutShader, 1, &(cstrContents), nullptr);
			glCompileShader(nOutShader);

			if (!IsCompiled(nOutShader))
			{
				std::cerr << "Failed to compile shader " << strFileName;
				return false;
			}
		}
		else
		{
			std::cerr << "Shader file not found: " << strFileName;
			return false;
		}

		return true;
	}

	bool Shader::IsCompiled(GLuint unShaderID)
	{
		GLint unStatus;
		/* Query the compile status */
		glGetShaderiv(unShaderID, GL_COMPILE_STATUS, &unStatus);

		if (unStatus != GL_TRUE)
		{
			constexpr unsigned int unMaxBuffSize = 512;
			char msgBuff[unMaxBuffSize];
			memset(msgBuff, 0, unMaxBuffSize);
			glGetShaderInfoLog(unShaderID, unMaxBuffSize - 1, nullptr, msgBuff);
			std::cerr << "GLSL Compile Failed:\n" << msgBuff << "\n";
			return false;
		}

		return true;
	}

	bool Shader::IsValidProgram()
	{
		GLint unStatus;
		/* Query the link status */
		glGetProgramiv(m_unShaderProgramID, GL_LINK_STATUS, &unStatus);
		if (unStatus != GL_TRUE)
		{
			constexpr unsigned int unMaxBuffSize = 512;
			char msgBuff[unMaxBuffSize];
			memset(msgBuff, 0, unMaxBuffSize);
			glGetProgramInfoLog(m_unShaderProgramID, unMaxBuffSize - 1, nullptr, msgBuff);
			std::cerr << "GLSL Link Status Failed:\n" << msgBuff << "\n";
			return false;
		}

		return true;
	}
}