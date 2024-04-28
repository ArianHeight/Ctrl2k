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

void testMat2()
{
	rqm::mat22 m2 = { 1, 2, 3, 4 };
	std::cout << m2.data[0][0] << ' ' << m2.data[0][1] << ' ' << m2.data[1][0] << ' ' << m2.data[1][1] << '\n';
	std::cout << m2.flat_data[0] << ' ' << m2.flat_data[1] << ' ' << m2.flat_data[2] << ' ' << m2.flat_data[3] << '\n';
	std::cout << m2.x << ' ' << m2.y << '\n';
	rqm::mat22 a = { 2, 4, 7, 9 };
	rqm::mat22 b = { 3, 5, 6, 8 };
	rqm::mat22 c = a * b; // should be 30, 42, 75, 107
	std::cout << c << '\n';
	rqm::vec2 v = { 3, 5 };
	rqm::vec2 u = a * v;
	std::cout << u << '\n'; // should be 26,66
	rqm::mat22 ident = rqm::gen_mat22_identity<float>();
	std::cout << ident << '\n';
	rqm::mat22 ct = c;
	std::cout << ct << '\n';
	ct.transpose();
	std::cout << ct << '\n';
	rqm::mat22 ainv = a.get_inverse();
	std::cout << a << ' ' << ainv << '\n';
	std::cout << v << ' ' << ainv * u << '\n';
	std::cout << b << ' ' << a.get_inverse() * c << ' ' << a.get_inverse() * c << '\n';

	rqm::mat22 transform = rqm::gen_mat22_from_cols(rqm::vec2(0, 1), rqm::vec2(-1, 0)); // rotate 90 degrees
	std::vector<rqm::vec2> verts = { {1, 0}, {0, 1}, {-1, 0}, {0, -1}, {5 , 1}, {-2, 10}, {-4, -3} };
	for(int i = 0; i < verts.size(); i++)
		std::cout << '[' << verts[i] << ']';
	std::cout << '\n';
	std::vector<rqm::vec2> newverts = transform * verts;
	for(int i = 0; i < newverts.size(); i++)
		std::cout << '[' << newverts[i] << ']';
	std::cout << '\n';
	transform.invert();
	rqm::mat22_mul_in_place(transform, newverts);
	for(int i = 0; i < newverts.size(); i++)
		std::cout << '[' << newverts[i] << ']';
	std::cout << '\n';

	std::cout << "\n";
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

	rqm::mat33 a = { 1,2,3,4,5,6,7,8,9 };
	rqm::mat33 b = a;
	b.transpose();
	rqm::mat33 c = a * b;
	std::cout << c << '\n'; // should be [14 32 50 | 32 77 122 | 50 122 194]
	std::cout << "a^T " << ((a.get_transpose() == b) ? "is equal to b" : "is not equal to b") << '\n';
	std::cout << "a " << ((a == b) ? "is equal to b" : "is not equal to b") << '\n';

	rqm::mat33 d = { 4,2,1,3,3,4,1,-4,-2 };
	rqm::mat33 dinv = d.get_inverse();
	std::cout << dinv << '\n'; // should be [0.2222 0 0.11111 | 0.2222 -0.2 -0.28888 | -0.3333 0.4 0.13333]

	rqm::mat33 transform3 = gen_mat33_from_cols(rqm::vec3(0, -1, 0), rqm::vec3(-1, 0, 0), rqm::vec3(0, 0, -1));
	std::vector<rqm::vec3> verts3 = { {1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {-1, 0, 1}, {2, 0, -1}, {5 , 1, -10}, {2, 10, 3}, {-4, -3, -5} };
	for(int i = 0; i < verts3.size(); i++)
		std::cout << '[' << verts3[i] << ']';
	std::cout << '\n';
	std::vector<rqm::vec3> transformedverts3 = transform3 * verts3;
	for(int i = 0; i < transformedverts3.size(); i++)
		std::cout << '[' << transformedverts3[i] << ']';
	std::cout << '\n';
	rqm::mat33_mul_in_place(transform3.get_inverse(), transformedverts3);
	for(int i = 0; i < transformedverts3.size(); i++)
		std::cout << '[' << transformedverts3[i] << ']';
	std::cout << '\n';

	rqm::mat33 transform2 = rqm::gen_mat33_from_cols<float>({ 0, 1, 0 }, { -1, 0, 0 }, { 5, 0, 1 });
	std::vector<rqm::vec2> verts2 = { {1, 0}, {0, 1}, {-1, 0}, {0, -1}, {5 , 1}, {-2, 10}, {-4, -3} };
	for(int i = 0; i < verts2.size(); i++)
		std::cout << '[' << verts2[i] << ']';
	std::cout << '\n';
	std::vector<rqm::vec2> transformedverts2 = rqm::mat33_mul(transform2, verts2, 1.0f);
	for(int i = 0; i < transformedverts2.size(); i++)
		std::cout << '[' << transformedverts2[i] << ']';
	std::cout << '\n';
	transform2.invert();
	rqm::mat33_mul_in_place(transform2, transformedverts2, 1.0f);
	for(int i = 0; i < transformedverts2.size(); i++)
		std::cout << '[' << transformedverts2[i] << ']';
	std::cout << '\n';

	std::cout << "\n";
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

	rqm::mat44 a = m;
	rqm::mat44 b = a;
	b.transpose();
	rqm::mat44 c = a * b;
	std::cout << a << ' ' << b << '\n';
	std::cout << c << '\n'; // should be [30 70 110 150 | 70 174 278 382 | 110 278 446 614 | 150 382 614 846]
	std::cout << "a^T " << ((a.get_transpose() == b) ? "is equal to b" : "is not equal to b") << '\n';
	std::cout << "a " << ((a == b) ? "is equal to b" : "is not equal to b") << '\n';

	rqm::mat44 d = { 1,1,1,0,0,3,1,2,2,3,1,0,1,0,2,1 };
	rqm::mat44 dinv = d.get_inverse();
	std::cout << dinv << '\n'; // should be [-3 -0.5 1.5 1 | 1 0.25 -0.25 -0.5 | 3 0.25 -1.25 -0.5 | -3 0 1 1]

	rqm::mat44 transform4 = gen_mat44_from_cols(rqm::vec4(0, -1, 0, 0), rqm::vec4(-1, 0, 0, 0), rqm::vec4(0, 0, -1, 0), rqm::vec4(0, 0, 0, 1));
	std::vector<rqm::vec4> verts4 = { {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {-1, 0, 1, 1}, {2, 0, -1, 1}, {5 , 1, -10, 1}, {2, 10, 3, 1}, {-4, -3, -5, 1} };
	for(int i = 0; i < verts4.size(); i++)
		std::cout << '[' << verts4[i] << ']';
	std::cout << '\n';
	std::vector<rqm::vec4> transformedverts4 = transform4 * verts4;
	for(int i = 0; i < transformedverts4.size(); i++)
		std::cout << '[' << transformedverts4[i] << ']';
	std::cout << '\n';
	rqm::mat44_mul_in_place(transform4.get_inverse(), transformedverts4);
	for(int i = 0; i < transformedverts4.size(); i++)
		std::cout << '[' << transformedverts4[i] << ']';
	std::cout << '\n';

	rqm::mat44 transform3 = rqm::gen_mat44_from_cols<float>({ 0, 1, 0, 0 }, { -1, 0, 0, 0 }, { 0, 0, -1, 0 }, { 5, -2, 1, 1 });
	std::vector<rqm::vec3> verts3 = { {1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {-1, 0, 1}, {2, 0, -1}, {5 , 1, -10}, {2, 10, 3}, {-4, -3, -5} };
	for(int i = 0; i < verts3.size(); i++)
		std::cout << '[' << verts3[i] << ']';
	std::cout << '\n';
	std::vector<rqm::vec3> transformedverts3 = rqm::mat44_mul(transform3, verts3, 1.0f);
	for(int i = 0; i < transformedverts3.size(); i++)
		std::cout << '[' << transformedverts3[i] << ']';
	std::cout << '\n';
	transform3.invert();
	rqm::mat44_mul_in_place(transform3, transformedverts3, 1.0f);
	for(int i = 0; i < transformedverts3.size(); i++)
		std::cout << '[' << transformedverts3[i] << ']';
	std::cout << '\n';

	std::cout << "\n";
}

void runMathTests()
{
	testVec1();
	testVec2();
	testVec3();
	testVec4();
	testQuat();
	testLerp();
	testMat2();
	testMat3();
	testMat4();
}