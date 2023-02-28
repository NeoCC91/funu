//
// Created by hhcoo on 2023/2/28.
//

#ifndef FUNU_VEC_H
#define FUNU_VEC_H

#include <array>
#include <cmath>
#include <algorithm>

namespace funu
{
	template<typename scalarType, size_t dimension>
	class Vec
	{
	public:
		explicit Vec(scalarType args...) : data_{ std::forward<scalarType>(args) }
		{
		
		}
		Vec() : data_{}
		{
		}
		
		scalarType norm() const
		{
			scalarType const squared_sum{};
			for (size_t i = 0; i < dimension; ++i)
			{
				squared_sum += data_[i] * data_[i];
			}
			return std::sqrt(squared_sum);
		}
		
		void normalized()
		{
			auto const copy{ normalize() };
			std::copy(copy.data_.begin(), copy.data_.end(), data_.begin());
		}
		Vec normalize() const
		{
			auto copy{ *this };
			auto const factor{ 1 / copy.norm() };
			std::transform(copy.data_.begin(), copy.data_.end(), [factor](auto const val)
			{
				return val * factor;
			});
			return copy;
		}
		
		Vec operator-(Vec const& ele) const
		{
			Vec res{};
			for (size_t i = 0; i < dimension; ++i)
			{
				res[i] = data_[i] - ele[i];
			}
			return res;
		}
		
		void operator-(int)
		{
			for (size_t i = 0; i < dimension; ++i)
			{
				data_[i] = -data_[i];
			}
		}
		
		Vec operator+(Vec const& ele) const
		{
			Vec res{};
			for (size_t i = 0; i < dimension; ++i)
			{
				res[i] = data_[i] + ele[i];
			}
			return res;
		}
		
		~Vec() = default;
	
	public:
		std::array<scalarType, dimension> data_;
	};
	
	using vec3f = Vec<float, 3>;
	using vec3d = Vec<double, 3>;
	
}
#endif //FUNU_VEC_H
