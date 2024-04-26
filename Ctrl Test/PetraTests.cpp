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
	assert(in.v1 * in.v2 == in.vHProd);
	assert(in.v1 / in.v2 == in.v12Div);
	assert(in.v2 / in.v1 == in.v21Div);

	// TODO test *= ops

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

	// TODO test modify by scalar

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
		{ 5.0f, 3.1f },
		{ 2.5f, -0.2f },
		{ 5.0f + 2.5f, 3.1f - 0.2f },
		{ -2.5f, -3.3f },
		{ 5.0f * 2.5f, -0.2f * 3.1f },
		{ 2.0f, 3.1f / -0.2f },
		{ 0.5f, -0.2f / 3.1f },
		{ 5.0f / v1m, 3.1f / v1m },
		{ 2.5f / v2m, -0.2f / v2m },
		{ -2.5f / v12m, -3.3f / v12m },
		{ 2.5f / v21m, 3.3f / v21m },
		v1ms,
		v2ms,
		v12ms,
		v21ms,
		v1m,
		v2m,
		v12m,
		v21m
	};
	testVecGeneral(args);
}

void testVec3()
{
	rqm::vec3 a = { 1.0, 2.0, 3.0 };
	std::cout << a << '\n';
	rqm::vec3 b = rqm::vec3(3, 4, 5);
	std::cout << a + b << ' ' << a - b << ' ' << a * b << ' ' << a / b << '\n';
	std::cout << b.magnitude() << ' ';
	float bdist = b.normalize();
	std::cout << bdist << ' ' << b << '\n';
	std::cout << b + 2 << ' ' << b - 2 << ' ' << b * 2 << ' ' << b / 2 << '\n';
	b += 1;
	std::cout << b << ' ';
	b -= 1;
	std::cout << b << ' ';
	b *= 2;
	std::cout << b << ' ';
	b /= 2;
	std::cout << b << '\n';
	rqm::vec2 c = rqm::vec2(1, 3);
	rqm::vec3 d = rqm::vec3(c, 2);
	rqm::vec3 e = c;
	std::cout << c << ' ' << d << ' ' << e << '\n';
	rqm::vec3 f = { 1, 3, 4 };
	f.xy = { 2, 5 };
	std::cout << f << ' ' << d.xy + f.xy << ' ';
	f.xy *= 2;
	std::cout << f << "\n\n";
}

void testVec4()
{
	rqm::vec4 a = { 1.0, 2.0, 3.0, 4.0 };
	std::cout << a << '\n';
	rqm::vec4 b = rqm::vec4(3, 4, 5, 6);
	std::cout << a + b << ' ' << a - b << ' ' << a * b << ' ' << a / b << '\n';
	std::cout << b.magnitude() << ' ';
	float bdist = b.normalize();
	std::cout << bdist << ' ' << b << '\n';
	std::cout << b + 2 << ' ' << b - 2 << ' ' << b * 2 << ' ' << b / 2 << '\n';
	b += 1;
	std::cout << b << ' ';
	b -= 1;
	std::cout << b << ' ';
	b *= 2;
	std::cout << b << ' ';
	b /= 2;
	std::cout << b << '\n';
	rqm::vec3 c = rqm::vec3(1, 3, 5);
	rqm::vec4 d = rqm::vec4(c, 2);
	rqm::vec4 e = c;
	std::cout << c << ' ' << d << ' ' << e << '\n';
	rqm::vec4 f = rqm::vec4(1);
	f.xyz.normalize_no_return();
	std::cout << f << "\n\n";
}

void testVecOps()
{
	rqm::vec2 a = rqm::vec2(2, 3);
	std::cout << rqm::dot(a, a) << '\n';
	rqm::vec3 b = rqm::vec3(2, 3, 4);
	std::cout << rqm::dot(b, b) << '\n';
	rqm::vec4 c = rqm::vec4(2, 3, 4, 5);
	std::cout << rqm::dot(c, c) << '\n';

	rqm::vec3 xaxis = { 1, 0, 0 };
	rqm::vec3 yaxis = { 0, 1, 0 };
	rqm::vec3 zaxis = { 0, 0, 1 };
	std::cout << rqm::cross(xaxis, yaxis) << ' ' << rqm::cross(zaxis, xaxis) << ' ' << rqm::cross(yaxis, zaxis) << '\n';

	rqm::vec3 begin = { 1, 2, 3 };
	rqm::vec3 end = { 4, 8, 12 };
	std::cout << rqm::lerp(begin, end, 0.5f) << '\n';

	rqm::vec4 d = { -1, 1, 0, 1 };
	rqm::vec4 e = { -5, -2, 5, 2 };
	std::cout << "min " << rqm::min(d, e) << " max " << rqm::max(e, d) << '\n';
	std::cout << "\n";
}

