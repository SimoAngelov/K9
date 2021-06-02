#pragma once
#include <cstdint>
#include <array>
#include <glm/glm.hpp>

namespace K9
{
	/// <summary>
	/// Holds shape geometry.
	/// </summary>
	class VertexArray
	{
	public:
		/* Different supported vertex layouts. */
		enum class ELayout
		{
			ePosTex,
		};

		/* Enumerate position types. */
		enum class EPos
		{
			eTopLeft = 0,
			eTopRight,
			eBottomRight,
			eBottomLeft
		};

		/// <summary>
		/// Define the geometry via points.
		/// </summary>
		struct SPointParam
		{
			/// <summary>
			/// Number of vertices.
			/// </summary>
			static constexpr uint64_t VERTEX_COUNT{ 4 };

			/// <summary>
			/// Number of indices.
			/// </summary>
			static constexpr uint64_t INDEX_COUNT{ 6 };

			/// <summary>
			/// Layout type.
			/// </summary>
			ELayout m_eLayout;

			/// <summary>
			/// Array, defining the position in space.
			/// </summary>
			std::array<glm::vec3, VERTEX_COUNT> m_arrPos;

			/// <summary>
			/// Array, defining the texture coordinates.
			/// </summary>
			std::array<glm::vec2, VERTEX_COUNT> m_arrTexCoords;

			/// <summary>
			/// Default constructor.
			/// </summary>
			/// <param name="eLayout"> Vertex layout. </param>
			/// <param name="arrPos"> Position data. </param>
			/// <param name="arrTexCoords"> Texture coordinates data. </param>
			SPointParam(ELayout eLayout = ELayout::ePosTex,
				const std::array<glm::vec3, VERTEX_COUNT>& arrPos =
				{
					glm::vec3{-0.5f, 0.5f, 0.0f},	/* Top-Left */
					glm::vec3{ 0.5f, 0.5f, 0.0f},	/* Top-Right */
					glm::vec3{ 0.5f,-0.5f, 0.0f},	/* Bottom-Right */
					glm::vec3{-0.5f,-0.5f, 0.0f}		/* Bottom-Left */
				},
				const std::array<glm::vec2, VERTEX_COUNT> arrTexCoords =
				{
					glm::vec2{0.0f, 1.0f},	/* Top-Left */
					glm::vec2{1.0f, 1.0f },	/* Top-Right */
					glm::vec2{1.0f, 0.0f},	/* Bottom-Right */
					glm::vec2{0.0f, 0.0f}	/* Bottom-Left */

				});

			/// <summary>
			/// Retrieve the specified position.
			/// </summary>
			/// <param name="ePos"> Index of the position to be retrieved. </param>
			/// <returns> The specified position. </returns>
			const glm::vec3& GetPos(EPos ePos) const;

			/// <summary>
			/// Set the specified position.
			/// </summary>
			/// <param name="ePos"> Index of the position to be set. </param>
			/// <param name="pos"> Position to be set. </param>
			void SetPos(EPos ePos, const glm::vec3 pos);

			/// <summary>
			/// Retrieve the specified texture coordinate.
			/// </summary>
			/// <param name="ePos"> Index of the texture coordinate to be retrieved. </param>
			/// <returns> The specified texture coordinate. </returns>
			const glm::vec2& GetTexCoord(EPos ePos) const;

			/// <summary>
			/// Set the specified texture coordinate.
			/// </summary>
			/// <param name="ePos"> Index of the texture coordinate to be set. </param>
			/// <param name="texCoord"> Texture coordinate to be set. </param>
			void SetTexCoord(EPos ePos, const glm::vec2& texCoord);
		};

		/// <summary>
		/// Define the geomtry via a rectangle.
		/// </summary>
		struct SRectParam
		{
			/// <summary>
			/// Layout type.
			/// </summary>
			ELayout m_eLayout;

			/// <summary>
			/// A normalized rect in the range [-1.0, 1.0].
			/// x - top-left.x
			/// y - top-left.y
			/// z - bottom-right.x
			/// w - bottom-right.y
			/// </summary>
			glm::vec4 m_normalizedSize;

