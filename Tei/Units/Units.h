#pragma once

#include "../Libs/Glm.h"

namespace tei::internal::units
{

#define Unique_t tei::internal::units::Unique_t_<__LINE__>::type

	template <unsigned>
	struct Unique_t_
	{
		template <typename T>
		struct type : T
		{
			using Base = T;
			using T::T;
		};
	};

	using Unit = float;

	using Vec1              = glm::tvec1<Unit>;
	using Vec2              = glm::vec2;
	using Vec3              = glm::vec3;

	using Mat2              = glm::mat2;
	using Mat3              = glm::mat3;

	using Position          = Unique_t<Vec2>;
	using Rotation          = Unique_t<Vec1>;
	using UnityScale        = Unique_t<Vec1>;
	using Scale             = Unique_t<Vec2>;

	using RotationMatrix    = Unique_t<Mat2>;
	using ScaleMatrix       = Unique_t<Mat2>;
	using TranslationMatrix = Unique_t<Mat3>;
	using TransformMatrix   = Unique_t<Mat3>;

	using Dimentions        = Unique_t<Vec2>;
	using Rectangle         = Unique_t<Mat2>;

	using Colour            = Unique_t<Vec3>;

	struct Transform
	{
		Position position{};
		Scale    scale   {};
		Rotation rotation{};

		Transform(Position          const& = {},
				  Scale             const& = { 1, 1 },
				  Rotation          const& = {}) noexcept;

		Transform(Scale             const&) noexcept;
		Transform(Rotation          const&) noexcept;

		Transform(RotationMatrix    const&) noexcept;
		Transform(ScaleMatrix       const&) noexcept;
		Transform(TranslationMatrix const&) noexcept;
		Transform(TransformMatrix   const&) noexcept;

		operator RotationMatrix   () const noexcept;
		operator ScaleMatrix      () const noexcept;
		operator TranslationMatrix() const noexcept;
		operator TransformMatrix  () const noexcept;
	};

}

namespace tei::internal::units
{

	inline Transform::Transform(Position const& t, Scale const& s, Rotation const& r) noexcept
		: position{ t }
		, scale{ s }
		, rotation{ r }
	{}

	inline Transform::Transform(Scale const& s) noexcept
		: Transform{ {}, s, {} }
	{}

	inline Transform::Transform(Rotation const& r) noexcept
		: Transform{ {}, {}, r }
	{}

	inline Transform::Transform(RotationMatrix const& m) noexcept
		: rotation{ atan2(m[0].y, m[0].x) }
	{}

	inline Transform::Transform(ScaleMatrix const& m) noexcept
		: scale{ glm::length(m[0]) }
	{}

	inline Transform::Transform(TranslationMatrix const& m) noexcept
		: position{ m[2] }
	{}

	inline Transform::Transform(TransformMatrix const& m) noexcept
		: position{ m[2] }
		, scale{ glm::length(m[0]), glm::length(m[1]) }
		, rotation{ atan2(m[0].y / scale.x, m[0].x / scale.x) }
	{}

	inline Transform::operator RotationMatrix() const noexcept
	{
		auto const
			x{ cos(rotation) },
			y{ sin(rotation) };
		return RotationMatrix{
			{  x, y }, 
			{ -y, x }
		};
	}

	inline Transform::operator ScaleMatrix() const noexcept
	{
		return ScaleMatrix{
			{ scale.x, 0 },
			{ 0, scale.x }
		};
	}

	inline Transform::operator TranslationMatrix() const noexcept
	{
		return TranslationMatrix{
			{ },
			{ },
			{ position, 0 }
		};
	}

	inline Transform::operator TransformMatrix() const noexcept
	{
		return TransformMatrix{ ScaleMatrix(*this) * RotationMatrix(*this) } * TranslationMatrix(*this);
	}

}