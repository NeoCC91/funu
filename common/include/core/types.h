#pragma once

#include <cstdint>
#include <Eigen/Dense>

namespace funu
{
	using Mat4x4f = Eigen::Matrix4f;
	using Mat3x3f = Eigen::Matrix3f;

	using Vec2f = Eigen::Vector2f;
	using Vec3f = Eigen::Vector3f;
	using Vec4f = Eigen::Vector4f;

	using IndexType = std::uint32_t;
}
