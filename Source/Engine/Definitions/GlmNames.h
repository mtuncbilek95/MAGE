/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Definitions.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <concepts>
#include <type_traits>

namespace Mage
{
	namespace Math
	{
		template<typename T>
			requires std::is_arithmetic_v<T>
		using Vec2 = glm::tvec2<T>;

		template<typename T>
			requires std::is_arithmetic_v<T>
		using Vec3 = glm::tvec3<T>;

		template<typename T>
			requires std::is_arithmetic_v<T>
		using Vec4 = glm::tvec4<T>;

		template<typename T>
			requires std::is_arithmetic_v<T>
		using Mat2 = glm::tmat2x2<T>;

		template<typename T>
			requires std::is_arithmetic_v<T>
		using Mat3 = glm::tmat3x3<T>;

		template<typename T>
			requires std::is_arithmetic_v<T>
		using Mat4 = glm::tmat4x4<T>;

		using Vec2uc = Vec2<u8>;
		using Vec3uc = Vec3<u8>;
		using Vec4uc = Vec4<u8>;

		using Vec2f = Vec2<f32>;
		using Vec3f = Vec3<f32>;
		using Vec4f = Vec4<f32>;

		using Vec2d = Vec2<f64>;
		using Vec3d = Vec3<f64>;
		using Vec4d = Vec4<f64>;

		using Vec2i = Vec2<i32>;
		using Vec3i = Vec3<i32>;
		using Vec4i = Vec4<i32>;

		using Vec2u = Vec2<u32>;
		using Vec3u = Vec3<u32>;
		using Vec4u = Vec4<u32>;

		using Mat2f = Mat2<f32>;
		using Mat3f = Mat3<f32>;
		using Mat4f = Mat4<f32>;

		using Mat2d = Mat2<f64>;
		using Mat3d = Mat3<f64>;
		using Mat4d = Mat4<f64>;
	}
}
