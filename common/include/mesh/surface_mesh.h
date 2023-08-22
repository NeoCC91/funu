#pragma once

#include "../core/config.h"
#include "../core/types.h"
#include <vector>

namespace funu
{
	//��������
	class FUNU_API surface_mesh
	{
	public:
		surface_mesh();
		surface_mesh(surface_mesh const& src);
		surface_mesh(surface_mesh&& src) noexcept;
		~surface_mesh();

		surface_mesh& operator=(surface_mesh const& rhs);
		surface_mesh& operator=(surface_mesh&& rhs) noexcept;

		//���˽ṹ
		struct HalfEdge
		{
			IndexType next_;
			IndexType prev_;
			IndexType face_;
			IndexType to_;
		};

		struct Vertex
		{
			IndexType out_;
		};

		struct Face
		{
			IndexType hedge_;
		};

	public:



	private:

		//���˽ṹ
		std::vector<Vertex> verts_;
		std::vector<Face> faces_;
		std::vector<HalfEdge> halfedges_;

		//��������
		std::vector<Vec4f> points_;

		//�������
	};
}