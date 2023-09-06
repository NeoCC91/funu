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
	TriMesh::HalfedgeHandle TriMesh::outgoing_hedge(VertexHandle vh) const
	{
		return verts_conn_[vh].outg_heh;
	}

	inline 
	TriMesh::HalfedgeHandle TriMesh::find_hedge(VertexHandle vh0, VertexHandle vh1) const
	{
		HalfedgeHandle heh{INVALID_HANDLE};
		for (auto viter{VCirculator(this, vh0)}; viter.is_valid(); ++viter)
		{
			if (to_vertex(viter.heh()) == vh1)
			{
				heh = viter.heh();
				break;
			}
		}
		return heh;
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
	bool TriMesh::is_isolated(VertexHandle vh) const
	{
		return outgoing_hedge(vh) == INVALID_HANDLE;
	}

	inline 
	bool TriMesh::is_boundary_vertex(VertexHandle vh) const
	{
		return adhereto_face(outgoing_hedge(vh)) == INVALID_HANDLE;
	}

	inline 
	bool TriMesh::is_manifold(VertexHandle vh) const
	{
		int outg_num{};
		for (auto viter{VCirculator(this, vh)}; viter.is_valid(); ++viter)
		{
			if (is_boundary_hedge(viter.heh()))
			{
				++outg_num;
			}
		}
		return outg_num < 2;
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
	TriMesh::HalfedgeHandle TriMesh::inner_hedge(FaceHandle fh) const
	{
		return faces_conn_[fh].inner_heh;
	}

	inline 
	bool TriMesh::add_face(VertexHandle vh0, VertexHandle vh1, VertexHandle vh2)
	{
		//务必是边界点
		if (is_boundary_vertex(vh0) && is_boundary_vertex(vh1) && is_boundary_vertex(vh2))
		{
			//std::array const input_verts{vh0, vh1, vh2, vh0};
			std::array new_hehs{
				find_hedge(vh0, vh1), find_hedge(vh1, vh2), find_hedge(vh2, vh0), INVALID_HANDLE
			};
			new_hehs[3] = new_hehs[0];
			HalfedgeHandle one_side_prev, one_side_next, other_side_prev, other_side_next, boundary_prev, boundary_next;
			for (int i = 0; i < 3; ++i)
			{
				if (new_hehs[i] != INVALID_HANDLE && new_hehs[i + 1] != INVALID_HANDLE &&
					next_hedge(new_hehs[i]) != new_hehs[i + 1])
				{
					auto iter_heh{opp_hedge(new_hehs[i])};
					while (is_boundary_hedge(iter_heh) == false)
					{
						iter_heh = ccw_rotated_hedge(iter_heh);
					}
					//非流型点只能是边界点
					if (iter_heh == new_hehs[i + 1])
					{
						return false;
					}

				}
			}
			return true;
		}
		return false;
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
	TriMesh::HalfedgeHandle TriMesh::next_hedge(HalfedgeHandle heh) const
	{
		return halfedges_conn_[heh].next_heh;
	}
	
	inline
	TriMesh::HalfedgeHandle TriMesh::prev_hedge(HalfedgeHandle heh) const
	{
		return halfedges_conn_[heh].prev_heh;
	}
	
	inline
	TriMesh::FaceHandle TriMesh::adhereto_face(HalfedgeHandle heh) const
	{
		return halfedges_conn_[heh].adt_fh;
	}

	inline
	TriMesh::VertexHandle TriMesh::to_vertex(HalfedgeHandle heh) const
	{
		return halfedges_conn_[heh].to_vh;
	}

	inline
	TriMesh::HalfedgeHandle TriMesh::opp_hedge(HalfedgeHandle heh) const
	{
		return (heh & 1) ? (heh - 1) : (heh + 1);
	}

	inline 
	TriMesh::HalfedgeHandle TriMesh::ccw_rotated_hedge(HalfedgeHandle heh) const
	{
		return opp_hedge(prev_hedge(heh));
	}

	inline 
	TriMesh::HalfedgeHandle TriMesh::cw_rotated_hedge(HalfedgeHandle heh) const
	{
		return next_hedge(opp_hedge(heh));
	}

	inline 
	bool TriMesh::is_boundary_hedge(HalfedgeHandle heh) const
	{
		return adhereto_face(heh) == INVALID_HANDLE;
	}
}
