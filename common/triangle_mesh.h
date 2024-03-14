#pragma once

#include <compare>
#include <vector>
#include <Eigen/Dense>

namespace funu
{
	//三角网格
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

		//基础元素索引
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

		//拓扑信息类型
		//顶点拓扑
		struct VertexTopology
		{
			//顶点出发的半边(边界点时，出边为某个边界半边)
			HalfedgeHandle outgoing_heh_;
		};

		//面片拓扑
		struct FaceTopology
		{
			//面片所含某个半边
			HalfedgeHandle inner_heh_;
		};

		//半边拓扑
		struct HalfEdgeTopology
		{
			//上一个半边
			HalfedgeHandle prev_heh_;
			//下一个半边
			HalfedgeHandle next_heh_;
			//所属面片
			FaceHandle adhere_to_fh_;
			//指向的顶点
			VertexHandle to_vh_;
		};

		//顶点操作
		//顶点数量
		size_t v_n() const;

		//出半边
		HalfedgeHandle outgoing_halfedge(VertexHandle vh) const;

		//获取vh0出发到vh1的半边
		HalfedgeHandle find_heh(VertexHandle vh0, VertexHandle vh1) const;

		//获取坐标数据
		Point& point(VertexHandle vh);
		Point const& point(VertexHandle vh) const;

		//孤立点
		bool is_isolated_vertex(VertexHandle vh) const;
		//边界点(包含孤立点)
		bool is_boundary_vertex(VertexHandle vh) const;
		//非流型点
		bool is_manifold(VertexHandle vh) const;
		//加点
		void add_vertex(Point const& pnt);
		//删点
		void remove_vertex(VertexHandle vh);

		//面片操作
		//面数量
		size_t f_n() const;
		//包含的半边
		HalfedgeHandle inner_heh(FaceHandle fh) const;

		//添加面片
		FaceHandle add_face(VertexHandle vh0, VertexHandle vh1, VertexHandle vh2);
		//删除面片
		void remove_face(FaceHandle fh);

		//半边操作
		//半边数量
		size_t he_n() const;
		//边数量
		size_t e_n() const;
		//下一条半边
		HalfedgeHandle next_heh(HalfedgeHandle heh) const;
		//上一条半边
		HalfedgeHandle prev_heh(HalfedgeHandle heh) const;
		//所属面片
		FaceHandle adhere_to_fh(HalfedgeHandle heh) const;
		//指向顶点
		VertexHandle to_vh(HalfedgeHandle heh) const;
		//反向半边
		HalfedgeHandle opposite_heh(HalfedgeHandle heh) const;
		//outgoing heh
		HalfedgeHandle ccw_outgoing_heh(HalfedgeHandle heh) const;
		HalfedgeHandle cw_outgoing_heh(HalfedgeHandle heh) const;
		//incoming heh
		HalfedgeHandle ccw_incoming_heh(HalfedgeHandle heh) const;
		HalfedgeHandle cw_incoming_heh(HalfedgeHandle heh) const;
		//边界半边
		bool is_boundary_halfedge(HalfedgeHandle heh) const;

	private:
		//内部操作和数据
		//点
		void set_outgoing_heh(VertexHandle vh, HalfedgeHandle heh);

		//面
		void set_inner_heh(FaceHandle fh, HalfedgeHandle heh);

		//边
		HalfedgeHandle new_edge(VertexHandle vh0, VertexHandle vh1);
		void set_to_vh(HalfedgeHandle heh, VertexHandle vh);
		void set_adhere_to_fh(HalfedgeHandle heh, FaceHandle fh);
		void set_next_heh(HalfedgeHandle heh0, HalfedgeHandle heh1);
		void set_prev_heh(HalfedgeHandle heh0, HalfedgeHandle heh1);
		void link_two_hehs(HalfedgeHandle heh0, HalfedgeHandle heh1);

		//拓扑结构信息
		std::vector<VertexTopology> verts_conn_;
		std::vector<FaceTopology> faces_conn_;
		std::vector<HalfEdgeTopology> halfedges_conn_;

		//位置数据 x,y,z,w
		std::vector<Point> points_;

		//删除标记数据
		std::vector<int> verts_removed_;
		std::vector<int> faces_removed_;
		std::vector<int> halfedges_removed_;
	};

	//顶点拓扑关系访问器
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

	//面片拓扑关系访问器
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