#include "VertexArray.h"
#include <glad/glad.h>

namespace K9
{
	VertexArray::VertexArray(const void* arrVertices, unsigned int unVertexCount,
		ELayout eLayout, const unsigned int* arrIndices, unsigned int unIndexCount)
		: m_unVertexCount(unVertexCount),
		m_unIndexCount(unIndexCount)
	{
		/* Create vertex array */
		glGenVertexArrays(1, &m_unVertexArrayID);
		glBindVertexArray(m_unVertexArrayID);
		constexpr unsigned int unVertexElementCount = 8;
		unsigned int unVertexSize = GetVertexSize(eLayout);

		/* Create vertex buffer */
		glGenBuffers(1, &m_unVertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, m_unVertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, static_cast<uint64_t>(unVertexCount) * unVertexSize, arrVertices, GL_STATIC_DRAW);

		/* Create index buffer */
		glGenBuffers(1, &m_unIndexBufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unIndexBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, unIndexCount * sizeof(unsigned int), arrIndices, GL_STATIC_DRAW);

		/* Specify the vertex attributes */
		if (eLayout == ELayout::ePosTex)
		{
			/* Position is 3 floats */
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, unVertexSize, 0);
			/* Texture coordinates is 2 floats */
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, unVertexSize,
				reinterpret_cast<void*>(sizeof(float) * 3));
		}
	}

	VertexArray::~VertexArray()
	{
		glDeleteBuffers(1, &m_unVertexBufferID);
		glDeleteBuffers(1, &m_unIndexBufferID);
		glDeleteVertexArrays(1, &m_unVertexArrayID);
	}

	void VertexArray::SetActive()
	{
		glBindVertexArray(m_unVertexArrayID);
	}

	unsigned int VertexArray::GetVertexSize(VertexArray::ELayout eLayout)
	{
		constexpr unsigned int unVertexElementCount = 5;
		unsigned int unVertexSize = (unVertexElementCount * sizeof(float));
		return unVertexSize;
	}
}