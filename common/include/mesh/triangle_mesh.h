#pragma once

#include "../core/config.h"
#include "../core/types.h"
#include <cstdint>
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

		struct TypeTraits
		{
			using HandleType = std::uint32_t;
		};

		static constexpr TypeTraits::HandleType INVALID_HANDLE{std::numeric_limits<TypeTraits::HandleType>::max()};
		using VertexHandle = TypeTraits::HandleType;
		using FaceHandle = TypeTraits::HandleType;
		using HalfedgeHandle = TypeTraits::HandleType;

		//拓扑结构
		struct HalfEdge
		{
			HalfEdge(): next_heh{INVALID_HANDLE}, prev_heh{INVALID_HANDLE}, adt_fh{INVALID_HANDLE}, to_vh{INVALID_HANDLE}
			{
			}
			//上一个半边
			HalfedgeHandle next_heh;
			//下一个半边
			HalfedgeHandle prev_heh;
			//所属面片
			FaceHandle adt_fh;
			//指向的顶点
			VertexHandle to_vh;
		};

		struct Vertex
		{
			Vertex(): outg_heh{INVALID_HANDLE}
			{
			}
			//顶点出发的半边(边界点时，出边为某个边界半边)
			HalfedgeHandle outg_heh;
		};

		struct Face
		{
			Face(): inner_heh{INVALID_HANDLE}
			{
			}
			//面片所含半边
			HalfedgeHandle inner_heh;
		};

	public:
		// API
		//顶点操作
		//点量
		size_t v_n() const;
		//出半边
		HalfedgeHandle outgoing_hedge(VertexHandle vh) const;
		HalfedgeHandle find_hedge(VertexHandle vh0, VertexHandle vh1) const;
		//获取坐标数据
		Vec4& point(VertexHandle vh);
		Vec4 const& point(VertexHandle vh) const;
		//孤立点
		bool is_isolated(VertexHandle vh) const;
		//边界点
		bool is_boundary_vertex(VertexHandle vh) const;
		//非流型点
		bool is_manifold(VertexHandle vh) const;
		//加点
		bool add_vertex(Vec4 const& pnt);
		//删点
		bool remove_vertex(VertexHandle vh);

		//面片操作
		//面量
		size_t f_n() const;
		//包含的半边
		HalfedgeHandle inner_hedge(FaceHandle fh) const;

		//添加面片
		bool add_face(VertexHandle vh0, VertexHandle vh1, VertexHandle vh2);
		//删除面片
		bool remove_face(FaceHandle fh);

		//半边操作
		//半边量
		size_t he_n() const;
		//边量
		size_t e_n() const;
		//下一条半边
		HalfedgeHandle next_hedge(HalfedgeHandle heh) const;
		//上一条半边
		HalfedgeHandle prev_hedge(HalfedgeHandle heh) const;
		//所属面片
		FaceHandle adhereto_face(HalfedgeHandle heh) const;
		//指向顶点
		VertexHandle to_vertex(HalfedgeHandle heh) const;
		//反向半边
		HalfedgeHandle opp_hedge(HalfedgeHandle heh) const;
		HalfedgeHandle ccw_rotated_hedge(HalfedgeHandle heh) const;
		HalfedgeHandle cw_rotated_hedge(HalfedgeHandle heh) const;
		//边界半边
		bool is_boundary_hedge(HalfedgeHandle heh) const;

	private:
		//内部api

	private:
		//拓扑结构
		std::vector<Vertex> verts_conn_;
		std::vector<Face> faces_conn_;
		std::vector<HalfEdge> halfedges_conn_;

		//位置数据 x,y,z,w
		std::vector<Vec4> points_;

		//标记数据
		std::vector<bool> verts_deleted_;
		std::vector<bool> faces_deleted_;
		std::vector<bool> halfedges_deleted_;
	};

	//顶点拓扑关系访问器
	class VCirculator
	{
	public:
		VCirculator(TriMesh const* mesh, TriMesh::VertexHandle vh): mesh_{mesh},
		                                                               start_heh_{mesh_->outgoing_hedge(vh)},
		                                                               curr_heh_{start_heh_}, rotate_count_{}
		{
		}

		//ccw
		VCirculator& operator++()
		{
			curr_heh_ = mesh_->ccw_rotated_hedge(curr_heh_);
			++rotate_count_;
			return *this;
		}

		//cw
		VCirculator& operator--()
		{
			curr_heh_ = mesh_->cw_rotated_hedge(curr_heh_);
			--rotate_count_;
			return *this;
		}

		bool is_valid() const
		{
			return curr_heh_ != start_heh_ || !rotate_count_; 
		}

		TriMesh::HalfedgeHandle heh() const
		{
			return curr_heh_;
		}

		TriMesh::FaceHandle fh() const
		{
			return mesh_->adhereto_face(curr_heh_);
		}

	private:
		TriMesh const* mesh_;
		TriMesh::HalfedgeHandle start_heh_;
		TriMesh::HalfedgeHandle curr_heh_;
		int rotate_count_;
	};
}
