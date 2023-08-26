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
		HalfedgeHandle& outgoing_hedge(VertexHandle vh);
		HalfedgeHandle const& outgoing_hedge(VertexHandle vh) const;
		//��ȡ��������
		Vec4& point(VertexHandle vh);
		Vec4 const& point(VertexHandle vh) const;
		//������
		bool is_isolated(VertexHandle vh) const;
		//�߽��
		bool is_boundary(VertexHandle vh) const;
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
		HalfedgeHandle& inner_hedge(FaceHandle fh);
		HalfedgeHandle const& inner_hedge(FaceHandle fh) const;

		//�����Ƭ
		bool add_face(VertexHandle vh0, VertexHandle vh1, VertexHandle vh2);
		//ɾ����Ƭ
		bool remove_face(FaceHandle fh);

		//��߲���
		//�����
		size_t he_n() const;
		//����
		size_t e_n() const;
		//��һ�����
		HalfedgeHandle& next_hedge(HalfedgeHandle heh);
		HalfedgeHandle const& next_hedge(HalfedgeHandle heh) const;
		//��һ�����
		HalfedgeHandle& prev_hedge(HalfedgeHandle heh);
		HalfedgeHandle const& prev_hedge(HalfedgeHandle heh) const;
		//������Ƭ
		FaceHandle& adhereto_face(HalfedgeHandle heh);
		FaceHandle const& adhereto_face(HalfedgeHandle heh) const;
		//ָ�򶥵�
		VertexHandle& to_vertex(HalfedgeHandle heh);
		VertexHandle const& to_vertex(HalfedgeHandle heh) const;
		//������
		HalfedgeHandle opp_hedge(HalfedgeHandle heh) const;
		HalfedgeHandle ccw_rotated_hedge(HalfedgeHandle heh) const;
		HalfedgeHandle cw_rotated_hedge(HalfedgeHandle heh) const;

		//������߷�����
		friend class VOheCirculator;

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

	//������
	class VOheCirculator
	{
	public:
		VOheCirculator(TriMesh const* mesh, TriMesh::VertexHandle vh): mesh_{mesh}, start_vh_{vh},
		                                                      outg_heh_{mesh_->verts_conn_[vh].outg_heh}
		{
		}

		TriMesh::HalfedgeHandle operator*() const
		{
			return outg_heh_;
		}

		//ccw
		void operator++()
		{
			
		}

		//cw
		void operator--()
		{
			
		}

	private:
		TriMesh const* mesh_;
		TriMesh::VertexHandle start_vh_;
		TriMesh::HalfedgeHandle outg_heh_;
	};
}
