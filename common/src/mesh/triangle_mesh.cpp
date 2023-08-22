#include <mesh/triangle_mesh.h>

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
}