void testQuat()
{
	rqm::quat rotateByZ = rqm::gen_quat_angle_about_axis_rad(rqm::PI<float> / 2, rqm::vec3(0, 0, 1));
	rqm::vec3 zaxis = { 0,0,1 };
	rqm::vec4 xpoint = { 1,0,0,1 };
	rqm::vec4 yaxis = { 0,1,0,0 };
	rqm::quat rotateByZ180 = rqm::gen_quat_angle_about_axis_deg(180.0f, rqm::vec3(0, 0, 1));
	std::cout << "zaxis rotated about z 90 degrees is " << rotateByZ.rotate(zaxis) << '\n';
	std::cout << "xpoint rotated about z 90 degrees is " << rotateByZ.rotate(xpoint) << '\n';
	std::cout << "yaxis rotated about z 90 degrees is " << rotateByZ.rotate(yaxis) << '\n';
	std::cout << "zaxis rotated about z 180 degrees is " << rotateByZ180.rotate(zaxis) << '\n';
	std::cout << "xpoint rotated about z 180 degrees is " << rotateByZ180.rotate(xpoint) << '\n';
	std::cout << "yaxis rotated about z 180 degrees is " << rotateByZ180.rotate(yaxis) << '\n';
	std::cout << "\n";
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

std::string det(const std::string& a, const std::string& b, const std::string& c, const std::string& d)
{
	return a + " * " + d + " - " + b + " * " + c;
}

/*
a, b, c,
d, e, f,
g, h, i

*/
std::string det(const std::string& a, const std::string& b, const std::string& c, const std::string& d, const std::string& e, const std::string& f, const std::string& g, const std::string& h, const std::string& i, std::unordered_map<std::string, int>& map, int& ind, bool useSub = false)
{
	std::string p[3];
	p[0] = det(e, f, h, i);
	p[1] = det(d, f, g, i);
	p[2] = det(d, e, g, h);

	for(int i = 0; i < 3; i++)
	{
		if(map.find(p[i]) == map.end())
		{
			map[p[i]] = ind;
			ind++;
		}
	}

	if(useSub)
	{
		return a + " * p" + std::to_string(map[p[0]]) + " - " + b + " * p" + std::to_string(map[p[1]]) + " + " + c + " * p" + std::to_string(map[p[2]]);
	}
	return a + "(" + p[0] + ") - " + b + "(" + p[1] + ") + " + c + "(" + p[2] + ")";
}

void helpInvert()
{
	std::unordered_map<std::string, int> pmap;
	int index = 0;
	//std::cout << det("xx", "xy", "yx", "yy") << '\n';
	//std::cout << det("xx", "xy", "xz", "yx", "yy", "yz", "zx", "zy", "zz", pmap, index);
	std::string names[4][4] = { {"xx", "xy", "xz", "xw"}, {"yx", "yy", "yz", "yw"}, {"zx", "zy", "zz", "zw"}, {"wx", "wy", "wz", "ww"} };
	std::vector<std::string> vars = { "A", "B", "C", "D", "E", "F","G","H","I","J","K","L","M","N","O","P" };
	std::vector<std::string> vals;
	for(int j = 0; j < 4; j++)
	{
		for(int i = 0; i < 4; i++)
		{
			std::vector<std::string> temp;
			for(int x = 0; x < 4; x++)
			{
				for(int y = 0; y < 4; y++)
				{
					if(x != i && y != j)
					{
						temp.push_back(names[x][y]);
					}
				}
			}
			vals.push_back(det(temp[0], temp[1], temp[2], temp[3], temp[4], temp[5], temp[6], temp[7], temp[8], pmap, index, true));
		}
	}
	for(const auto& it : pmap)
	{
		std::cout << "p" << it.second << " = " << it.first << '\n';
	}
	for(int i = 0; i < 16; i++)
	{
		std::cout << vars[i] << " = " << vals[i] << '\n';
	}
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
	testVecOps();
	testQuat();
	testMat2();
	testMat3();
	testMat4();
}