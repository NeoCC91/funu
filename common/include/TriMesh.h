#pragma once

#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Geometry/EigenVectorT.hh>

namespace funu
{
	struct EigenTraits : OpenMesh::DefaultTraits
	{
		using Point = Eigen::Vector4f;
		using Normal = Eigen::Vector4f;
		using TexCoord2D = Eigen::Vector4f;
		using TexCoord3D = Eigen::Vector4f;
		using Color = Eigen::Vector4f;
	};

	using TriMesh = OpenMesh::TriMesh_ArrayKernelT<EigenTraits>;
}