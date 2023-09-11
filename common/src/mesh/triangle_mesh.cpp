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
	TriMesh::HalfedgeHandle TriMesh::outgoing_heh(VertexHandle vh) const
	{
		return verts_conn_[vh].outg_heh_;
	}

	inline 
	TriMesh::HalfedgeHandle TriMesh::find_heh(VertexHandle vh0, VertexHandle vh1) const
	{
		HalfedgeHandle heh{INVALID_HANDLE};
		for (auto viter{VCirculator(this, vh0)}; viter.is_valid(); ++viter)
		{
			if (to_vh(viter.heh()) == vh1)
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
	bool TriMesh::is_isolated_vertex(VertexHandle vh) const
	{
		return outgoing_heh(vh) == INVALID_HANDLE;
	}

	inline 
	bool TriMesh::is_boundary_vertex(VertexHandle vh) const
	{
		return is_isolated_vertex(vh) || is_boundary_hedge(outgoing_heh(vh));
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
	TriMesh::HalfedgeHandle TriMesh::inner_heh(FaceHandle fh) const
	{
		return faces_conn_[fh].inner_heh_;
	}

	inline 
	bool TriMesh::add_face(VertexHandle vh0, VertexHandle vh1, VertexHandle vh2)
	{
		//务必是边界点
		if (is_boundary_vertex(vh0) && is_boundary_vertex(vh1) && is_boundary_vertex(vh2))
		{
			std::array const key_vhs{vh0, vh1, vh2, vh0};
			std::array key_hehs{
				find_heh(vh0, vh1), find_heh(vh1, vh2), find_heh(vh2, vh0), INVALID_HANDLE
			};
			for (int i = 0; i < 3; ++i)
			{
				if (key_hehs[i] != INVALID_HANDLE && !is_boundary_hedge(key_hehs[i]))
				{
					return false;
				}
			}
			key_hehs[3] = key_hehs[0];
			std::array<std::pair<HalfedgeHandle, HalfedgeHandle>, 18> hehpair_cache;
			int cache_count{};

			/*
			for (int i = 0; i < 3; ++i)
			{
				if (key_hehs[i] != INVALID_HANDLE && key_hehs[i + 1] != INVALID_HANDLE &&
					next_heh(key_hehs[i]) != key_hehs[i + 1])
				{
					auto iter_heh{opp_heh(key_hehs[i])};
					while (is_boundary_hedge(iter_heh) == false)
					{
						iter_heh = ccw_rotated_heh(iter_heh);
					}
					//非流型点只接受边界点的情况
					if (iter_heh == key_hehs[i + 1])
					{
						return false;
					}
					hehpair_cache[cache_count++] = {prev_heh(iter_heh), next_heh(key_hehs[i])};
					hehpair_cache[cache_count++] = {prev_heh(key_hehs[i + 1]), iter_heh};
					hehpair_cache[cache_count++] = {key_hehs[i], key_hehs[i + 1]};
				}
			}
			*/

			auto const lastHeh{static_cast<HalfedgeHandle>(he_n()-1)};
			for (int i = 0; i < 3; ++i)
			{
				if (key_hehs[i] == INVALID_HANDLE)
				{
					key_hehs[i] = new_edge(key_vhs[i], key_vhs[i + 1]);
				}
			}
			key_hehs[3] = key_hehs[0];
			faces_conn_.emplace_back();
			set_inner_heh(static_cast<FaceHandle>(f_n()-1),key_hehs[0]);

			for (int i = 0; i < 3; ++i)
			{
				auto const joint_vh{key_vhs[i + 1]};
				int case_id{};
				if (key_hehs[i] > lastHeh)
				{
					case_id |= 1;
				}
				if (key_hehs[i + 1] > lastHeh)
				{
					case_id |= 2;
				}
				switch (case_id)
				{
				case 1:
					break;
				case 2:
					break;
				case 3:
					break;
				default:
					break;
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
	TriMesh::HalfedgeHandle TriMesh::next_heh(HalfedgeHandle heh) const
	{
		return halfedges_conn_[heh].next_heh_;
	}
	
	inline
	TriMesh::HalfedgeHandle TriMesh::prev_heh(HalfedgeHandle heh) const
	{
		return halfedges_conn_[heh].prev_heh_;
	}
	
	inline
	TriMesh::FaceHandle TriMesh::adhereto_fh(HalfedgeHandle heh) const
	{
		return halfedges_conn_[heh].adt_fh_;
	}

	inline
	TriMesh::VertexHandle TriMesh::to_vh(HalfedgeHandle heh) const
	{
		return halfedges_conn_[heh].to_vh_;
	}

	inline
	TriMesh::HalfedgeHandle TriMesh::opp_heh(HalfedgeHandle heh) const
	{
		return (heh & 1) ? (heh - 1) : (heh + 1);
	}

	inline 
	TriMesh::HalfedgeHandle TriMesh::ccw_rotated_heh(HalfedgeHandle heh) const
	{
		return opp_heh(prev_heh(heh));
	}

	inline 
	TriMesh::HalfedgeHandle TriMesh::cw_rotated_heh(HalfedgeHandle heh) const
	{
		return next_heh(opp_heh(heh));
	}

	inline 
	bool TriMesh::is_boundary_hedge(HalfedgeHandle heh) const
	{
		return adhereto_fh(heh) == INVALID_HANDLE;
	}

	inline 
	bool TriMesh::set_inner_heh(FaceHandle fh, HalfedgeHandle heh)
	{
		faces_conn_[fh].inner_heh_ = heh;
		return true;
	}

	inline 
	TriMesh::HalfedgeHandle TriMesh::new_edge(VertexHandle vh0, VertexHandle vh1)
	{
		auto const he_count{static_cast<HalfedgeHandle>(he_n())};
		halfedges_conn_.emplace_back();
		set_to_vh(he_count, vh1);
		halfedges_conn_.emplace_back();
		set_to_vh(he_count + 1, vh0);
		return he_count;
	}

	inline 
	bool TriMesh::set_to_vh(HalfedgeHandle heh, VertexHandle vh)
	{
		halfedges_conn_[heh].to_vh_ = vh;
		return true;
	}
}
