#pragma once

namespace sqEngine::Math
{
	struct Vector2
	{
		union
		{
			struct { float x, y; };
			std::array<float, 2> v;
		};

		constexpr Vector2() noexcept : Vector2(0.0f) {}
	    constexpr Vector2(float f) noexcept : Vector2(f, f) {}
		constexpr Vector2(float x, float y)noexcept :x(x), y(y){}

		const static Vector2 Zero;
		const static Vector2 One;
		const static Vector2 XAxis;
		const static Vector2 YAxis;


		constexpr Vector2 operator-() const { return { -x,-y}; }
		constexpr Vector2 operator-(const Vector2& ref) const
		{
			Vector2 temp;
			temp.x = this->x - ref.x;
			temp.y = this->y - ref.y;
			return temp;
		}

		Vector2& operator-=(const Vector2& ref)
		{
			this->x = this->x - ref.x;
			this->y = this->y - ref.y;
			return *this;
		}
		Vector2& operator+=(const Vector2& ref)
		{
			this->x = this->x + ref.x;
			this->y = this->y + ref.y;
			return *this;
		}
		constexpr Vector2 operator+(const Vector2& ref) const
		{
			Vector2 temp;
			temp.x = this->x + ref.x;
			temp.y = this->y + ref.y;
			return temp;
		}
		constexpr Vector2 operator*(const Vector2& ref) const
		{
			Vector2 temp;
			temp.x = this->x * ref.x;
			temp.y = this->y * ref.y;
			return temp;
		}
		constexpr Vector2 operator*(const float& ref) const
		{
			Vector2 temp;
			temp.x = this->x * ref;
			temp.y = this->y * ref;
			return temp;
		}
		Vector2& operator/=(const float& ref)
		{
			if (ref != 0)
			{
				float tep = 1 / ref;
				this->x = this->x * tep;
				this->y = this->y * tep;
				return *this;
			}
			else
			{
				return *this;
			}
		}
		Vector2& operator*=(const float& ref)
		{
			this->x = this->x * ref;
			this->y = this->y * ref;
			return *this;
		}
	};
}
