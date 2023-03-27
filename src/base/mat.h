//
// Created by hhcoo on 2023/3/20.
//

#ifndef FUNU_SRC_BASE_MAT_H_
#define FUNU_SRC_BASE_MAT_H_

#include "vec.h"

namespace funu
{
	//matrix rowSize*colSize 不使用堆内存
	//行为主序 Mat[i][j]指的是第i行，第j列
	//代数运算对象不可变
	template<typename scalarType, int rowSize, int colSize>
	class Mat final
	{
		
		template<typename vtype, int r, int c>
		friend Vec<vtype, c> operator*(Vec<vtype, r> const&, Mat<vtype, r, c> const&);
	
	public:
		//static_assert(rowSize < 1 || colSize < 1);
		
		using DataType = std::array<Vec<scalarType, colSize>, rowSize>;
		
		Mat() : arrs_{}
		{
		}
		
		//输入数据同样行序为主序
		explicit Mat(DataType const& data) : arrs_{ data }
		{
		
		}
		
		//access methods
		Vec<scalarType, colSize> const& operator[](int index) const
		{
			return arrs_[index];
		}
		
		Vec<scalarType, colSize>& operator[](int index)
		{
			return arrs_[index];
		}
		
		//代数运算
		//矩阵乘以向量，矩阵列向量线性组合
		Vec<scalarType, rowSize> operator*(Vec<scalarType, colSize> const& rhs) const
		{
			Vec<scalarType, rowSize> res{};
			for (int i = 0; i < rowSize; ++i)
			{
				res[i] = arrs_[i] * rhs;
			}
			return res;
		}
		
		//矩阵乘以scalar
		Mat operator*(scalarType factor) const
		{
			Mat res{};
			for (int i = 0; i < rowSize; ++i)
			{
				res[i] = arrs_[i] * factor;
			}
			return res;
		}
		
		//矩阵与矩阵(列数大于2)相乘
		template<int rhsColSize>
		Mat<scalarType, rowSize, rhsColSize> operator*(Mat<scalarType, colSize, rhsColSize> const& rhs) const
		{
			static_assert(rhsColSize >= 2, "use operator*(Vec<scalarType, colSize> const&)!");
			Mat<scalarType, rowSize, rhsColSize> res{};
			for (int i = 0; i < rowSize; ++i)
			{
				res[i] = arrs_[i] * rhs;
			}
			return res;
		}
		
		//矩阵加和减
		Mat operator+(Mat const& rhs) const
		{
			Mat res{};
			for (int i = 0; i < rowSize; ++i)
			{
				res[i] = arrs_[i] + rhs[i];
			}
			return res;
		}
		Mat operator-(Mat const& rhs) const
		{
			Mat res{};
			for (int i = 0; i < rowSize; ++i)
			{
				res[i] = arrs_[i] - rhs[i];
			}
			return res;
		}
		
		//矩阵其他操作
		//矩阵转置
		Mat<scalarType, colSize, rowSize> transpose() const
		{
			Mat<scalarType, colSize, rowSize> transposedMat;
			for (int i = 0; i < colSize; ++i)
			{
				for (int j = 0; j < rowSize; ++j)
				{
					transposedMat[i][j] = arrs_[j][i];
				}
			}
			return transposedMat;
		}
		
		//通过cofactor expansion计算 要求为方阵
		scalarType determinant() const;
		//通过伴随矩阵计算 要求为方阵
		Mat inverse() const;
	
	private:
		
		//提取子矩阵，会不会导致重载失败？
		template<int subRowSize, int subColSize>
		void extractSubMat(Mat<scalarType, subRowSize, subColSize>& subMat, int rowStart,
			int colStart,
			int rowIgnore = -1,
			int colIgnore = -1) const
		{
			int targetRow{ rowStart };
			for (int i = 0; i < subRowSize; ++i, ++targetRow)
			{
				if (targetRow == rowIgnore)
				{
					--i;
					continue;
				}
				int targetCol = colStart;
				for (int j = 0; j < subColSize; ++j, ++targetCol)
				{
					if (targetCol != colIgnore)
					{
						subMat[i][j] = arrs_[targetRow][targetCol];
					}
					else
					{
						--j;
					}
				}
			}
		}
	
	private:
		DataType arrs_;
	};
	
	template<typename vtype, int r, int c>
	Vec<vtype, c> operator*(Vec<vtype, r> const& v, Mat<vtype, r, c> const& mat)
	{
		Vec<vtype, c> res{};
		for (int i = 0; i < r; ++i)
		{
			res = res + mat[i] * v[i];
		}
		return res;
	}
	
	template<typename scalarType, int rowSize, int colSize>
	Mat<scalarType, rowSize, colSize> operator*(scalarType factor, Mat<scalarType, rowSize, colSize> const& rhs)
	{
		return rhs * factor;
	}
	
