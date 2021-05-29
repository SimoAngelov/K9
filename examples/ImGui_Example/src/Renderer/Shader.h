#pragma once
#include <glm/glm.hpp>
#include <string>

using GLenum = unsigned int;
using GLuint = unsigned int;

namespace K9
{
	class Shader
	{
	public:
		Shader();
		~Shader() = default;
		/* Load the vertex/fragment shaders with the given names */
		bool Load(const std::string& strVertexName, const std::string& strFragName);
		void Unload();
		/* Set this as the active shader program */
		void SetActive();
		/* Sets a Matrix uniform */
		void SetMatrixUniform(const char* cstrName, const glm::mat4& mat4Value);
		/* Set an array of matrix uniforms */
		void SetMatrixUniforms(const char* cstrName, glm::mat4* arrMatrices, unsigned int unMatrixCount);
		/* Sets a Vector3D uniform */
		void SetVectorUniform(const char* cstrName, const glm::vec3& vec3DValue);
		/* Sets a Vector2D uniform */
		void SetVector2DUniform(const char* cstrName, const glm::vec2& vec2DValue);
		/* Sets a float uniform */
		void SetFloatUniform(const char* cstrName, float fValue);
		/* Sets an integer uniform */
		void SetIntUniform(const char* cstrName, int nValue);
	private:
		/* Tries to compile the specified shader */
		bool CompileShader(const std::string& strFileName, GLenum eShaderType, GLuint& nOutShader);

		/* Tests whether shader compiled successfully */
		bool IsCompiled(GLuint unShaderID);
		/* Tests whether vertex/fragment programs link */
		bool IsValidProgram();
	private:
		/* Store the shader object IDs */
		GLuint m_unVertexShaderID;
		GLuint m_unFragShaderID;
		GLuint m_unShaderProgramID;
	};
}