#ifndef VECTOR_H__
#define VECTOR_H__

#include "Presets.h"

namespace GDI {
	/// \brief Base class containing the information for a point/vector
	template <class PointDerivative> struct PointBase {
		virtual void Add(PointDerivative pd) = 0;

		virtual void Substract(PointDerivative pd) = 0;

		virtual void ScalarProduct(float sp) = 0;

		virtual void Multiply(PointDerivative pd) = 0;

		virtual bool Divide(PointDerivative pd) = 0;

		virtual float Norm() = 0;
	};

	/// \brief Class containing float x/y components.
	struct VectorInfo {
		float x, y;
	};

	/// \brief Class point.
	struct Point : public POINT, PointBase<POINT> {
		Point() {
			this->x = this->y = 0;
		}

		Point(int x, int y) {
			this->x = x;
			this->y = y;
		}

		inline void Add(POINT pd) {
			this->x += pd.x;
			this->y += pd.y;
		}

		inline void Substract(POINT pd) {
			this->x -= pd.x;
			this->y -= pd.y;
		}

		inline void Multiply(POINT pd) {
			this->x *= pd.x;
			this->y *= pd.y;
		}

		inline bool Divide(POINT pd) {
			if (pd.x && pd.y) {
				this->x /= pd.x;
				this->y /= pd.y;
				return true;
			}
			return false;
		}

		inline void ScalarProduct(float c) {
			this->x *= c;
			this->y *= c;
		}

		inline float Norm() {
			return sqrt(this->x * this->x + this->y * this->y);
		}

		template <class T> inline T& X() {
			return (T)x;
		}

		template <class T> inline T& Y() {
			return (T)y;
		}
	};

	/// \brief Class vector;
	struct Vector : public VectorInfo, PointBase<VectorInfo> {
		Vector() {
			this->x = this->y = 0;
		}

		Vector(int x, int y) {
			this->x = x;
			this->y = y;
		}

		inline void Add(VectorInfo pd) {
			this->x += pd.x;
			this->y += pd.y;
		}

		inline void Substract(VectorInfo pd) {
			this->x -= pd.x;
			this->y -= pd.y;
		}

		inline void Multiply(VectorInfo pd) {
			this->x *= pd.x;
			this->y *= pd.y;
		}

		inline bool Divide(VectorInfo pd) {
			if (pd.x && pd.y) {
				this->x /= pd.x;
				this->y /= pd.y;
				return true;
			}
			return false;
		}

		inline void ScalarProduct(float c) {
			this->x *= c;
			this->y *= c;
		}

		inline float Norm() {
			return sqrt(this->x * this->x + this->y * this->y);
		}

		template <class T> inline T& X() {
			return (T)x;
		}

		template <class T> inline T& Y() {
			return (T)y;
		}
	};
}
#endif