	template<typename scalarType, int rowSize, int colSize>
	scalarType Mat<scalarType, rowSize, colSize>::determinant() const
	{
		if (rowSize == 1)
		{
			return arrs_[0][0];
		}
		else if (rowSize == 2)
		{
			return arrs_[1][1] * arrs_[0][0] - arrs_[0][1] * arrs_[1][0];
		}
		else if (rowSize == 3)
		{
			return
				arrs_[0][0] * arrs_[1][1] * arrs_[2][2] + arrs_[0][1] * arrs_[1][2] * arrs_[2][0]
					+ arrs_[0][2] * arrs_[1][0] * arrs_[2][1] -
					arrs_[0][0] * arrs_[1][2] * arrs_[2][1] - arrs_[0][1] * arrs_[1][0] * arrs_[2][2]
					- arrs_[0][2] * arrs_[1][1] * arrs_[2][0];
		}
		else if (rowSize == 4)
		{
			//cofactor expansion along the first row
			return
				arrs_[0][0] * (
					arrs_[1][1] * arrs_[2][2] * arrs_[3][3] + arrs_[1][2] * arrs_[2][3] * arrs_[3][1]
						+ arrs_[1][3] * arrs_[2][1] * arrs_[3][2] -
						arrs_[1][1] * arrs_[2][3] * arrs_[3][2] - arrs_[1][2] * arrs_[2][1] * arrs_[3][3]
						- arrs_[1][3] * arrs_[2][2] * arrs_[3][1]
				)
					- arrs_[0][1] * (
						arrs_[1][0] * arrs_[2][2] * arrs_[3][3] + arrs_[1][2] * arrs_[2][3] * arrs_[3][0]
							+ arrs_[1][3] * arrs_[2][0] * arrs_[3][2] -
							arrs_[1][0] * arrs_[2][3] * arrs_[3][2] - arrs_[1][2] * arrs_[2][0] * arrs_[3][3]
							- arrs_[1][3] * arrs_[2][2] * arrs_[3][0]
					)
					+ arrs_[0][2] * (
						arrs_[1][0] * arrs_[2][1] * arrs_[3][3] + arrs_[1][1] * arrs_[2][3] * arrs_[3][0]
							+ arrs_[1][3] * arrs_[2][0] * arrs_[3][1] -
							arrs_[1][0] * arrs_[2][3] * arrs_[3][1] - arrs_[1][1] * arrs_[2][0] * arrs_[3][3]
							- arrs_[1][3] * arrs_[2][1] * arrs_[3][0]
					)
					- arrs_[0][3] * (
						arrs_[1][0] * arrs_[2][1] * arrs_[3][2] + arrs_[1][1] * arrs_[2][2] * arrs_[3][0]
							+ arrs_[1][2] * arrs_[2][0] * arrs_[3][1] -
							arrs_[1][0] * arrs_[2][2] * arrs_[3][1] - arrs_[1][1] * arrs_[2][0] * arrs_[3][2]
							- arrs_[1][2] * arrs_[2][1] * arrs_[3][0]
					);
		}
		else if (rowSize >= 5)
		{
			///cofactor expansion along the first row
			int factor{};
			scalarType sum{};
			//默认第一行展开，第k项
			for (int k = 0; k < rowSize; ++k)
			{
				Mat<scalarType, rowSize - 1, colSize - 1> minorMat;
				extractSubMat(minorMat, 1, 0, 0, k);
				factor ^= 1;
				sum = sum + ((factor << 1) - 1) * arrs_[0][k] * minorMat.determinant();
			}
			return sum;
		}
	}
	
	template<typename scalarType, int rowSize, int colSize>
	Mat<scalarType, rowSize, colSize> Mat<scalarType, rowSize, colSize>::inverse() const
	{
		Mat<scalarType, rowSize, rowSize> res{};
		scalarType determinateInverse{ static_cast<scalarType>(1) / this->determinant() };
		if (rowSize == 1)
		{
			res[0][0] = static_cast<scalarType>(1) / arrs_[0][0];
		}
		else if (rowSize == 2)
		{
			res[0][0] = determinateInverse * arrs_[1][1];
			res[0][1] = -determinateInverse * arrs_[0][1];
			res[1][0] = -determinateInverse * arrs_[1][0];
			res[1][1] = determinateInverse * arrs_[0][0];
		}
		else if (rowSize >= 3)
		{
			//伴随矩阵adjMat
			auto& adjMat{ res };
			//第i行，第j列，相对于伴随矩阵
			for (int i = 0; i < rowSize; ++i)
			{
				for (int j = 0; j < colSize; ++j)
				{
					//[j,i]的minor
					Mat<scalarType, rowSize - 1, colSize - 1> minorMat;
					extractSubMat(minorMat, 0, 0, j, i);
					int factor{ (j + i) % 2 == 0 ? (1) : (-1) };
					adjMat[i][j] = determinateInverse * factor * minorMat.determinant();
				}
			}
		}
		return res;
	}
	
	using Matrix33f = Mat<float, 3, 3>;
	using Matrix33d = Mat<double, 3, 3>;
	using Matrix44f = Mat<float, 4, 4>;
	using Matrix44d = Mat<double, 4, 4>;
}

#endif //FUNU_SRC_BASE_MAT_H_
