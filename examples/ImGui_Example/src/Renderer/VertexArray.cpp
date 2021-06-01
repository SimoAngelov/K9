#include "VertexArray.h"
#include <glad/glad.h>
#include <iostream>

namespace K9
{
	VertexArray::VertexArray(const void* arrVertices, unsigned int unVertexCount,
		ELayout eLayout, const unsigned int* arrIndices, unsigned int unIndexCount)
		: m_unVertexCount(unVertexCount),
		m_unIndexCount(unIndexCount)
	{
		SetVertexArray(arrVertices, unVertexCount, eLayout, arrIndices, unIndexCount);
	}

	VertexArray::VertexArray(const SPointParam& pointParam)
	{
		SetVertexArray(pointParam);
	}

	VertexArray::VertexArray(const SRectParam& rectParam)
	{
		SetVertexArray(rectParam);
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

	void VertexArray::SetVertexArray(const void* arrVertices, unsigned int unVertexCount, ELayout eLayout, const unsigned int* arrIndices, unsigned int unIndexCount)
	{
		/* Create vertex array */
		m_unVertexCount = unVertexCount;
		m_unIndexCount = unIndexCount;

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

	void VertexArray::SetVertexArray(const SPointParam& pointParam)
	{
		const auto& topLeftPos = pointParam.GetPos(EPos::eTopLeft);
		const auto& topRightPos = pointParam.GetPos(EPos::eTopRight);
		const auto& bottomRightPos = pointParam.GetPos(EPos::eBottomRight);
		const auto& bottomLeftPos = pointParam.GetPos(EPos::eBottomLeft);

		const auto& topLeftUV = pointParam.GetTexCoord(EPos::eTopLeft);
		const auto& topRightUV = pointParam.GetTexCoord(EPos::eTopRight);
		const auto& bottomRightUV = pointParam.GetTexCoord(EPos::eBottomRight);
		const auto& bottomLeftUV = pointParam.GetTexCoord(EPos::eBottomLeft);

		float arrVertices[] =
		{
			topLeftPos.x, topLeftPos.y, topLeftPos.z, topLeftUV.x, topLeftUV.y,
			topRightPos.x, topRightPos.y, topRightPos.z, topRightUV.x, topRightUV.y,
			bottomRightPos.x, bottomRightPos.y, bottomRightPos.z, bottomRightUV.x, bottomRightUV.y,
			bottomLeftPos.x, bottomLeftPos.y, bottomLeftPos.z, bottomLeftUV.x, bottomLeftUV.y
		};

		unsigned int arrIndices[] = {
			0, 1, 2,
			2, 3, 0
		};

		SetVertexArray(arrVertices, SPointParam::VERTEX_COUNT, pointParam.m_eLayout,
			arrIndices, SPointParam::INDEX_COUNT);

	}

	void VertexArray::SetVertexArray(const SRectParam& rectParam)
	{
		SPointParam pointParam{};
		pointParam.m_eLayout = rectParam.m_eLayout;

		const auto& rect = rectParam.m_normalizedSize;
		const auto& minUV = rectParam.m_minUV;
		const auto& maxUV = rectParam.m_maxUV;

		pointParam.SetPos(EPos::eTopLeft, glm::vec3{ rect.x, rect.y, 0.0f });
		pointParam.SetPos(EPos::eTopRight, glm::vec3{ rect.z, rect.y, 0.0f });
		pointParam.SetPos(EPos::eBottomRight, glm::vec3{ rect.z, rect.w, 0.0f });
		pointParam.SetPos(EPos::eBottomLeft, glm::vec3{ rect.x, rect.w, 0.0f });

		pointParam.SetTexCoord(EPos::eTopLeft, glm::vec2{ minUV.x, minUV.y });
		pointParam.SetTexCoord(EPos::eTopRight, glm::vec2{ maxUV.x, minUV.y });
		pointParam.SetTexCoord(EPos::eBottomRight, glm::vec2{ maxUV.x, maxUV.y });
		pointParam.SetTexCoord(EPos::eBottomLeft, glm::vec2{ minUV.x, maxUV.y });
		SetVertexArray(pointParam);
	}

	VertexArray::SPointParam::SPointParam(ELayout eLayout, const std::array<glm::vec3, VERTEX_COUNT>& arrPos,
										const std::array<glm::vec2, VERTEX_COUNT> arrTexCoords)
		: m_eLayout{eLayout}, m_arrPos{arrPos}, m_arrTexCoords{arrTexCoords}
	{
	}

	const glm::vec3& VertexArray::SPointParam::GetPos(EPos ePos) const
	{
		uint64_t unIndex = static_cast<uint64_t>(ePos);
		return m_arrPos.at(unIndex);
	}

	void VertexArray::SPointParam::SetPos(EPos ePos, const glm::vec3 pos)
	{
		uint64_t unIndex = static_cast<uint64_t>(ePos);
		m_arrPos.at(unIndex) = pos;
	}

	const glm::vec2& VertexArray::SPointParam::GetTexCoord(EPos ePos) const
	{
		uint64_t unIndex = static_cast<uint64_t>(ePos);
		return m_arrTexCoords.at(unIndex);
	}

	void VertexArray::SPointParam::SetTexCoord(EPos ePos, const glm::vec2& texCoord)
	{
		uint64_t unIndex = static_cast<uint64_t>(ePos);
		m_arrTexCoords.at(unIndex) = texCoord;
	}

	VertexArray::SRectParam::SRectParam(ELayout eLayout,
		const glm::vec4& normalizedSize,
		const glm::vec2& minUV, const glm::vec2& maxUV)
		: m_eLayout{ eLayout }, m_normalizedSize{ normalizedSize }, m_minUV{ minUV }, m_maxUV{ maxUV }
	{
	}

	bool VertexArray::SRectParam::CheckUV()
	{
		if (m_minUV.x < 0.0f)
		{
			std::cerr << " m_minUV.x(" << m_minUV.x << ") is invalid!\n";
			return false;
		}
		
		if (m_minUV.y > 1.0f)
		{
			std::cerr << " m_minUV.y(" << m_minUV.y << ") is invalid\n";
			return false;
		}

		if (m_maxUV.x < 0.0f)
		{
			std::cerr << " m_maxUV.x(" << m_maxUV.x << ") is invalid!\n";
			return false;
		}

		if (m_maxUV.y > 1.0f)
		{
			std::cerr << " m_maxUV.y(" << m_minUV.y << ") is invalid\n";
			return false;
		}

		return true;
	}
}