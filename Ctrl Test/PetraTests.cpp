#include "tests.h"

#include <iostream>
#include <string>
#include <unordered_map>

#include "Core/Monument/Monument.h"
#include "Core/Petra/rqm.h"

template<typename VecT>
struct VecTestInputs
{
	VecT v1;
	VecT v2;
	VecT vSum;
	VecT v12Diff; // v2 - v1
	VecT vHProd;
	VecT v12Div; // v1 / v2
	VecT v21Div; // v2 / v1
	VecT v1norm;
	VecT v2norm;
	VecT v12norm;
	VecT v21norm;
	float v1magsqr;
	float v2magsqr;
	float v12magsqr;
	float v21magsqr;
	float v1mag;
	float v2mag;
	float v12mag;
	float v21mag;
	float v12Dot;
	float scalar;
	VecT v1ScalarSum;
	VecT v1ScalarDiff; // scalar - v1
	VecT scalarV1Diff; // v1 - scalar
	VecT v1ScalarProd;
	VecT v1ScalarDiv; // v1 / scalar
	VecT scalarV1Div; // scalar / v1
	VecT v12Min;
	VecT v12Max;
};

// modifies the inputs
template<typename VecT>
void testVecGeneral(const VecTestInputs<VecT>& in)
{
	std::cout << "===================================\nTesting " << in.v1 << " and " << in.v2 << '\n';

	assert(in.v1 + in.v2 == in.vSum);
	VecT v12 = in.v2 - in.v1;
	VecT v21 = in.v1 - in.v2;
	assert(v21 == -in.v12Diff);
	assert(v12 == in.v12Diff);
	assert(rqm::hadamard_product(in.v1, in.v2) == in.vHProd);
	assert(rqm::hadamard_division(in.v1, in.v2) == in.v12Div);
	assert(rqm::hadamard_division(in.v2, in.v1) == in.v21Div);

	VecT temp = in.v1;
	temp += in.v2;
	assert(temp == in.vSum);
	temp = in.v1;
	temp -= in.v2;
	assert(temp == -in.v12Diff);

	assert(in.v1.magnitude_squared() == in.v1magsqr);
	assert(in.v2.magnitude_squared() == in.v2magsqr);
	assert(v12.magnitude_squared() == in.v12magsqr);
	assert(v21.magnitude_squared() == in.v21magsqr);

	assert(in.v1.magnitude() == in.v1mag);
	assert(in.v2.magnitude() == in.v2mag);
	assert(v12.normalize() == in.v12mag);
	assert(v21.normalize() == in.v21mag);

	VecT v1Copy = in.v1;
	v1Copy.normalize_no_return();
	VecT v2Copy = in.v2;
	v2Copy.normalize_no_return();
	assert(v1Copy == in.v1norm);
	assert(v2Copy == in.v2norm);
	assert(v12 == in.v12norm);
	assert(v21 == in.v21norm);

	assert(rqm::dot(in.v1, in.v2) == in.v12Dot);

	assert(in.v1 + in.scalar == in.v1ScalarSum);
	assert(in.scalar + in.v1 == in.v1ScalarSum);
	assert(in.scalar - in.v1 == in.v1ScalarDiff);
	assert(in.v1 - in.scalar == in.scalarV1Diff);
	assert(in.v1 * in.scalar == in.v1ScalarProd);
	assert(in.scalar * in.v1 == in.v1ScalarProd);
	assert(in.v1 / in.scalar == in.v1ScalarDiv);
	assert(in.scalar / in.v1 == in.scalarV1Div);

	temp = in.v1;
	temp += in.scalar;
	assert(temp == in.v1ScalarSum);
	temp = in.v1;
	temp -= in.scalar;
	assert(temp == in.scalarV1Diff);
	temp = in.v1;
	temp *= in.scalar;
	assert(temp == in.v1ScalarProd);
	temp = in.v1;
	temp /= in.scalar;
	assert(temp == in.v1ScalarDiv);

	assert(rqm::min(in.v1, in.v2) == in.v12Min);
	assert(rqm::max(in.v1, in.v2) == in.v12Max);
	rqm::min_in_place(in.v1, in.v2, temp);
	assert(temp == in.v12Min);
	rqm::max_in_place(in.v1, in.v2, temp);
	assert(temp == in.v12Max);

	std::cout << std::endl;
}

void testVec1()
{
	// TODO if we ever decide to not just use a typedef for vec1
}

