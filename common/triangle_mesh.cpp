#include <array>
#include "triangle_mesh.h"

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

	inline size_t TriMesh::v_n() const
	{
		return verts_conn_.size();
	}

	inline TriMesh::HalfedgeHandle TriMesh::outgoing_halfedge(VertexHandle vh) const
	{
		return verts_conn_[vh.idx()].outgoing_heh_;
	}

	inline TriMesh::HalfedgeHandle TriMesh::find_heh(VertexHandle vh0, VertexHandle vh1) const
	{
		HalfedgeHandle heh;
		for (auto viter{VCirculator(*this, vh0)}; viter.is_valid(); ++viter)
		{
			if (to_vh(viter.outgoing_heh()) == vh1)
			{
				heh = viter.outgoing_heh();
				break;
			}
		}
		return heh;
	}

	inline vec4f& TriMesh::point(VertexHandle vh)
	{
		return points_[vh.idx()];
	}

	inline vec4f const& TriMesh::point(VertexHandle vh) const
	{
		return points_[vh.idx()];
	}

	inline bool TriMesh::is_isolated_vertex(VertexHandle vh) const
	{
		return !outgoing_halfedge(vh).is_valid();
	}

	inline bool TriMesh::is_boundary_vertex(VertexHandle vh) const
	{
		return is_isolated_vertex(vh) || is_boundary_halfedge(outgoing_halfedge(vh));
	}

	inline bool TriMesh::is_manifold(VertexHandle vh) const
	{
		int outg_num{};
		for (auto viter{VCirculator(*this, vh)}; viter.is_valid(); ++viter)
		{
			if (is_boundary_halfedge(viter.outgoing_heh()))
			{
				++outg_num;
			}
		}
		return outg_num < 2;
	}

	inline void TriMesh::add_vertex(vec4f const& pnt)
	{
		verts_conn_.emplace_back();
		points_.push_back(pnt);
		verts_removed_.push_back(0);
	}

	inline void TriMesh::remove_vertex(VertexHandle vh)
	{
		verts_removed_[vh.idx()] = 1;
	}

	inline size_t TriMesh::f_n() const
	{
		return faces_conn_.size();
	}

	inline TriMesh::HalfedgeHandle TriMesh::inner_heh(FaceHandle fh) const
	{
		return faces_conn_[fh.idx()].inner_heh_;
	}

	inline TriMesh::FaceHandle TriMesh::add_face(VertexHandle vh0, VertexHandle vh1, VertexHandle vh2)
	{
		//务必都是边界点
		if (is_boundary_vertex(vh0) && is_boundary_vertex(vh1) && is_boundary_vertex(vh2))
		{
			std::array const key_vhs{vh0, vh1, vh2, vh0};
			std::array key_hehs{
				find_heh(vh0, vh1), find_heh(vh1, vh2), find_heh(vh2, vh0), HalfedgeHandle{}
			};
			for (int i = 0; i < 3; ++i)
			{
				if (key_hehs[i].is_valid() && !is_boundary_halfedge(key_hehs[i]))
				{
					return {};
				}
			}
			key_hehs[3] = key_hehs[0];

			for (int i = 0; i < 3; ++i)
			{
				if (key_hehs[i].is_valid() && key_hehs[i + 1].is_valid() &&
					next_heh(key_hehs[i]) != key_hehs[i + 1])
				{
					auto iter_heh{opposite_heh(key_hehs[i])};
					while (is_boundary_halfedge(iter_heh) == false)
					{
						iter_heh = ccw_outgoing_heh(iter_heh);
					}
					//非流型点只接受边界点的情况
					if (iter_heh == key_hehs[i + 1])
					{
						return {};
					}
					link_two_hehs(prev_heh(iter_heh), next_heh(key_hehs[i]));
					link_two_hehs(prev_heh(key_hehs[i + 1]), iter_heh);
					link_two_hehs(key_hehs[i], key_hehs[i + 1]);
				}
			}

			auto const lastHeh{static_cast<HalfedgeHandle>(he_n() - 1)};
			for (int i = 0; i < 3; ++i)
			{
				if (!key_hehs[i].is_valid())
				{
					key_hehs[i] = new_edge(key_vhs[i], key_vhs[i + 1]);
				}
			}
			key_hehs[3] = key_hehs[0];

			//加入面片
			faces_conn_.emplace_back();
			faces_removed_.push_back(0);
			FaceHandle const curr_fh{static_cast<FaceHandle>(f_n() - 1)};
			set_inner_heh(curr_fh, key_hehs[0]);

			//设置外围半边的拓扑数据
			for (int i = 0; i < 3; ++i)
			{
				HalfedgeHandle const outer_prev{opposite_heh(key_hehs[i + 1])};
				HalfedgeHandle const outer_next{opposite_heh(key_hehs[i])};
				HalfedgeHandle boundary_prev;
				HalfedgeHandle boundary_next;
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
				// key_hehs[i]-->key_hehs[i+1]
				switch (case_id)
				{
				case 0:
					//旧-->旧
					if (outgoing_halfedge(joint_vh) == key_hehs[i + 1])
					{
						for (auto viter{VCirculator(*this, joint_vh)}; viter.is_valid(); ++viter)
						{
							if (is_boundary_halfedge(viter.outgoing_heh()))
							{
								set_outgoing_heh(joint_vh, viter.outgoing_heh());
								break;
							}
						}
					}
					break;
				case 1:
					// 新-->旧
					boundary_prev = prev_heh(key_hehs[i + 1]);
					link_two_hehs(boundary_prev, outer_next);
					set_outgoing_heh(joint_vh, outer_next);
					break;
				case 2:
					// 旧-->新
					boundary_next = next_heh(key_hehs[i]);
					link_two_hehs(outer_prev, boundary_next);
					set_outgoing_heh(joint_vh, boundary_next);
					break;
				case 3:
					//新-->新
					//孤立点
					if (is_isolated_vertex(joint_vh))
					{
						set_outgoing_heh(joint_vh, outer_next);
						link_two_hehs(outer_prev, outer_next);
					}
					//非孤立点
					else
					{
						boundary_next = outgoing_halfedge(joint_vh);
						boundary_prev = prev_heh(boundary_next);
						link_two_hehs(outer_prev, boundary_next);
						link_two_hehs(boundary_prev, outer_next);
					}
					break;
				default:
					break;
				}
				if (case_id)
				{
					link_two_hehs(key_hehs[i], key_hehs[i + 1]);
				}
				set_adhere_to_fh(key_hehs[i], curr_fh);
			}
			return curr_fh;
		}
		return {};
	}

	inline void TriMesh::remove_face(FaceHandle fh)
	{
		faces_removed_[fh.idx()] = 1;
	}

	inline size_t TriMesh::he_n() const
	{
		return halfedges_conn_.size();
	}

	inline size_t TriMesh::e_n() const
	{
		return halfedges_conn_.size() >> 1;
	}

	inline TriMesh::HalfedgeHandle TriMesh::next_heh(HalfedgeHandle heh) const
	{
		return halfedges_conn_[heh.idx()].next_heh_;
	}
	
	inline TriMesh::HalfedgeHandle TriMesh::prev_heh(HalfedgeHandle heh) const
	{
		return halfedges_conn_[heh.idx()].prev_heh_;
	}
	
	inline TriMesh::FaceHandle TriMesh::adhere_to_fh(HalfedgeHandle heh) const
	{
		return halfedges_conn_[heh.idx()].adhere_to_fh_;
	}

	inline TriMesh::VertexHandle TriMesh::to_vh(HalfedgeHandle heh) const
	{
		return halfedges_conn_[heh.idx()].to_vh_;
	}

	inline TriMesh::HalfedgeHandle TriMesh::opposite_heh(HalfedgeHandle heh) const
	{
		return HalfedgeHandle((heh.idx() & 1) ? (heh.idx() - 1) : (heh.idx() + 1));
	}

	inline TriMesh::HalfedgeHandle TriMesh::ccw_outgoing_heh(HalfedgeHandle heh) const
	{
		return opposite_heh(prev_heh(heh));
	}

	inline TriMesh::HalfedgeHandle TriMesh::cw_outgoing_heh(HalfedgeHandle heh) const
	{
		return next_heh(opposite_heh(heh));
	}

	inline TriMesh::HalfedgeHandle TriMesh::ccw_incoming_heh(HalfedgeHandle heh) const
	{
		return prev_heh(opposite_heh(heh));
	}

	inline TriMesh::HalfedgeHandle TriMesh::cw_incoming_heh(HalfedgeHandle heh) const
	{
		return opposite_heh(next_heh(heh));
	}

	inline bool TriMesh::is_boundary_halfedge(HalfedgeHandle heh) const
	{
		return !adhere_to_fh(heh).is_valid();
	}

	inline void TriMesh::set_outgoing_heh(VertexHandle vh, HalfedgeHandle heh)
	{
		verts_conn_[vh.idx()].outgoing_heh_ = heh;
	}

	inline void TriMesh::set_inner_heh(FaceHandle fh, HalfedgeHandle heh)
	{
		faces_conn_[fh.idx()].inner_heh_ = heh;
	}

	inline TriMesh::HalfedgeHandle TriMesh::new_edge(VertexHandle vh0, VertexHandle vh1)
	{
		int const he_count{static_cast<int>(he_n())};
		halfedges_conn_.emplace_back();
		set_to_vh(HalfedgeHandle(he_count), vh1);
		halfedges_conn_.emplace_back();
		set_to_vh(HalfedgeHandle(he_count + 1), vh0);
		return HalfedgeHandle(he_count);
	}

	inline void TriMesh::set_to_vh(HalfedgeHandle heh, VertexHandle vh)
	{
		halfedges_conn_[heh.idx()].to_vh_ = vh;
	}

	inline void TriMesh::set_adhere_to_fh(HalfedgeHandle heh, FaceHandle fh)
	{
		halfedges_conn_[heh.idx()].adhere_to_fh_ = fh;
	}

	inline void TriMesh::set_next_heh(HalfedgeHandle heh0, HalfedgeHandle heh1)
	{
		halfedges_conn_[heh0.idx()].next_heh_ = heh1;
	}

	inline void TriMesh::set_prev_heh(HalfedgeHandle heh0, HalfedgeHandle heh1)
	{
		halfedges_conn_[heh0.idx()].prev_heh_ = heh1;
	}

	inline void TriMesh::link_two_hehs(HalfedgeHandle heh0, HalfedgeHandle heh1)
	{
		set_next_heh(heh0,heh1);
		set_prev_heh(heh1,heh0);
	}

	VCirculator::VCirculator(TriMesh const& mesh, TriMesh::VertexHandle vh): mesh_{mesh},
	                                                                         start_heh_{mesh_.outgoing_halfedge(vh)},
	                                                                         curr_heh_{start_heh_},
	                                                                         rotate_count_{}
	{
	}

	inline VCirculator& VCirculator::operator++()
	{
		curr_heh_ = mesh_.ccw_outgoing_heh(curr_heh_);
		++rotate_count_;
		return *this;
	}

	inline VCirculator& VCirculator::operator--()
	{
		curr_heh_ = mesh_.cw_outgoing_heh(curr_heh_);
		--rotate_count_;
		return *this;
	}

	inline bool VCirculator::is_valid() const
	{
		return curr_heh_ != start_heh_ || !rotate_count_;
	}

	inline TriMesh::HalfedgeHandle VCirculator::outgoing_heh() const
	{
		return curr_heh_;
	}

	inline TriMesh::HalfedgeHandle VCirculator::incoming_heh() const
	{
		return mesh_.opposite_heh(curr_heh_);
	}

	inline TriMesh::VertexHandle VCirculator::vh() const
	{
		return mesh_.to_vh(curr_heh_);
	}

	FCirculator::FCirculator(TriMesh const& mesh, TriMesh::FaceHandle fh): mesh_{mesh},
	                                                                       prev_inner_heh_{mesh_.inner_heh(fh)},
	                                                                       next_inner_heh_{
		                                                                       mesh_.next_heh(prev_inner_heh_)
	                                                                       },
	                                                                       start_heh_{
		                                                                       find_target_oheh(prev_inner_heh_)
	                                                                       },
	                                                                       curr_heh_{start_heh_}, rotate_count_{}
	{
	}

	inline FCirculator& FCirculator::operator++()
	{
		curr_heh_ = mesh_.ccw_outgoing_heh(curr_heh_);
		if (curr_heh_ == next_inner_heh_)
		{
			prev_inner_heh_ = next_inner_heh_;
			next_inner_heh_ = mesh_.next_heh(next_inner_heh_);
			curr_heh_ = find_target_oheh(prev_inner_heh_);
		}
		++rotate_count_;
		return *this;
	}

	inline FCirculator& FCirculator::operator--()
	{
		curr_heh_ = mesh_.cw_outgoing_heh(curr_heh_);
		if (curr_heh_ == mesh_.opposite_heh(prev_inner_heh_))
		{
			next_inner_heh_ = prev_inner_heh_;
			prev_inner_heh_ = mesh_.prev_heh(prev_inner_heh_);
			curr_heh_ = mesh_.next_heh(curr_heh_);
		}
		--rotate_count_;
		return *this;
	}

	inline bool FCirculator::is_valid() const
	{
		return curr_heh_ != start_heh_ || !rotate_count_;
	}

	inline TriMesh::HalfedgeHandle FCirculator::outgoing_heh() const
	{
		return curr_heh_;
	}

	inline TriMesh::HalfedgeHandle FCirculator::incoming_heh() const
	{
		return mesh_.opposite_heh(curr_heh_);
	}

	inline TriMesh::HalfedgeHandle FCirculator::find_target_oheh(TriMesh::HalfedgeHandle inner_heh) const
	{
		return mesh_.ccw_outgoing_heh(mesh_.opposite_heh(inner_heh));
	}


}