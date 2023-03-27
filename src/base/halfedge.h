//
// Created by hhcoo on 2023/3/18.
//

#ifndef FUNU_SRC_BASE_HALFEDGE_H_
#define FUNU_SRC_BASE_HALFEDGE_H_

#include <vector>
#include "vec.h"

namespace funu
{
	template<typename PointType>
	class TriMesh
	{
	public:
		
		using VertexHandle = int;
		using FaceHandle = int;
		using HalfEdgeHandle = int;
		using EdgeHandle = int;
		
		struct Vertex
		{
			explicit Vertex(PointType pnt) : pos_(pnt), startHalfEdge_{ -1 }
			{
			}
			//坐标数据
			PointType pos_;
			//起始半边 -1表示孤立点
			HalfEdgeHandle startHalfEdge_;
		};
		
		struct Face
		{
			Face() : halfEdge_{ -1 }
			{
			}
			//面片所包含的一条半边
			HalfEdgeHandle halfEdge_;
		};
		
		struct HalfEdge
		{
			//半边所属的面片 -1表示边界半边
			FaceHandle face_;
			//该面片内下一条半边
			HalfEdgeHandle nextHalfEdge_;
			//反向半边
			HalfEdgeHandle oppositeHalfEdge_;
			//这个半边所指向的点
			VertexHandle toVertex_;
			//int fromVertex_;
		};
		
		struct Edge
		{
			Edge(HalfEdge he0, HalfEdge he1) : halfEdge0_{ he0 }, halfEdge1_{ he1 }
			{
			}
			HalfEdgeHandle halfEdge0_;
			HalfEdgeHandle halfEdge1_;
		};
		
		using VertexContainer = std::vector<Vertex>;
		using FaceContainer = std::vector<Face>;
		using HalfEdgeContainer = std::vector<HalfEdge>;
		using EdgeContainer = std::vector<Edge>;
		
		//顶点处理
		//查询点信息
		int getVNum()
		{
			return numV_;
		}
		//添加点
		VertexHandle addVertex(Vertex v);
		//bool addVertices(VertexContainer const& vertices);
		//删除点
		bool deleteVertex(VertexHandle vh);
		
		//面片处理
		//查询面片信息
		int getFNum()
		{
			return numF_;
		}
		//添加面片,不可出现非流行
		FaceHandle addFace(VertexHandle vh0, VertexHandle vh1, VertexHandle vh2);
		//删除面片,点保留，可能会出现孤立点
		bool deleteFace(FaceHandle fh);
		
		//边处理
		//查询边信息
		int getHENum()
		{
			return numHE_;
		}
		int getENum()
		{
			return numE_;
		}
	
	private:
		//点信息
		//有效顶点个数
		int numV_;
		VertexContainer vertices_;
		
		//面信息
		//有效面片个数
		int numF_;
		FaceContainer faces_;
		
		//边和半边信息
		//有效边和半边个数
		int numHE_;
		HalfEdgeContainer halfedges_;
		int numE_;
		EdgeContainer edges_;
	};
	
	template<typename PointType>
	auto TriMesh<PointType>::addVertex(TriMesh::Vertex v) -> TriMesh::VertexHandle
	{
		vertices_.push_back(v);
		return vertices_.size() - 1;
	}
	
}
#endif //FUNU_SRC_BASE_HALFEDGE_H_