void testVec2()
{
	float v1ms = 5.0f * 5.0f + 3.1f * 3.1f;
	float v2ms = 2.5f * 2.5f + (-0.2f * -0.2f);
	float v12ms = (-2.5f * -2.5f) + (-3.3f * -3.3f);
	float v21ms = (2.5f * 2.5f) + (3.3f * 3.3f);
	float v1m = rqm::sqrt(v1ms);
	float v2m = rqm::sqrt(v2ms);
	float v12m = rqm::sqrt(v12ms);
	float v21m = rqm::sqrt(v21ms);
	VecTestInputs<rqm::vec2> args =
	{
		{ 5.0f, 3.1f }, // v1
		{ 2.5f, -0.2f }, // v2
		{ 5.0f + 2.5f, 3.1f - 0.2f }, // sum
		{ -2.5f, -3.3f }, // v2 - v1
		{ 5.0f * 2.5f, -0.2f * 3.1f }, // prod
		{ 2.0f, 3.1f / -0.2f }, // v1 / v2
		{ 0.5f, -0.2f / 3.1f }, // v2 / v1
		{ 5.0f / v1m, 3.1f / v1m }, // v1norm
		{ 2.5f / v2m, -0.2f / v2m }, // v2norm
		{ -2.5f / v12m, -3.3f / v12m }, // (v2 - v1)norm
		{ 2.5f / v21m, 3.3f / v21m }, // (v1 - v2)norm
		v1ms,
		v2ms,
		v12ms,
		v21ms,
		v1m,
		v2m,
		v12m,
		v21m,
		5.0f * 2.5f + (3.1f * -0.2f), // dot
		6.3f, // scalar
		{ 5.0f + 6.3f, 3.1f + 6.3f }, // scalar + v1
		{ 6.3f - 5.0f, 6.3f - 3.1f }, // scalar - v1
		{ 5.0f - 6.3f, 3.1f - 6.3f }, // v1 - scalar
		{ 5.0f * 6.3f, 3.1f * 6.3f }, // scalar * v1
		{ 5.0f / 6.3f, 3.1f / 6.3f }, // v1 / scalar
		{ 6.3f / 5.0f, 6.3f / 3.1f }, // scalar / v1
		{ 2.5f, -0.2f }, // min(v1, v2)
		{ 5.0f, 3.1f } // max(v1, v2)
	};
	testVecGeneral(args);
	rqm::vec2 indTest = { 1.0f, 2.0f };
	assert(indTest[0] == indTest.x);
	assert(indTest[1] == indTest.y);
}

void testVec3()
{
	float v1x = -1.5f; float v1y = 7.4f; float v1z = -5.6f;
	float v2x = 3.2f; float v2y = 4.7f; float v2z = -0.3f;
	float v12dx = v2x - v1x; float v12dy = v2y - v1y; float v12dz = v2z - v1z;
	float v1ms = v1x * v1x + v1y * v1y + v1z * v1z;
	float v2ms = v2x * v2x + v2y * v2y + v2z * v2z;
	float v12ms = v12dx * v12dx + v12dy * v12dy + v12dz * v12dz;
	float v21ms = (-v12dx * -v12dx) + (-v12dy * -v12dy) + (-v12dz * -v12dz);
	float v1m = rqm::sqrt(v1ms);
	float v2m = rqm::sqrt(v2ms);
	float v12m = rqm::sqrt(v12ms);
	float v21m = rqm::sqrt(v21ms);
	float s = -12.3f;
	VecTestInputs<rqm::vec3> args =
	{
		{ v1x, v1y, v1z }, // v1
		{ v2x, v2y, v2z }, // v2
		{ v1x + v2x, v1y + v2y, v1z + v2z }, // sum
		{ v12dx, v12dy, v12dz }, // v2 - v1
		{ v1x * v2x, v1y * v2y, v1z * v2z }, // prod
		{ v1x / v2x, v1y / v2y, v1z / v2z }, // v1 / v2
		{ v2x / v1x, v2y / v1y, v2z / v1z }, // v2 / v1
		{ v1x / v1m, v1y / v1m, v1z / v1m }, // v1norm
		{ v2x / v2m, v2y / v2m, v2z / v2m }, // v2norm
		{ v12dx / v12m, v12dy / v12m, v12dz / v12m }, // (v2 - v1)norm
		{ -v12dx / v21m, -v12dy / v21m, -v12dz / v21m }, // (v1 - v2)norm
		v1ms,
		v2ms,
		v12ms,
		v21ms,
		v1m,
		v2m,
		v12m,
		v21m,
		v1x * v2x + v1y * v2y + v1z * v2z, // dot
		s, // scalar
		{ v1x + s, v1y + s, v1z + s }, // scalar + v1
		{ s - v1x, s - v1y, s - v1z }, // scalar - v1
		{ v1x - s, v1y - s, v1z - s }, // v1 - scalar
		{ s * v1x, s * v1y, s * v1z }, // scalar * v1
		{ v1x / s, v1y / s, v1z / s }, // v1 / scalar
		{ s / v1x, s / v1y, s / v1z }, // v1 / scalar
		{ std::min(v1x, v2x), std::min(v1y, v2y), std::min(v1z, v2z) }, // min(v1, v2)
		{ std::max(v1x, v2x), std::max(v1y, v2y), std::max(v1z, v2z) } // max(v1, v2)
	};
	testVecGeneral(args);
	rqm::vec3 indTest = { 1.0f, 2.0f, 3.0f };
	assert(indTest[0] == indTest.x);
	assert(indTest[1] == indTest.y);
	assert(indTest[2] == indTest.z);
	rqm::vec3 c1 = { 1.0f, 2.0f, 3.0f };
	rqm::vec3 c2 = { 3.0f, 4.0f, 5.0f };
	assert(rqm::cross(c1, c2) == rqm::vec3(-2.0f, 4.0f, -2.0f));
	rqm::vec3 c3;
	rqm::cross_in_place(c2, c1, c3);
	assert(c3 == -rqm::cross(c1, c2));
}

