#pragma once

#include <cstdint>
#include <Eigen/Dense>

namespace funu
{
	using Mat44 = Eigen::Matrix4f;
	using Mat33 = Eigen::Matrix3f;

	using Vec2 = Eigen::Vector2f;
	using Vec3 = Eigen::Vector3f;
	using Vec4 = Eigen::Vector4f;

	using IndexType = std::uint32_t;
	static constexpr IndexType INVALID_IDX{std::numeric_limits<IndexType>::max()};
}
