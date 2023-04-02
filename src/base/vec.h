//
// Created by hhcoo on 2023/2/28.
//

#ifndef FUNU_VEC_H
#define FUNU_VEC_H

#include <array>
#include <cmath>
#include <algorithm>
#include <numeric>

namespace funu
{
	//向量
	//代数运算对象不可变
	template<typename scalarType, int dimension>
	class Vec final
	{
	public:
		explicit Vec(scalarType args...) : arr_{ std::forward<scalarType>(args) }
		{
		}
		
		//构造0向量
		Vec() : arr_{}
		{
		}
		
		//access
		scalarType const& operator[](int index) const
		{
			return arr_[index];
		}
		
		scalarType& operator[](int index)
		{
			return arr_[index];
		}
		
		//代数运算
		Vec operator-(Vec const& rhs) const
		{
			Vec res{};
			for (int i = 0; i < dimension; ++i)
			{
				res[i] = arr_[i] - rhs[i];
			}
			return res;
		}
		
		Vec operator+(Vec const& rhs) const
		{
			Vec res{};
			for (int i = 0; i < dimension; ++i)
			{
				res[i] = arr_[i] + rhs[i];
			}
			return res;
		}
		
		scalarType operator*(Vec const& rhs) const
		{
			scalarType sum{};
			for (int i = 0; i < dimension; ++i)
			{
				sum = sum + arr_[i] * rhs[i];
			}
			return sum;
		}
		
		Vec operator*(scalarType factor) const
		{
			Vec res{};
			for (int i = 0; i < dimension; ++i)
			{
				res[i] = arr_[i] * factor;
			}
			return res;
		}
		
		//negative operator
		Vec operator-(int) const
		{
			Vec res{};
			for (int i = 0; i < dimension; ++i)
			{
				res[i] = -arr_[i];
			}
			return res;
		}
		
		//似乎是无用函数
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
			return static_cast<scalarType>(std::sqrt(squared_norm()));
		}
		
		Vec normalize() const
		{
			//不检查0向量
			return operator*(static_cast<scalarType>(1) / norm());
		}
	
	private:
		std::array<scalarType, dimension> arr_;
	};
	
	template<typename scalarType>
	using Vec2 = Vec<scalarType, 2>;
	using Vec2f = Vec2<float>;
	using Vec2d = Vec2<double>;
	
	template<typename scalarType>
	using Vec3 = Vec<scalarType, 3>;
	using Vec3f = Vec3<float>;
	using Vec3d = Vec3<double>;
	
	template<typename scalarType>
	using Vec4 = Vec<scalarType, 4>;
	using Vec4f = Vec4<float>;
	using Vec4d = Vec4<double>;
	
	//dot product any dimension
	template<typename scalarType, int dimension>
	Vec<scalarType, dimension> operator*(scalarType scalar, Vec<scalarType, dimension> const& vec)
	{
		return vec * scalar;
	}
	
	//corss product ^ in 3d
	template<typename scalarType>
	Vec3<scalarType> operator^(Vec3<scalarType> const& lhs, Vec3<scalarType> const& rhs)
	{
		auto const& [x1, y1, z1]{ lhs };
		auto const& [x2, y2, z2]{ rhs };
		return Vec3<scalarType>{ y1 * z2 - z1 * y2, z1 * x2 - x1 * z2, x1 * y2 - y1 * x2 };
	}
	
	//scalar triple product 可计算六面体有向体积，判断4点是否共面，使用矩阵特征值完成
	
	//perp product ^ in 2d  向量拐向判断、平面三角形面积
	template<typename scalarType>
	scalarType operator^(Vec2<scalarType> const& lhs, Vec2<scalarType> const& rhs)
	{
		auto const& [x1, y1]{ lhs };
		auto const& [x2, y2]{ rhs };
		return x1 * y2 - x2 * y1;
	}
}
#endif //FUNU_VEC_H
