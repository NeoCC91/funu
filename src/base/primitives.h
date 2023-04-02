//
// Created by hhcoo on 2023/4/2.
//

#ifndef FUNU_SRC_BASE_PRIMITIVES_H_
#define FUNU_SRC_BASE_PRIMITIVES_H_

#include "vec.h"

namespace funu
{
	
	template<typename scalarType>
	class line2{
	
		
	};
	
	template<typename scalarType>
	class line3
	{
	public:
		line3(Vec3<scalarType> const& ori,Vec3<scalarType> const& dir):origin_{ori},dir_{dir}
		{
		}
	private:
		Vec3<scalarType> origin_;
		Vec3<scalarType> dir_;
	};
	
	using line3f=line3<float>;
	using line3d=line3<double>;
	
	template<typename scalarType>
	class Plane
	{
	public:
		//ax+by+cz+d=0
		Plane(scalarType a,scalarType b,scalarType c,scalarType d):normal_{a,b,c}
		{
			auto factor{static_cast<scalarType>(1)/normal_.norm()};
			normal_=normal_*factor;
			d_=d*factor;
		}
		
		//点和法向
		Plane(Vec3<scalarType> const& ori,Vec3<scalarType> const& n):origin_{ori},normal_{n.normalize()}
		{
			d_=-origin_*normal_;
		}
		
		//点到面的有向距离
		scalarType pntDis(Vec3<scalarType> const& pnt)
		{
			return pnt*normal_+d_;
		}
		
		//线与面交点
		
		
	private:
		Vec3<scalarType> origin_;
		// a,b,c
		Vec3<scalarType> normal_;
		scalarType d_;
	};
	
	using Planef=Plane<float>;
	using Planed=Plane<double>;
}
#endif //FUNU_SRC_BASE_PRIMITIVES_H_