#pragma once

#include "../core/config.h"
#include "../core/types.h"
#include <cstdint>
#include <vector>

namespace funu
{
	//��������
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

		//���˽ṹ
		struct HalfEdge
		{
			HalfEdge(): next_heh{INVALID_HANDLE}, prev_heh{INVALID_HANDLE}, adt_fh{INVALID_HANDLE}, to_vh{INVALID_HANDLE}
			{
			}
			//��һ�����
			HalfedgeHandle next_heh;
			//��һ�����
			HalfedgeHandle prev_heh;
			//������Ƭ
			FaceHandle adt_fh;
			//ָ��Ķ���
			VertexHandle to_vh;
		};

		struct Vertex
		{
			Vertex(): outg_heh{INVALID_HANDLE}
			{
			}
			//��������İ��(�߽��ʱ������Ϊĳ���߽���)
			HalfedgeHandle outg_heh;
		};

		struct Face
		{
			Face(): inner_heh{INVALID_HANDLE}
			{
			}
			//��Ƭ�������
			HalfedgeHandle inner_heh;
		};

	public:
		// API
		//�������
		//����
		size_t v_n() const;
		//�����
		HalfedgeHandle outgoing_hedge(VertexHandle vh) const;
		HalfedgeHandle find_hedge(VertexHandle vh0, VertexHandle vh1) const;
		//��ȡ��������
		Vec4& point(VertexHandle vh);
		Vec4 const& point(VertexHandle vh) const;
		//������
		bool is_isolated(VertexHandle vh) const;
		//�߽��
		bool is_boundary_vertex(VertexHandle vh) const;
		//�����͵�
		bool is_manifold(VertexHandle vh) const;
		//�ӵ�
		bool add_vertex(Vec4 const& pnt);
		//ɾ��
		bool remove_vertex(VertexHandle vh);

		//��Ƭ����
		//����
		size_t f_n() const;
		//�����İ��
		HalfedgeHandle inner_hedge(FaceHandle fh) const;

		//������Ƭ
		bool add_face(VertexHandle vh0, VertexHandle vh1, VertexHandle vh2);
		//ɾ����Ƭ
		bool remove_face(FaceHandle fh);

		//��߲���
		//�����
		size_t he_n() const;
		//����
		size_t e_n() const;
		//��һ�����
		HalfedgeHandle next_hedge(HalfedgeHandle heh) const;
		//��һ�����
		HalfedgeHandle prev_hedge(HalfedgeHandle heh) const;
		//������Ƭ
		FaceHandle adhereto_face(HalfedgeHandle heh) const;
		//ָ�򶥵�
		VertexHandle to_vertex(HalfedgeHandle heh) const;
		//������
		HalfedgeHandle opp_hedge(HalfedgeHandle heh) const;
		HalfedgeHandle ccw_rotated_hedge(HalfedgeHandle heh) const;
		HalfedgeHandle cw_rotated_hedge(HalfedgeHandle heh) const;
		//�߽���
		bool is_boundary_hedge(HalfedgeHandle heh) const;

	private:
		//�ڲ�api

	private:
		//���˽ṹ
		std::vector<Vertex> verts_conn_;
		std::vector<Face> faces_conn_;
		std::vector<HalfEdge> halfedges_conn_;

		//λ������ x,y,z,w
		std::vector<Vec4> points_;

		//�������
		std::vector<bool> verts_deleted_;
		std::vector<bool> faces_deleted_;
		std::vector<bool> halfedges_deleted_;
	};

	//�������˹�ϵ������
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