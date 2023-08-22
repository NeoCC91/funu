#include <mesh/surface_mesh.h>

namespace funu
{
	surface_mesh::surface_mesh() = default;

	surface_mesh::surface_mesh(surface_mesh const& src)
	{
		verts_ = src.verts_;
		faces_ = src.faces_;
		halfedges_ = src.halfedges_;
		points_ = src.points_;
	}

	surface_mesh::surface_mesh(surface_mesh&& src) noexcept
	{
		verts_.swap(src.verts_);
		faces_.swap(src.faces_);
		halfedges_.swap(src.halfedges_);
		points_.swap(src.points_);
	}

	surface_mesh::~surface_mesh() = default;

	surface_mesh& surface_mesh::operator=(surface_mesh const& rhs)
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

	surface_mesh& surface_mesh::operator=(surface_mesh&& rhs) noexcept
	{
		verts_.swap(rhs.verts_);
		faces_.swap(rhs.faces_);
		halfedges_.swap(rhs.halfedges_);
		points_.swap(rhs.points_);
		return *this;
	}
}
