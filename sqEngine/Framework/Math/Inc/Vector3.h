#pragma once

namespace sqEngine::Math
{
	struct Vector3
	{
		union
		{
			struct { float x, y, z; };
			std::array<float, 3> v;
		};
		constexpr Vector3() noexcept : Vector3(0.0f) {}
		explicit constexpr Vector3(float f) noexcept : Vector3(f, f, f) {}
		constexpr Vector3(float x, float y, float z)noexcept :x(x), y(y), z(z){}

		const static Vector3 Zero;
		const static Vector3 One;
		const static Vector3 XAxis;
		const static Vector3 YAxis;
		const static Vector3 ZAxis;

		constexpr Vector3 operator-() const { return { -x,-y,-z }; }
		constexpr Vector3 operator-(const Vector3& ref) const
		{
			Vector3 temp;
			temp.x = this->x - ref.x;
			temp.y = this->y - ref.y;
			temp.z = this->z - ref.z;
			return temp;
		}

		Vector3& operator-=(const Vector3& ref)
		{
			this->x = this->x - ref.x;
			this->y = this->y - ref.y;
			this->z = this->z - ref.z;
			return *this;
		}
		Vector3& operator+=(const Vector3 & ref)
		{ 
			this->x = this->x + ref.x;
			this->y = this->y + ref.y;
			this->z = this->z + ref.z;
			return *this;
		}
		constexpr Vector3 operator+(const Vector3& ref) const
		{
			Vector3 temp;
			temp.x = this->x + ref.x;
			temp.y = this->y + ref.y;
			temp.z = this->z + ref.z;
			return temp;
		}
		constexpr Vector3 operator*(const Vector3& ref) const
		{
			Vector3 temp;
			temp.x = this->x * ref.x;
			temp.y = this->y * ref.y;
			temp.z = this->z * ref.z;
			return temp;
		}
		constexpr Vector3 operator*(const float& ref) const
		{
			Vector3 temp;
			temp.x = this->x * ref;
			temp.y = this->y * ref;
			temp.z = this->z * ref;
			return temp;
		}
		Vector3& operator/=(const float& ref)
		{
			if (ref != 0)
			{
				float tep = 1 / ref;
				this->x = this->x * tep;
				this->y = this->y * tep;
				this->z = this->z * tep;
				return *this;
			}
			else
			{
				return * this;
			}
		}
		Vector3& operator*=(const float& ref)
		{
			this->x = this->x * ref;
			this->y = this->y * ref;
			this->z = this->z * ref;
			return *this;
		}
	};

	constexpr float Dot(const Vector3& ref, const Vector3& ref2)
	{
		float temp = ref.x* ref2.x + ref.y * ref2.y + ref.z * ref2.z;
		return temp;
	}

	constexpr float MagnitudeSqr(const Vector3& ref)
	{
		float temp = ref.x * ref.x + ref.y * ref.y + ref.z * ref.z;
		return temp;
	}

	inline float Magnitude(const Vector3& ref)
	{
		float temp = sqrt(MagnitudeSqr(ref));
		return temp;
	}

	constexpr float DistanceSqr(const Vector3& ref,const Vector3& ref2)
	{
		return (ref.x - ref2.x) * (ref.x - ref2.x) + (ref.y - ref2.y) *(ref.y - ref2.y) + (ref.z - ref2.z) * (ref.z - ref2.z);
	}

	inline float Distance(const Vector3& ref, const Vector3& ref2)
	{
		float temp = DistanceSqr(ref, ref2);
		return sqrt(temp);
	}

	inline Vector3 Normalize(const Vector3& ref)
	{
		Vector3 tempref = ref;
		Vector3 tempv;
		float temp = Magnitude(ref);
		temp = 1 / temp;
		if (temp != 0)
		{
			tempv = tempref * temp;
		}
		else
		{
			Vector3 v{ 0,0,0 };
			return  v;
		}	
		return tempv;
	}

	constexpr Vector3 Cross(const Vector3& ref, const Vector3& ref2)
	{
		Vector3 temp{ ref.y * ref2.z - ref.z * ref2.y,
					  ref.z * ref2.x - ref.x * ref2.z,
					  ref.x * ref2.y - ref.y * ref2.x };
		return temp;
	}
}
	
	
