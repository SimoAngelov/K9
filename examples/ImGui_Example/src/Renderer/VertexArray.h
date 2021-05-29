#pragma once
#include <cstdint>

namespace K9
{
	class VertexArray
	{
	public:
		/* Different supported vertex layouts */
		enum class ELayout
		{
			ePosTex,
		};

		VertexArray(const void* arrVertices, unsigned int unVertexCount, ELayout eLayout,
			const unsigned int* arrIndices, unsigned int unIndexCount);
		~VertexArray();

		void SetActive();
		unsigned int GetIndexCount() const { return m_unIndexCount; }
		unsigned int GetVertexCount() const { return m_unVertexCount; }

		static unsigned int GetVertexSize(VertexArray::ELayout eLayout);
	private:
		/* How many vertices in the vertex buffer */
		unsigned int m_unVertexCount;
		/* How many indices in the index buffer */
		unsigned int m_unIndexCount;
		/* OpenGL ID of the vertex buffer */
		unsigned int m_unVertexBufferID;
		/* OpenGL ID of the index buffer */
		unsigned int m_unIndexBufferID;
		/* OpenGL ID of the vertex array object */
		unsigned int m_unVertexArrayID;
	};
}