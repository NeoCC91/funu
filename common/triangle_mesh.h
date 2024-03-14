#pragma once

#include <compare>
#include <vector>
#include <Eigen/Dense>

namespace funu
{
	//��������
	class TriMesh
	{
	public:
		TriMesh();
		TriMesh(TriMesh const& src);
		TriMesh(TriMesh&& src) noexcept;
		~TriMesh();

		TriMesh& operator=(TriMesh const& rhs);
		TriMesh& operator=(TriMesh&& rhs) noexcept;

		using Point = Eigen::Vector4f;
		using Dir = Eigen::Vector4f;

		//����Ԫ������
		struct Handle
		{
			[[nodiscard]] int idx() const { return idx_; }
			[[nodiscard]] explicit operator int() const { return idx(); }
			[[nodiscard]] bool is_valid() const { return idx_ >= 0; }
			[[nodiscard]] std::strong_ordering operator<=>(Handle const& rhs) const = default;

		protected:
			Handle() = default;

			explicit Handle(int const idx): idx_(idx)
			{
			}

			int idx_{-1};
		};

		struct VertexHandle : Handle
		{
			VertexHandle() = default;

			explicit VertexHandle(int const idx): Handle(idx)
			{
			}
		};

		struct FaceHandle : Handle
		{
			FaceHandle() = default;

			explicit FaceHandle(int const idx): Handle(idx)
			{
			}
		};

		struct HalfedgeHandle : Handle
		{
			HalfedgeHandle() = default;

			explicit HalfedgeHandle(int const idx): Handle(idx)
			{
			}
		};

		//������Ϣ����
		//��������
		struct VertexTopology
		{
			//��������İ��(�߽��ʱ������Ϊĳ���߽���)
			HalfedgeHandle outgoing_heh_;
		};

		//��Ƭ����
		struct FaceTopology
		{
			//��Ƭ����ĳ�����
			HalfedgeHandle inner_heh_;
		};

		//�������
		struct HalfEdgeTopology
		{
			//��һ�����
			HalfedgeHandle prev_heh_;
			//��һ�����
			HalfedgeHandle next_heh_;
			//������Ƭ
			FaceHandle adhere_to_fh_;
			//ָ��Ķ���
			VertexHandle to_vh_;
		};

		//�������
		//��������
		size_t v_n() const;

		//�����
		HalfedgeHandle outgoing_halfedge(VertexHandle vh) const;

		//��ȡvh0������vh1�İ��
		HalfedgeHandle find_heh(VertexHandle vh0, VertexHandle vh1) const;

		//��ȡ��������
		Point& point(VertexHandle vh);
		Point const& point(VertexHandle vh) const;

		//������
		bool is_isolated_vertex(VertexHandle vh) const;
		//�߽��(����������)
		bool is_boundary_vertex(VertexHandle vh) const;
		//�����͵�
		bool is_manifold(VertexHandle vh) const;
		//�ӵ�
		void add_vertex(Point const& pnt);
		//ɾ��
		void remove_vertex(VertexHandle vh);

		//��Ƭ����
		//������
		size_t f_n() const;
		//�����İ��
		HalfedgeHandle inner_heh(FaceHandle fh) const;

		//�����Ƭ
		FaceHandle add_face(VertexHandle vh0, VertexHandle vh1, VertexHandle vh2);
		//ɾ����Ƭ
		void remove_face(FaceHandle fh);

		//��߲���
		//�������
		size_t he_n() const;
		//������
		size_t e_n() const;
		//��һ�����
		HalfedgeHandle next_heh(HalfedgeHandle heh) const;
		//��һ�����
		HalfedgeHandle prev_heh(HalfedgeHandle heh) const;
		//������Ƭ
		FaceHandle adhere_to_fh(HalfedgeHandle heh) const;
		//ָ�򶥵�
		VertexHandle to_vh(HalfedgeHandle heh) const;
		//������
		HalfedgeHandle opposite_heh(HalfedgeHandle heh) const;
		//outgoing heh
		HalfedgeHandle ccw_outgoing_heh(HalfedgeHandle heh) const;
		HalfedgeHandle cw_outgoing_heh(HalfedgeHandle heh) const;
		//incoming heh
		HalfedgeHandle ccw_incoming_heh(HalfedgeHandle heh) const;
		HalfedgeHandle cw_incoming_heh(HalfedgeHandle heh) const;
		//�߽���
		bool is_boundary_halfedge(HalfedgeHandle heh) const;

	private:
		//�ڲ�����������
		//��
		void set_outgoing_heh(VertexHandle vh, HalfedgeHandle heh);

		//��
		void set_inner_heh(FaceHandle fh, HalfedgeHandle heh);

		//��
		HalfedgeHandle new_edge(VertexHandle vh0, VertexHandle vh1);
		void set_to_vh(HalfedgeHandle heh, VertexHandle vh);
		void set_adhere_to_fh(HalfedgeHandle heh, FaceHandle fh);
		void set_next_heh(HalfedgeHandle heh0, HalfedgeHandle heh1);
		void set_prev_heh(HalfedgeHandle heh0, HalfedgeHandle heh1);
		void link_two_hehs(HalfedgeHandle heh0, HalfedgeHandle heh1);

		//���˽ṹ��Ϣ
		std::vector<VertexTopology> verts_conn_;
		std::vector<FaceTopology> faces_conn_;
		std::vector<HalfEdgeTopology> halfedges_conn_;

		//λ������ x,y,z,w
		std::vector<Point> points_;

		//ɾ���������
		std::vector<int> verts_removed_;
		std::vector<int> faces_removed_;
		std::vector<int> halfedges_removed_;
	};

	//�������˹�ϵ������
	class VCirculator
	{
	public:
		VCirculator(TriMesh const& mesh, TriMesh::VertexHandle vh);

		//ccw
		VCirculator& operator++();

		//cw
		VCirculator& operator--();

		bool is_valid() const;

		TriMesh::HalfedgeHandle outgoing_heh() const;

		TriMesh::HalfedgeHandle incoming_heh() const;

		TriMesh::VertexHandle vh() const;

	private:
		TriMesh const& mesh_;
		TriMesh::HalfedgeHandle start_heh_;
		TriMesh::HalfedgeHandle curr_heh_;
		int rotate_count_;
	};

	//��Ƭ���˹�ϵ������
	class FCirculator
	{
	public:
		FCirculator(TriMesh const& mesh, TriMesh::FaceHandle fh);

		//ccw
		FCirculator& operator++();

		//cw
		FCirculator& operator--();

		bool is_valid() const;

		TriMesh::HalfedgeHandle outgoing_heh() const;

		TriMesh::HalfedgeHandle incoming_heh() const;

	private:
		TriMesh::HalfedgeHandle find_target_oheh(TriMesh::HalfedgeHandle inner_heh) const;

		TriMesh const& mesh_;
		TriMesh::HalfedgeHandle prev_inner_heh_;
		TriMesh::HalfedgeHandle next_inner_heh_;
		TriMesh::HalfedgeHandle start_heh_;
		TriMesh::HalfedgeHandle curr_heh_;
		int rotate_count_;
	};
}