void testVec4()
{
	float v1x = -4.7f; float v1y = 5.6f; float v1z = -3.2f; float v1w = 10.5f;
	float v2x = 9.8f; float v2y = 2.3f; float v2z = -0.8f; float v2w = -7.8f;
	float v12dx = v2x - v1x; float v12dy = v2y - v1y; float v12dz = v2z - v1z; float v12dw = v2w - v1w;
	float v1ms = v1x * v1x + v1y * v1y + v1z * v1z + v1w * v1w;
	float v2ms = v2x * v2x + v2y * v2y + v2z * v2z + v2w * v2w;
	float v12ms = v12dx * v12dx + v12dy * v12dy + v12dz * v12dz + v12dw * v12dw;
	float v21ms = (-v12dx * -v12dx) + (-v12dy * -v12dy) + (-v12dz * -v12dz) + (-v12dw * -v12dw);
	float v1m = rqm::sqrt(v1ms);
	float v2m = rqm::sqrt(v2ms);
	float v12m = rqm::sqrt(v12ms);
	float v21m = rqm::sqrt(v21ms);
	float s = -4.6f;
	VecTestInputs<rqm::vec4> args =
	{
		{ v1x, v1y, v1z, v1w }, // v1
		{ v2x, v2y, v2z, v2w }, // v2
		{ v1x + v2x, v1y + v2y, v1z + v2z, v1w + v2w }, // sum
		{ v12dx, v12dy, v12dz, v12dw }, // v2 - v1
		{ v1x * v2x, v1y * v2y, v1z * v2z, v1w * v2w }, // prod
		{ v1x / v2x, v1y / v2y, v1z / v2z, v1w / v2w }, // v1 / v2
		{ v2x / v1x, v2y / v1y, v2z / v1z, v2w / v1w }, // v2 / v1
		{ v1x / v1m, v1y / v1m, v1z / v1m, v1w / v1m }, // v1norm
		{ v2x / v2m, v2y / v2m, v2z / v2m, v2w / v2m }, // v2norm
		{ v12dx / v12m, v12dy / v12m, v12dz / v12m, v12dw / v12m }, // (v2 - v1)norm
		{ -v12dx / v21m, -v12dy / v21m, -v12dz / v21m, -v12dw / v21m }, // (v1 - v2)norm
		v1ms,
		v2ms,
		v12ms,
		v21ms,
		v1m,
		v2m,
		v12m,
		v21m,
		v1x * v2x + v1y * v2y + v1z * v2z + v1w * v2w, // dot
		s, // scalar
		{ v1x + s, v1y + s, v1z + s, v1w + s }, // scalar + v1
		{ s - v1x, s - v1y, s - v1z, s - v1w }, // scalar - v1
		{ v1x - s, v1y - s, v1z - s, v1w - s }, // v1 - scalar
		{ s * v1x, s * v1y, s * v1z, s * v1w }, // scalar * v1
		{ v1x / s, v1y / s, v1z / s, v1w / s }, // v1 / scalar
		{ s / v1x, s / v1y, s / v1z, s / v1w }, // v1 / scalar
		{ std::min(v1x, v2x), std::min(v1y, v2y), std::min(v1z, v2z), std::min(v1w, v2w) }, // min(v1, v2)
		{ std::max(v1x, v2x), std::max(v1y, v2y), std::max(v1z, v2z), std::max(v1w, v2w) } // max(v1, v2)
	};
	testVecGeneral(args);
	rqm::vec4 indTest = { 1.0f, 2.0f, 3.0f, 4.0f };
	assert(indTest[0] == indTest.x);
	assert(indTest[1] == indTest.y);
	assert(indTest[2] == indTest.z);
	assert(indTest[3] == indTest.w);
}

