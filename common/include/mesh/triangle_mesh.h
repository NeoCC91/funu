#pragma once

#include "../core/config.h"
#include "../core/types.h"
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

		//���˽ṹ
		struct HalfEdge
		{
			HalfEdge(): next{INVALID_IDX}, prev{INVALID_IDX}, face{INVALID_IDX}, to{INVALID_IDX}
			{
			}
			//��һ�����
			IndexType next;
			//��һ�����
			IndexType prev;
			//������Ƭ
			IndexType face;
			//ָ��Ķ���
			IndexType to;
		};

		struct Vertex
		{
			Vertex(): out{INVALID_IDX}
			{
			}
			//�������������һ�����
			IndexType out;
		};

		struct Face
		{
			Face(): hedge{INVALID_IDX}
			{
			}
			//��Ƭ�������
			IndexType hedge;
		};

	public:
		// API
		//�������
		//����
		size_t v_n() const;
		bool add_vertex(Vec4 const& pnt);
		bool remove_vertex(IndexType vert_idx);

		//��Ƭ����
		//����
		size_t f_n() const;


		bool add_face(IndexType vert_idx0, IndexType vert_idx1, IndexType vert_idx2);


		bool remove_face(IndexType face_idx);

		//��߲���
		//�����
		size_t he_n() const;
		//����
		size_t e_n() const;

	private:
		//�ڲ�api

	private:
		//���˽ṹ
		std::vector<Vertex> verts_;
		std::vector<Face> faces_;
		std::vector<HalfEdge> halfedges_;

		//λ������
		std::vector<Vec4> points_;

		//�������
		std::vector<bool> verts_deleted_;
		std::vector<bool> faces_deleted_;
		std::vector<bool> halfedges_deleted_;
	};
}
