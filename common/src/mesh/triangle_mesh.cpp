#include "mesh/triangle_mesh.h"

#include <mesh/triangle_mesh.h>
#include <array>

namespace funu
{
	TriMesh::TriMesh() = default;

	TriMesh::TriMesh(TriMesh const& src)
	{
		verts_ = src.verts_;
		faces_ = src.faces_;
		halfedges_ = src.halfedges_;
		points_ = src.points_;
	}

	TriMesh::TriMesh(TriMesh&& src) noexcept
	{
		verts_.swap(src.verts_);
		faces_.swap(src.faces_);
		halfedges_.swap(src.halfedges_);
		points_.swap(src.points_);
	}

	TriMesh::~TriMesh() = default;

	TriMesh& TriMesh::operator=(TriMesh const& rhs)
	{
		if (this != &rhs)
		{
			verts_ = rhs.verts_;
			faces_ = rhs.faces_;
			halfedges_ = rhs.halfedges_;
			points_ = rhs.points_;
		}
		return *this;
	}

	TriMesh& TriMesh::operator=(TriMesh&& rhs) noexcept
	{
		verts_.swap(rhs.verts_);
		faces_.swap(rhs.faces_);
		halfedges_.swap(rhs.halfedges_);
		points_.swap(rhs.points_);
		return *this;
	}

	auto TriMesh::v_n() const
	{
		return verts_.size();
	}

	inline 
	bool TriMesh::add_vertex(Vec4 const& pnt)
	{
		verts_.emplace_back();
		points_.push_back(pnt);
		return true;
	}

	inline 
	bool TriMesh::remove_vertex(IndexType vert_idx)
	{
		verts_deleted_[vert_idx] = true;
		return true;
	}

	auto TriMesh::f_n() const
	{
		return faces_.size();
	}


	inline 
	bool TriMesh::add_face(IndexType vert_idx0, IndexType vert_idx1, IndexType vert_idx2)
	{
		std::array input_verts{vert_idx0,vert_idx1,vert_idx2,vert_idx0};
		return true;
	}

	inline 
	bool TriMesh::remove_face(IndexType face_idx)
	{
		faces_deleted_[face_idx] = true;
		return true;
	}

	auto TriMesh::he_n() const
	{
		return halfedges_.size();
	}

	auto TriMesh::e_n() const
	{
		return halfedges_.size() >> 1;
	}
}