void testQuat()
{
	std::cout << "===================================\nTesting Quaternions" << '\n';
	rqm::quat rotateByZRad = rqm::gen_quat_angle_about_axis_rad(rqm::PI<float> / 2, rqm::vec3(0, 0, 1));
	rqm::quat rotateByZDeg = rqm::gen_quat_angle_about_axis_deg(90.0f, rqm::vec3(0, 0, 1));
	assert(rotateByZDeg == rotateByZRad);
	rqm::vec3 zaxis = { 0,0,1 };
	rqm::vec4 xpoint = { 1,0,0,1 };
	rqm::vec4 yaxis = { 0,1,0,0 };
	assert(rotateByZRad.rotate(zaxis) == zaxis);
	assert(rotateByZDeg.rotate(xpoint) == rqm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	rotateByZDeg.rotate_in_place(yaxis);
	assert(yaxis == rqm::vec4(-1, 0, 0, 0));
	rqm::mat33 rotateMatrix = rqm::quat_to_mat33(rotateByZDeg);
	assert(rotateMatrix * yaxis.xyz == rqm::vec3(0, -1, 0));
	assert(rqm::mat33_to_quat(rotateMatrix) == rotateByZDeg);
	
	std::cout << std::endl;
}

void testLerp()
{
	rqm::vec3 v1 = { 0, 1, 2 };
	rqm::vec3 v2 = { 2, 3, 4 };
	assert(rqm::lerp(v1, v2, 0.5f) == rqm::vec3(1, 2, 3));
	float invsqrt2 = 1.0f / std::sqrt(2.0f);
	rqm::vec3 nv1 = { 0, 1, 0 };
	rqm::vec3 nv2 = { 1, 0, 0 };
	assert(rqm::nlerp(nv1, nv2, 0.5f) == rqm::vec3(invsqrt2, invsqrt2, 0));
	rqm::quat q1 = { 0, 1, 0, 0 };
	rqm::quat q2 = { 1, 0, 0, 0 };
	assert(rqm::slerp(q1, q2, 0.5f) == rqm::quat(invsqrt2, invsqrt2, 0, 0));
	assert(rqm::slerp_short(q1, q2, 0.5f) == rqm::quat(invsqrt2, invsqrt2, 0, 0));
	// TODO maybe someday can test the special behaviour of short slerp compared to normal slerp
}

template<typename MatT, typename VecT>
struct TestMatGeneralInputs
{
	MatT m1;
	MatT m2;
	VecT v1;
	VecT m1V1Prod;
	VecT m2V1Prod;
	MatT m12Sum;
	MatT m12Diff; // m2 - m1
	MatT m1T;
	MatT m1Inv;
	MatT m12Prod; // m1 * m2
	MatT m21Prod; // m2 * m1
	float m1Trace;
	float m1Det;
	bool m1Ortho;
	bool m2Ortho;
	float scalar;
	MatT m1ScalarSum;
	MatT m1ScalarDiff; // scalar - m1
	MatT scalarM1Diff; // m1 - scalar
	MatT m1ScalarProd;
	MatT m1ScalarDiv; // m1 / scalar
};

template<typename MatT, typename VecT>
void TestMatGeneral(const TestMatGeneralInputs<MatT, VecT>& in)
{
	std::cout << "===================================\nTesting " << in.m1 << " and " << in.m2 << '\n';

	assert(in.m1 * in.v1 == in.m1V1Prod);
	assert(in.m2 * in.v1 == in.m2V1Prod);

	assert(in.m1 + in.m2 == in.m12Sum);
	assert(in.m2 - in.m1 == in.m12Diff);
	assert(in.m1.get_transpose() == in.m1T);
	assert(in.m1.get_inverse() == in.m1Inv);
	assert(in.m1 * in.m2 == in.m12Prod);
	assert(in.m2 * in.m1 == in.m21Prod);
	assert(in.m1.get_trace() == in.m1Trace);
	assert(in.m1.get_determinant() == in.m1Det);
	assert(in.m1.is_orthogonal() == in.m1Ortho);
	assert(in.m2.is_orthogonal() == in.m2Ortho);

	MatT temp = in.m1;
	temp += in.m2;
	assert(temp == in.m12Sum);
	temp = in.m2;
	temp -= in.m1;
	assert(temp == in.m12Diff);
	temp = in.m1;
	temp.transpose();
	assert(temp == in.m1T);
	temp = in.m1;
	temp.invert();
	assert(temp == in.m1Inv);

	assert(in.m1 + in.scalar == in.m1ScalarSum);
	assert(in.scalar + in.m1 == in.m1ScalarSum);
	assert(in.scalar - in.m1 == in.m1ScalarDiff);
	assert(in.m1 - in.scalar == in.scalarM1Diff);
	assert(in.m1 * in.scalar == in.m1ScalarProd);
	assert(in.scalar * in.m1 == in.m1ScalarProd);
	assert(in.m1 / in.scalar == in.m1ScalarDiv);

	temp = in.m1;
	temp += in.scalar;
	assert(temp == in.m1ScalarSum);
	temp = in.m1;
	temp -= in.scalar;
	assert(temp == in.scalarM1Diff);
	temp = in.m1;
	temp *= in.scalar;
	assert(temp == in.m1ScalarProd);
	temp = in.m1;
	temp /= in.scalar;
	assert(temp == in.m1ScalarDiv);

	std::cout << std::endl;
}

void testMat2()
{
	rqm::mat22 m = { 1,2,3,4 };
	if(m.xx == m.flat_data[0] && m.xy == m.flat_data[1] && m.yx == m.flat_data[2] && m.yy == m.flat_data[3] &&
		m.xx == m.data[0][0] && m.xy == m.data[0][1] && m.yx == m.data[1][0] && m.yy == m.data[1][1] &&
		m.xx == m.row0.x && m.xy == m.row0.y && m.yx == m.row1.x && m.yy == m.row1.y)
	{
		std::cout << "data structure of mat22 is consistent\n";
	}
	else
	{
		assert(false);
	}

	TestMatGeneralInputs<rqm::mat22, rqm::vec2> inputs =
	{
		{ 1.f, 2.f, 3.f, 4.f }, // m1
		{ 5.f, 6.f, 7.f, 8.f }, // m2
		{ 9.f, 10.f }, // v1
		{ 29.f, 67.f }, // m1V1Prod
		{ 105.f, 143.f }, // m2V1Prod
		{ 6.f, 8.f, 10.f, 12.f }, // m12Sum
		{ 4.f, 4.f, 4.f, 4.f }, // m12Diff // m2 - m1
		{ 1.f, 3.f, 2.f, 4.f }, // m1T
		{ -2.f, 1.f, 3.f /2.f, -1.f /2.f }, // m1Inv
		{ 19.f, 22.f, 43.f, 50.f }, // m12Prod // m1 * m2
		{ 23.f, 34.f, 31.f, 46.f }, // m21Prod // m2 * m1
		5.0f, // m1Trace
		-2.0f, // m1Det
		false, // m1Ortho
		false, // m2Ortho
		-12.0f, // scalar
		{ -11.f, -10.f, -9.f, -8.f }, // m1ScalarSum
		{ -13.f, -14.f, -15.f, -16.f }, // m1ScalarDiff // scalar - m1
		{ 13.f, 14.f, 15.f, 16.f }, // scalarM1Diff // m1 - scalar
		{ -12.f, -24.f, -36.f, -48.f }, // m1ScalarProd
		{ 1.f/-12.f, 2.f/-12.f, 3.f/-12.f, 4.f/-12.f } // m1ScalarDiv // m1 / scalar
	};
	TestMatGeneral(inputs);

	rqm::mat22 transform = rqm::gen_mat22_from_cols(rqm::vec2(0, 1), rqm::vec2(-1, 0)); // rotate 90 degrees
	std::vector<rqm::vec2> verts = { {1, 0}, {0, 1}, {-1, 0}, {0, -1}, {5 , 1}, {-2, 10}, {-4, -3} };
	std::vector<rqm::vec2> correct = { {0, 1}, {-1, 0}, {0, -1}, {1, 0}, {-1 , 5}, {-10, -2}, {3, -4} };
	assert(transform * verts == correct);
	rqm::mat22_mul_in_place(transform, verts);
	assert(verts == correct);

	rqm::mat22 i;
	rqm::set_identity(i);
	assert(i == rqm::identity22<float>());
}

void testMat3()
{
	rqm::mat33 m = { 1,2,3,4,5,6,7,8,9 };
	if(m.xx == m.flat_data[0] && m.xy == m.flat_data[1] && m.xz == m.flat_data[2] &&
		m.yx == m.flat_data[3] && m.yy == m.flat_data[4] && m.yz == m.flat_data[5] &&
		m.zx == m.flat_data[6] && m.zy == m.flat_data[7] && m.zz == m.flat_data[8] &&
		m.xx == m.data[0][0] && m.xy == m.data[0][1] && m.xz == m.data[0][2] &&
		m.yx == m.data[1][0] && m.yy == m.data[1][1] && m.yz == m.data[1][2] &&
		m.zx == m.data[2][0] && m.zy == m.data[2][1] && m.zz == m.data[2][2] &&
		m.xx == m.row0.x && m.xy == m.row0.y && m.xz == m.row0.z &&
		m.yx == m.row1.x && m.yy == m.row1.y && m.yz == m.row1.z &&
		m.zx == m.row2.x && m.zy == m.row2.y && m.zz == m.row2.z)
	{
		std::cout << "data structure of mat33 is consistent\n";
	}
	else
	{
		assert(false);
	}

	TestMatGeneralInputs<rqm::mat33, rqm::vec3> inputs =
	{
		{ -4.f, -3.f, -2.f, -1.f, 1.f, 2.f, 3.f, 4.f, 5.f }, // m1
		{ 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f }, // m2
		{ 9.f, 10.f, 11.f }, // v1
		{ -88.f, 23.f, 122.f }, // m1V1Prod
		{ 62.f, 152.f, 242.f }, // m2V1Prod
		{ -3.f, -1.f, 1.f, 3.f, 6.f, 8.f, 10.f, 12.f, 14.f }, // m12Sum
		{ 5.f, 5.f, 5.f, 5.f, 4.f, 4.f, 4.f, 4.f, 4.f }, // m12Diff // m2 - m1
		{ -4.f, -1.f, 3.f, -3.f, 1.f, 4.f, -2.f, 2.f, 5.f }, // m1T
		{ 3.f/7.f, -1.f, 4.f/7.f, -11.f/7.f, 2.f, -10.f/7.f, 1.f, -1.f, 1.f }, // m1Inv
		{ -30.f, -39.f, -48.f, 17.f, 19.f, 21.f, 54.f, 66.f, 78.f }, // m12Prod // m1 * m2
		{ 3.f, 11.f, 17.f, -3.f, 17.f, 32.f, -9.f, 23.f, 47.f }, // m21Prod // m2 * m1
		2.0f, // m1Trace
		-7.0f, // m1Det
		false, // m1Ortho
		false, // m2Ortho
		3.0f, // scalar
		{ -1.f, 0.f, 1.f, 2.f, 4.f, 5.f, 6.f, 7.f, 8.f }, // m1ScalarSum
		{ 7.f, 6.f, 5.f, 4.f, 2.f, 1.f, 0.f, -1.f, -2.f }, // m1ScalarDiff // scalar - m1
		{ -7.f, -6.f, -5.f, -4.f, -2.f, -1.f, 0.f, 1.f, 2.f }, // scalarM1Diff // m1 - scalar
		{ -12.f, -9.f, -6.f, -3.f, 3.f, 6.f, 9.f, 12.f, 15.f }, // m1ScalarProd
		{ -4.f / 3.f, -3.f / 3.f, -2.f / 3.f, -1.f / 3.f, 1.f / 3.f, 2.f / 3.f, 3.f / 3.f, 4.f / 3.f, 5.f / 3.f } // m1ScalarDiv // m1 / scalar
	};
	TestMatGeneral(inputs);

	{
		rqm::mat33 transform3 = gen_mat33_from_cols(rqm::vec3(0, -1, 0), rqm::vec3(-1, 0, 0), rqm::vec3(0, 0, -1));
		std::vector<rqm::vec3> verts3 = { {1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {-1, 0, 1}, {2, 0, -1}, {5 , 1, -10}, {2, 10, 3}, {-4, -3, -5} };
		std::vector<rqm::vec3> points3 = { {0, -1, 0}, {-1, 0, 0}, {0, 0, -1}, {0, 1, -1}, {0, -2, 1}, {-1 , -5, 10}, {-10, -2, -3}, {3, 4, 5} };
		assert(rqm::mat33_mul(transform3, verts3) == points3);
		rqm::mat33_mul_in_place(transform3, verts3);
		assert(verts3 == points3);
	}
	{
		rqm::mat33 transform2 = rqm::gen_mat33_from_cols<float>({ 0, 1, 0 }, { -1, 0, 0 }, { 5, 0, 1 });
		std::vector<rqm::vec2> verts2 = { {1, 0}, {0, 1}, {-1, 0}, {0, -1}, {5, 1}, {-2, 10}, {-4, -3} };
		std::vector<rqm::vec2> points2 = { {5, 1}, {4, 0}, {5, -1}, {6, 0}, {4, 5}, {-5, -2}, {8, -4} };
		std::vector<rqm::vec2> dir2 = { {0, 1}, {-1, 0}, {0, -1}, {1, 0}, {-1, 5}, {-10, -2}, {3, -4} };
		assert(rqm::mat33_mul(transform2, verts2, 1.0f) == points2);
		assert(rqm::mat33_mul(transform2, verts2, 0.0f) == dir2);
		rqm::mat33_mul_in_place(transform2, verts2, 1.0f);
		assert(verts2 == points2);
	}

	rqm::mat33 i;
	rqm::set_identity(i);
	assert(i == rqm::identity33<float>());
}

void testMat4()
{
	rqm::mat44 m = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };
	if(m.xx == m.flat_data[0] && m.xy == m.flat_data[1] && m.xz == m.flat_data[2] && m.xw == m.flat_data[3] &&
		m.yx == m.flat_data[4] && m.yy == m.flat_data[5] && m.yz == m.flat_data[6] && m.yw == m.flat_data[7] &&
		m.zx == m.flat_data[8] && m.zy == m.flat_data[9] && m.zz == m.flat_data[10] && m.zw == m.flat_data[11] &&
		m.wx == m.flat_data[12] && m.wy == m.flat_data[13] && m.wz == m.flat_data[14] && m.ww == m.flat_data[15] &&
		m.xx == m.data[0][0] && m.xy == m.data[0][1] && m.xz == m.data[0][2] && m.xw == m.data[0][3] &&
		m.yx == m.data[1][0] && m.yy == m.data[1][1] && m.yz == m.data[1][2] && m.yw == m.data[1][3] &&
		m.zx == m.data[2][0] && m.zy == m.data[2][1] && m.zz == m.data[2][2] && m.zw == m.data[2][3] &&
		m.wx == m.data[3][0] && m.wy == m.data[3][1] && m.wz == m.data[3][2] && m.ww == m.data[3][3] &&
		m.xx == m.row0.x && m.xy == m.row0.y && m.xz == m.row0.z && m.xw == m.row0.w &&
		m.yx == m.row1.x && m.yy == m.row1.y && m.yz == m.row1.z && m.yw == m.row1.w &&
		m.zx == m.row2.x && m.zy == m.row2.y && m.zz == m.row2.z && m.zw == m.row2.w &&
		m.wx == m.row3.x && m.wy == m.row3.y && m.wz == m.row3.z && m.ww == m.row3.w)
	{
		std::cout << "data structure of mat44 is consistent\n";
	}
	else
	{
		assert(false);
	}

	TestMatGeneralInputs<rqm::mat44, rqm::vec4> inputs =
	{
		{ 4.f, 0.f, 0.f, 0.f, 0.f, 0.f, 2.f, 0.f, 0.f, 1.f, 2.f, 0.f, 1.f, 0.f, 0.f, 1.f }, // m1
		{ 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f, 16.f }, // m2
		{ 9.f, 10.f, 11.f, 12.f }, // v1
		{ 36.f, 22.f, 32.f, 21.f }, // m1V1Prod
		{ 110.f, 278.f, 446.f, 614.f }, // m2V1Prod
		{ 5.f, 2.f, 3.f, 4.f, 5.f, 6.f, 9.f, 8.f, 9.f, 11.f, 13.f, 12.f, 14.f, 14.f, 15.f, 17.f }, // m12Sum
		{ -3.f, 2.f, 3.f, 4.f, 5.f, 6.f, 5.f, 8.f, 9.f, 9.f, 9.f, 12.f, 12.f, 14.f, 15.f, 15.f }, // m12Diff // m2 - m1
		{ 4.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 2.f, 2.f, 0.f, 0.f, 0.f, 0.f, 1.f }, // m1T
		{ 0.25f, 0.f, 0.f, 0.f, 0.f, -1.f, 1.f, 0.f, 0.f, 0.5f, 0.f, 0.f, -0.25f, 0.f, 0.f, 1.0f }, // m1Inv
		{ 4.f, 8.f, 12.f, 16.f, 18.f, 20.f, 22.f, 24.f, 23.f, 26.f, 29.f, 32.f, 14.f, 16.f, 18.f, 20.f }, // m12Prod // m1 * m2
		{ 8.f, 3.f, 10.f, 4.f, 28.f, 7.f, 26.f, 8.f, 48.f, 11.f, 42.f, 12.f, 68.f, 15.f, 58.f, 16.f }, // m21Prod // m2 * m1
		7.0f, // m1Trace
		-8.0f, // m1Det
		false, // m1Ortho
		false, // m2Ortho
		-2.0f, // scalar
		{ 2.f, -2.f, -2.f, -2.f, -2.f, -2.f, 0.f, -2.f, -2.f, -1.f, 0.f, -2.f, -1.f, -2.f, -2.f, -1.f }, // m1ScalarSum
		{ -6.f, -2.f, -2.f, -2.f, -2.f, -2.f, -4.f, -2.f, -2.f, -3.f, -4.f, -2.f, -3.f, -2.f, -2.f, -3.f }, // m1ScalarDiff // scalar - m1
		{ 6.f, 2.f, 2.f, 2.f, 2.f, 2.f, 4.f, 2.f, 2.f, 3.f, 4.f, 2.f, 3.f, 2.f, 2.f, 3.f }, // scalarM1Diff // m1 - scalar
		{ -8.f, 0.f, 0.f, 0.f, 0.f, 0.f, -4.f, 0.f, 0.f, -2.f, -4.f, 0.f, -2.f, 0.f, 0.f, -2.f }, // m1ScalarProd
		{ -2.f, 0.f, 0.f, 0.f, 0.f, 0.f, -1.f, 0.f, 0.f, 1.f/-2.f, -1.f, 0.f, 1.f/-2.f, 0.f, 0.f, 1.f/-2.f } // m1ScalarDiv // m1 / scalar
	};
	TestMatGeneral(inputs);

	{
		rqm::mat44 transform4 = gen_mat44_from_cols(rqm::vec4(0, -1, 0, 0), rqm::vec4(-1, 0, 0, 0), rqm::vec4(0, 0, -1, 0), rqm::vec4(0, 0, 0, 1));
		std::vector<rqm::vec4> verts4 = { {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {-1, 0, 1, 1}, {2, 0, -1, 1}, {5 , 1, -10, 1}, {2, 10, 3, 1}, {-4, -3, -5, 1} };
		std::vector<rqm::vec4> results = { {0, -1, 0, 0}, {-1, 0, 0, 0}, {0, 0, -1, 0}, {0, 1, -1, 1}, {0, -2, 1, 1}, {-1, -5, 10, 1}, {-10, -2, -3, 1}, {3, 4, 5, 1} };
		assert(transform4 * verts4 == results);
		rqm::mat44_mul_in_place(transform4, verts4);
		assert(verts4 == results);
	}

	{
		rqm::mat44 transform3 = rqm::gen_mat44_from_cols<float>({ 0, -1, 0, 0 }, { 1, 0, 0, 0 }, { 0, 0, -1, 0 }, { 5, -2, 1, 1 });
		std::vector<rqm::vec3> verts3 = { {1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {-1, 0, 1}, {2, 0, -1}, {5 , 1, -10}, {2, 10, 3}, {-4, -3, -5} };
		std::vector<rqm::vec3> points3 = { {5, -3, 1}, {6, -2, 1}, {5, -2, 0}, {5, -1, 0}, {5, -4, 2}, {6, -7, 11}, {15, -4, -2}, {2, 2, 6} };
		std::vector<rqm::vec3> dir3 = { {0, -1, 0}, {1, 0, 0}, {0, 0, -1}, {0, 1, -1}, {0, -2, 1}, {1, -5, 10}, {10, -2, -3}, {-3, 4, 5} };
		assert(rqm::mat44_mul(transform3, verts3, 1.0f) == points3);
		assert(rqm::mat44_mul(transform3, verts3, 0.0f) == dir3);
		rqm::mat44_mul_in_place(transform3, verts3, 1.0f);
		assert(verts3 == points3);
	}

	rqm::mat44 i;
	rqm::set_identity(i);
	assert(i == rqm::identity44<float>());
}

void testBasic()
{
	std::cout << "===================================\nTesting Basic Math Header" << '\n';

	assert(rqm::epsilon_equals(rqm::rad2deg(rqm::deg2rad(90.f)), 90.f));
	assert(rqm::PI<float> > 3.1415 && rqm::PI<float> < 3.15);
	assert(rqm::epsilon_equals(rqm::sin(rqm::PI<float> / 6.f), 0.5f));
	assert(rqm::epsilon_equals(rqm::cos(rqm::PI<float> / 3.f), 0.5f));
	assert(rqm::epsilon_equals(rqm::tan(rqm::PI<float> / 4.f), 1.f));
	assert(rqm::epsilon_equals(rqm::asin(0.5f), rqm::PI<float> / 6.f));
	assert(rqm::epsilon_equals(rqm::acos(0.5f), rqm::PI<float> / 3.f));
	assert(rqm::epsilon_equals(rqm::atan(1.f), rqm::PI<float> / 4.f));

	assert(rqm::signum(0.f) == 0.f);
	assert(rqm::signum(20.f) == 1.f);
	assert(rqm::signum(-33.f) == -1.f);
	assert(rqm::signum_no_zero(0.f) == 1.f);
	assert(rqm::signum_no_zero(20.f) == 1.f);
	assert(rqm::signum_no_zero(-33.f) == -1.f);

	assert(rqm::max(-2.f, 5.f) == 5.f);
	assert(rqm::min(-3.f, 4.f) == -3.f);
	assert(rqm::abs(3.f) == 3.f);
	assert(rqm::abs(-12.f) == 12.f);

	assert(rqm::sqrt(1.f) == 1.f);
	assert(rqm::sqrt(36.f) == 6.f);
}

void runMathTests()
{
	std::cout << "\n***********************************\nRunning Tests For Petra...\n";
	testVec1();
	testVec2();
	testVec3();
	testVec4();
	testQuat();
	testLerp();
	testMat2();
	testMat3();
	testMat4();
	testBasic();
}