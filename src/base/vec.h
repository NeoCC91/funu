//
// Created by hhcoo on 2023/2/28.
//

#ifndef FUNU_VEC_H
#define FUNU_VEC_H

#include <array>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <exception>

namespace funu
{
	template<typename scalarType, int row, int col>
	class Matrix
	{
	public:
		Matrix() : arrs_{}
		{
		}
		~Matrix() = default;
		
	
	public:
		std::array<std::array<scalarType, row>, col> arrs_;
	};
	
	template<typename scalarType, int dimension>
	class Vec
	{
	public:
		explicit Vec(scalarType args...) : arr_{ std::forward<scalarType>(args) }
		{
		
		}
		Vec() : arr_{}
		{
		}
		
		void set_zero()
		{
			operator=(operator*(static_cast<scalarType>(0)));
			//std::memset(arr_,static_cast<scalarType>(0),dimension);
		}
		
		[[nodiscard]] bool is_zero() const
		{
			for (int i = 0; i < dimension; ++i)
			{
				if (arr_[i] != static_cast<scalarType>(0))
				{
					return false;
				}
			}
			return true;
		}
		
		scalarType squared_norm() const
		{
			return operator*(*this);
		}
		
		scalarType norm() const
		{
			return std::sqrt(squared_norm());
		}
		
		void normalized()
		{
			auto const copy{ normalize() };
			//std::copy(copy.arr_.begin(), copy.arr_.end(), arr_.begin());
			operator=(copy);
		}
		Vec normalize() const
		{
			if (is_zero())
			{
				throw std::exception("zero vector!");
			}
			auto const factor{ 1 / norm() };
			return operator*(factor);
		}
		
		Vec operator-(Vec const& rhs) const
		{
			Vec res{};
			for (int i = 0; i < dimension; ++i)
			{
				res[i] = arr_[i] - rhs.arr_[i];
			}
			return res;
		}
		
		void operator-(int)
		{
			for (int i = 0; i < dimension; ++i)
			{
				arr_[i] = -arr_[i];
			}
		}
		
		Vec operator+(Vec const& rhs) const
		{
			Vec res{};
			for (int i = 0; i < dimension; ++i)
			{
				res[i] = arr_[i] + rhs.arr_[i];
			}
			return res;
		}
		
		scalarType operator*(Vec const& rhs) const
		{
			scalarType sum{};
			for (int i = 0; i < dimension; ++i)
			{
				sum += arr_[i] * rhs.arr_[i];
			}
			return sum;
		}
		
		Vec operator*(scalarType factor) const
		{
			auto copy{ *this };
			std::transform(copy.arr_.begin(), copy.arr_.end(), [factor](auto const val)
			{
				return val * factor;
			});
			return copy;
		}
		
		~Vec() = default;
	
	public:
		std::array<scalarType, dimension> arr_;
	};
	
	template<typename scalarType>
	using Vec2 = Vec<scalarType, 2>;
	using vec2f = Vec2<float>;
	using vec2d = Vec2<double>;
	
	template<typename scalarType>
	using Vec3 = Vec<scalarType, 3>;
	using vec3f = Vec3<float>;
	using vec3d = Vec3<double>;
	
	template<typename scalarType>
	using Vec4 = Vec<scalarType, 4>;
	using vec4f = Vec4<float>;
	using vec4d = Vec4<double>;
	
	//dot product any dimension
	template<typename scalarType, int dimension>
	Vec<scalarType, dimension> operator*(scalarType scalar, Vec<scalarType, dimension> const& vec)
	{
		return vec * scalar;
	}
	
	//corss product ^ 3d
	template<typename scalarType>
	Vec3<scalarType> operator^(Vec3<scalarType> const& lhs, Vec3<scalarType> const& rhs)
	{
		auto const& [x1, y1, z1]{ lhs };
		auto const& [x2, y2, z2]{ rhs };
		return Vec3<scalarType>{ y1 * z2 - z1 * y2, z1 * x2 - x1 * z2, x1 * y2 - y1 * x2 };
	}
	
	//scalar triple product
	
	//perp product ^ 2d
	template<typename scalarType>
	scalarType operator^(Vec2<scalarType> const& lhs, Vec2<scalarType> const& rhs)
	{
		auto const& [x1, y1]{ lhs };
		auto const& [x2, y2]{ rhs };
		return x1 * y2 - x2 * y1;
	}
	
}
#endif //FUNU_VEC_H