			/// <summary>
			/// The minimum value of the UV texture coordinates in the range [0.0, 1.0].
			/// </summary>
			glm::vec2 m_minUV;

			/// <summary>
			/// The maximum value of the UV texture coordinates in the range [0.0, 1.0].
			/// </summary>
			glm::vec2 m_maxUV;

			/// <summary>
			/// Default constructor.
			/// </summary>
			/// <param name="eLayout"> Vertex layout. </param>
			/// <param name="normalizedSize"> Normalized vertex positions. </param>
			/// <param name="minUV"> Normalized min UV texture coordinate. </param>
			/// <param name="maxUV"> Normalized max UV texture coordinate. </param>
			SRectParam(ELayout eLayout = ELayout::ePosTex,
				const glm::vec4& normalizedSize = { -0.5f, 0.5f, 0.5f, -0.5f },
				const glm::vec2& minUV = { 0.0f, 0.0f },
				const glm::vec2& maxUV = { 1.0f, 1.0f });

			/// <summary>
			/// Check if the UV texture coordinates are valid.
			/// </summary>
			/// <returns> True, if m_minUV and m_maxUV are in the range [0.0, 1.0]. </returns>
			bool CheckUV();
		};

		VertexArray(const void* arrVertices, unsigned int unVertexCount, ELayout eLayout,
			const unsigned int* arrIndices, unsigned int unIndexCount);
		VertexArray(const SPointParam& pointParam);
		VertexArray(const SRectParam& rectParam);
		~VertexArray();

		/// <summary>
		/// Set the current vertex array as the active one.
		/// </summary>
		void SetActive();

		/// <summary>
		/// Retrieve the index count.
		/// </summary>
		/// <returns> m_unIndexCount. </returns>
		unsigned int GetIndexCount() const { return m_unIndexCount; }

		/// <summary>
		/// Retrieve the vertex count.
		/// </summary>
		/// <returns> m_unVertexCount. </returns>
		unsigned int GetVertexCount() const { return m_unVertexCount; }

		/// <summary>
		/// Retrieve the size of a single vertex.
		/// </summary>
		/// <param name="eLayout"> Layout of the vertex. </param>
		/// <returns> The size of the vertex, based on its layout. </returns>
		static unsigned int GetVertexSize(VertexArray::ELayout eLayout);

	private:
		/// <summary>
		/// Set the vertex array data.
		/// </summary>
		/// <param name="arrVertices"> Vertex data.</param>
		/// <param name="unVertexCount"> Vertex count. </param>
		/// <param name="eLayout"> Layout of the vertices. </param>
		/// <param name="arrIndices"> Index data. </param>
		/// <param name="unIndexCount"> Index count. </param>
		void SetVertexArray(const void* arrVertices, unsigned int unVertexCount, ELayout eLayout,
			const unsigned int* arrIndices, unsigned int unIndexCount);

		/// <summary>
		/// Set the vertex array data.
		/// </summary>
		/// <param name="pointParam"> Holds vertex array data. </param>
		void SetVertexArray(const SPointParam& pointParam);

		/// <summary>
		/// Set the vertex array data.
		/// </summary>
		/// <param name="rectParam"> Holds vertex array data. </param>
		void SetVertexArray(const SRectParam& rectParam);

	private:
		/// <summary>
		///  How many vertices in the vertex buffer.
		/// </summary>
		unsigned int m_unVertexCount = 0;
		
		/// <summary>
		/// How many indices in the index buffer.
		/// </summary>
		unsigned int m_unIndexCount = 0;
	
		/// <summary>
		///  OpenGL ID of the vertex buffer.
		/// </summary>
		unsigned int m_unVertexBufferID = 0;
	
		/// <summary>
		/// OpenGL ID of the index buffer.
		/// </summary>
		unsigned int m_unIndexBufferID = 0;
	
		/// <summary>
		/// OpenGL ID of the vertex array object.
		/// </summary>
		unsigned int m_unVertexArrayID = 0;
	};
}