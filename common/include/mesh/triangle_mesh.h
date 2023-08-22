#pragma once

#include "../core/config.h"
#include "../core/types.h"
#include <vector>

namespace funu
{
	//��������
	class FUNU_API TriMesh
	{
	public:
		TriMesh();
		TriMesh(TriMesh const& src);
		TriMesh(TriMesh&& src) noexcept;
		~TriMesh();

		TriMesh& operator=(TriMesh const& rhs);
		TriMesh& operator=(TriMesh&& rhs) noexcept;

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



	public:

		//���˽ṹ
		std::vector<Vertex> verts_;
		std::vector<Face> faces_;
		std::vector<HalfEdge> halfedges_;

		//��������
		std::vector<Vec4f> points_;

		//�������
	};
}