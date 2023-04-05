//
// Created by hhcoo on 2023/4/2.
//

#ifndef FUNU_SRC_BASE_PRIMITIVES_H_
#define FUNU_SRC_BASE_PRIMITIVES_H_

#include "vec.h"

namespace funu
{
	
	//平面直线
	template<typename scalarType>
	class Line2{
	public:
		//隐式方程 ax+by+c=0
		Line2(scalarType a,scalarType b,scalarType c): dir_{ -b, a}
		{
			auto factor{static_cast<scalarType>(1)/dir_.norm()};
			dir_=dir_*factor;
			c_=c*factor;
			origin_=-c_*dir_;
		}
		
		//点和方向
		Line2(Vec2<scalarType> const& ori,Vec2<scalarType> const& dir): origin_{ ori}, dir_{ dir.normalize()}{
			c_=dir_^origin_;
		}
		
		//获取线上的点
		Vec2<scalarType> const& pointOn() const{
			return origin_;
		}
		//获取线方向，单位向量
		Vec2<scalarType> const& dir() const{
			return dir_;
		}
		//获取线法向(a,b)
		Vec2<scalarType> const& normal() const{
			return {dir_[1],-dir_[0]};
		}
		
		//点到线的有向距离
		scalarType pntDis(Vec2<scalarType> const& pnt) const
		{
			return pnt^dir_+c_;
		}
		
	private:
		//线上点
		Vec2<scalarType> origin_;
		//单位向量，方向。-b,a
		Vec2<scalarType> dir_;
		scalarType c_;
	};
	
	using Line2f=Line2<float>;
	using Line2d=Line2<double>;
	
	//3维空间直线
	template<typename scalarType>
	class Line3
	{
	public:
		Line3(Vec3<scalarType> const& ori,Vec3<scalarType> const& dir): origin_{ ori}, dir_{ dir}
		{
		}
	private:
		//线上点
		Vec3<scalarType> origin_;
		//单位向量，方向
		Vec3<scalarType> dir_;
	};
	
	using Line3f=Line3<float>;
	using Line3d=Line3<double>;
	
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
			origin_=-d_*normal_;
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
		
		//线、射线、线段与面交点
		//面与面交线
		
		
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