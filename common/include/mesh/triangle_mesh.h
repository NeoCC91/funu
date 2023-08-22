#pragma once

#include "../core/config.h"
#include "../core/types.h"
#include <vector>

namespace funu
{
	//三角网格
	class FUNU_API TriMesh
	{
	public:
		TriMesh();
		TriMesh(TriMesh const& src);
		TriMesh(TriMesh&& src) noexcept;
		~TriMesh();

		TriMesh& operator=(TriMesh const& rhs);
		TriMesh& operator=(TriMesh&& rhs) noexcept;

		//拓扑结构
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

		//拓扑结构
		std::vector<Vertex> verts_;
		std::vector<Face> faces_;
		std::vector<HalfEdge> halfedges_;

		//坐标数据
		std::vector<Vec4f> points_;

		//标记数据
	};
}