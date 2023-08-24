#include "mesh/triangle_mesh.h"
#include <array>

namespace funu
{
	TriMesh::TriMesh() = default;

	TriMesh::TriMesh(TriMesh const& src)
	{
		verts_conn_ = src.verts_conn_;
		faces_conn_ = src.faces_conn_;
		halfedges_conn_ = src.halfedges_conn_;
		points_ = src.points_;
	}

	TriMesh::TriMesh(TriMesh&& src) noexcept
	{
		verts_conn_.swap(src.verts_conn_);
		faces_conn_.swap(src.faces_conn_);
		halfedges_conn_.swap(src.halfedges_conn_);
		points_.swap(src.points_);
	}

	TriMesh::~TriMesh() = default;

	TriMesh& TriMesh::operator=(TriMesh const& rhs)
	{
		if (this != &rhs)
		{
			verts_conn_ = rhs.verts_conn_;
			faces_conn_ = rhs.faces_conn_;
			halfedges_conn_ = rhs.halfedges_conn_;
			points_ = rhs.points_;
		}
		return *this;
	}

	TriMesh& TriMesh::operator=(TriMesh&& rhs) noexcept
	{
		verts_conn_.swap(rhs.verts_conn_);
		faces_conn_.swap(rhs.faces_conn_);
		halfedges_conn_.swap(rhs.halfedges_conn_);
		points_.swap(rhs.points_);
		return *this;
	}

	inline 
	size_t TriMesh::v_n() const
	{
		return verts_conn_.size();
	}

	inline 
	TriMesh::HalfedgeHandle& TriMesh::outgoing_hedge(VertexHandle vh)
	{
		return verts_conn_[vh].outg_heh;
	}

	inline 
	TriMesh::HalfedgeHandle const& TriMesh::outgoing_hedge(VertexHandle vh) const
	{
		return verts_conn_[vh].outg_heh;
	}

	inline 
	Vec4& TriMesh::point(VertexHandle vh)
	{
		return points_[vh];
	}

	inline 
	Vec4 const& TriMesh::point(VertexHandle vh) const
	{
		return points_[vh];
	}

	inline 
	bool TriMesh::add_vertex(Vec4 const& pnt)
	{
		verts_conn_.emplace_back();
		points_.push_back(pnt);
		verts_deleted_.push_back({});
		return true;
	}

	inline 
	bool TriMesh::remove_vertex(VertexHandle vh)
	{
		verts_deleted_[vh] = true;
		return true;
	}

	inline 
	size_t TriMesh::f_n() const
	{
		return faces_conn_.size();
	}

	inline 
	TriMesh::HalfedgeHandle& TriMesh::inner_hedge(FaceHandle fh)
	{
		return faces_conn_[fh].inner_heh;
	}

	inline 
	TriMesh::HalfedgeHandle const& TriMesh::inner_hedge(FaceHandle fh) const
	{
		return faces_conn_[fh].inner_heh;
	}

	inline 
	bool TriMesh::add_face(VertexHandle vh0, VertexHandle vh1, VertexHandle vh2)
	{
		std::array input_verts{vh0,vh1,vh2,vh0};
		return true;
	}

	inline 
	bool TriMesh::remove_face(FaceHandle fh)
	{
		faces_deleted_[fh] = true;
		return true;
	}

	inline 
	size_t TriMesh::he_n() const
	{
		return halfedges_conn_.size();
	}

	inline 
	size_t TriMesh::e_n() const
	{
		return halfedges_conn_.size() >> 1;
	}

	inline 
	TriMesh::HalfedgeHandle& TriMesh::next_hedge(HalfedgeHandle heh)
	{
		return halfedges_conn_[heh].next_heh;
	}

	inline 
	TriMesh::HalfedgeHandle const& TriMesh::next_hedge(HalfedgeHandle heh) const
	{
		return halfedges_conn_[heh].next_heh;
	}

	inline 
	TriMesh::HalfedgeHandle& TriMesh::prev_hedge(HalfedgeHandle heh)
	{
		return halfedges_conn_[heh].prev_heh;
	}
	
	inline 
	TriMesh::HalfedgeHandle const& TriMesh::prev_hedge(HalfedgeHandle heh) const
	{
		return halfedges_conn_[heh].prev_heh;
	}

	inline
	TriMesh::HalfedgeHandle TriMesh::opp_hedge(HalfedgeHandle heh) const
	{
		return (heh & 1) ? (heh - 1) : (heh + 1);
	}
	
	inline
	TriMesh::FaceHandle& TriMesh::adhereto_face(HalfedgeHandle heh)
	{
		return halfedges_conn_[heh].adt_fh;
	}
	
	inline
	TriMesh::FaceHandle const& TriMesh::adhereto_face(HalfedgeHandle heh) const
	{
		return halfedges_conn_[heh].adt_fh;
	}
}
