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
	public:
		//隐式方程 ax+by+c=0
		line2(scalarType a,scalarType b,scalarType c):dir_{-b,a}
		{
			auto factor{static_cast<scalarType>(1)/dir_.norm()};
			dir_=dir_*factor;
			c_=c*factor;
		}
		
		//点和方向
		line2(Vec2<scalarType> const& ori,Vec2<scalarType> const& dir):origin_{ori},dir_{dir.normalize()}{
		
		}
		
		
	private:
		//线上点
		Vec2<scalarType> origin_;
		//单位向量，方向。-b,a
		Vec2<scalarType> dir_;
		scalarType c_;
	};
	
	template<typename scalarType>
	class line3
	{
	public:
		line3(Vec3<scalarType> const& ori,Vec3<scalarType> const& dir):origin_{ori},dir_{dir}
		{
		}
	private:
		//线上点
		Vec3<scalarType> origin_;
		//单位向量，方向
		Vec3<scalarType> dir_;
	};
	
	using line3f=line3<float>;
	using line3d=line3<double>;
	
	template<typename scalarType>
	class Plane
	{
	public:
		//隐式方程 ax+by+cz+d=0
		Plane(scalarType a,scalarType b,scalarType c,scalarType d):normal_{a,b,c}
		{
			auto factor{static_cast<scalarType>(1)/normal_.norm()};
			normal_=normal_*factor;
			d_=d*factor;
		}
		
		//点和法向
		Plane(Vec3<scalarType> const& v,Vec3<scalarType> const& normal):origin_{v},normal_{normal.normalize()}
		{
			d_=-origin_*normal_;
		}
		
		//三个点
		Plane(Vec3<scalarType> const& v0,Vec3<scalarType> const& v1,Vec3<scalarType> const& v2):
		Plane(v0,(v1-v0)^(v2-v0))
		{
		
		}
		
		//获取平面上的点
		Vec3<scalarType> const& pointOn() const{
			return origin_;
		}
		//获取平面法向，单位向量
		Vec3<scalarType> const& normal() const{
			return normal_;
		}
		
		//点到面的有向距离
		scalarType pntDis(Vec3<scalarType> const& pnt) const
		{
			return pnt*normal_+d_;
		}
		
		//线与面交点
		
		
	private:
		//面内点
		Vec3<scalarType> origin_;
		//单位向量，法向。a,b,c
		Vec3<scalarType> normal_;
		scalarType d_;
	};
	
	using Planef=Plane<float>;
	using Planed=Plane<double>;
}
#endif //FUNU_SRC_BASE_PRIMITIVES_H_