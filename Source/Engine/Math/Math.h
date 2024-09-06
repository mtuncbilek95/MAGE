#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <concepts>

namespace Math
{
	template<typename T>
	concept Arithmetic = std::is_arithmetic_v<T>;

	template<Arithmetic T>
	using Vec2 = glm::tvec2<T, glm::aligned_highp>;

	template<Arithmetic T>
	using Vec3 = glm::tvec3<T, glm::aligned_highp>;

	template<Arithmetic T>
	using Vec4 = glm::tvec4<T, glm::aligned_highp>;

	template<Arithmetic T>
	using Mat2 = glm::tmat2x2<T, glm::aligned_highp>;

	template<Arithmetic T>
	using Mat3 = glm::tmat3x3<T, glm::aligned_highp>;

	template<Arithmetic T>
	using Mat4 = glm::tmat4x4<T, glm::aligned_highp>;

	using Vec2f = glm::tvec2<f32, glm::aligned_highp>;
	using Vec3f = glm::tvec3<f32, glm::aligned_highp>;
	using Vec4f = glm::tvec4<f32, glm::aligned_highp>;

	using Vec2d = glm::tvec2<f64, glm::aligned_highp>;
	using Vec3d = glm::tvec3<f64, glm::aligned_highp>;
	using Vec4d = glm::tvec4<f64, glm::aligned_highp>;

	using Vec2i = glm::tvec2<i32, glm::aligned_highp>;
	using Vec3i = glm::tvec3<i32, glm::aligned_highp>;
	using Vec4i = glm::tvec4<i32, glm::aligned_highp>;

	using Vec2u = glm::tvec2<u32, glm::aligned_highp>;
	using Vec3u = glm::tvec3<u32, glm::aligned_highp>;
	using Vec4u = glm::tvec4<u32, glm::aligned_highp>;

	using Vec2c = glm::tvec2<i8, glm::aligned_highp>;
	using Vec3c = glm::tvec3<i8, glm::aligned_highp>;
	using Vec4c = glm::tvec4<i8, glm::aligned_highp>;

	using Vec2s = glm::tvec2<i16, glm::aligned_highp>;
	using Vec3s = glm::tvec3<i16, glm::aligned_highp>;
	using Vec4s = glm::tvec4<i16, glm::aligned_highp>;

	using Mat2f = glm::tmat2x2<f32, glm::aligned_highp>;
	using Mat3f = glm::tmat3x3<f32, glm::aligned_highp>;
	using Mat4f = glm::tmat4x4<f32, glm::aligned_highp>;
}

