//
// Created by hhcoo on 2023/3/18.
//

#ifndef FUNU_SRC_BASE_HALFEDGE_H_
#define FUNU_SRC_BASE_HALFEDGE_H_

#include <vector>

#include "vec.h"

template<typename PointType>
class TriMesh
{
public:
	
	struct HalfEdge
	{
		//半边所属的面片 -1表示边界半边
		int face_;
		//该面片内下一条半边
		int nextHalfEdge_;
		//反向半边
		int oppositeHalfEdge_;
		//这个半边所指向的点
		int toVertex_;
	};
	
	struct Edge
	{
		Edge(HalfEdge he0, HalfEdge he1) : halfEdge0_{ he0 }, halfEdge1_{ he1 }
		{
		}
		int halfEdge0_;
		int halfEdge1_;
	};
	
	struct Face
	{
		Face() : halfEdge_{ -1 }
		{
		}
		//面片所包含的一条半边
		int halfEdge_;
	};
	
	struct Vertex
	{
		explicit Vertex(PointType pnt) : pos_(pnt), startHalfEdge_{ -1 }
		{
		}
		//坐标数据
		PointType pos_;
		//起始半边 -1表示孤立点
		int startHalfEdge_;
	};
	
	using VertexContainer = std::vector<Vertex>;
	using HalfEdgeContainer = std::vector<HalfEdge>;
	using EdgeContainer = std::vector<Edge>;
	using FaceContainer = std::vector<Face>;
	
	//顶点处理
	bool addVertex(Vertex v);
	bool addVertices(VertexContainer const& vertices);
	
	//面片处理
	bool addFace(Vertex v0, Vertex v1, Vertex v2);
	//bool addFace(Face f);
	//bool addFaces(FaceContainer const& f);

private:
	VertexContainer vertices_;
	HalfEdgeContainer halfedges_;
	EdgeContainer edges_;
	FaceContainer faces_;
};

#endif //FUNU_SRC_BASE_HALFEDGE_H_
