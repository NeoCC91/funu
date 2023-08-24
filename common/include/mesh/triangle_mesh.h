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
			HalfEdge(): next{INVALID_IDX}, prev{INVALID_IDX}, face{INVALID_IDX}, to{INVALID_IDX}
			{
			}
			//上一个半边
			IndexType next;
			//下一个半边
			IndexType prev;
			//所属面片
			IndexType face;
			//指向的顶点
			IndexType to;
		};

		struct Vertex
		{
			Vertex(): out{INVALID_IDX}
			{
			}
			//顶点出发的任意一个半边
			IndexType out;
		};

		struct Face
		{
			Face(): hedge{INVALID_IDX}
			{
			}
			//面片所含半边
			IndexType hedge;
		};

	public:
		// API
		//顶点操作
		//点量
		size_t v_n() const;
		bool add_vertex(Vec4 const& pnt);
		bool remove_vertex(IndexType vert_idx);

		//面片操作
		//面量
		size_t f_n() const;


		bool add_face(IndexType vert_idx0, IndexType vert_idx1, IndexType vert_idx2);


		bool remove_face(IndexType face_idx);

		//半边操作
		//半边量
		size_t he_n() const;
		//边量
		size_t e_n() const;

	private:
		//内部api

	private:
		//拓扑结构
		std::vector<Vertex> verts_;
		std::vector<Face> faces_;
		std::vector<HalfEdge> halfedges_;

		//位置数据
		std::vector<Vec4> points_;

		//标记数据
		std::vector<bool> verts_deleted_;
		std::vector<bool> faces_deleted_;
		std::vector<bool> halfedges_deleted_;
	};
}
