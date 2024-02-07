#pragma once
#include "vecmath.h"
#include <vector>

/*

The rqm base namespace includes everything used for general computer graphics based vector math.
This includes things like vectors of size 1-4, matrix 2x2, 3x3, 4x4(maybe 4x3 as well), and quaternions.

Here we are concerned about speed mostly, and nothing else.

This file is just for the square matrices of sizes 2x2, 3x3, and 4x4.

Note: The constructor for all mat types here are generating from rows! Use gen from col functions to create mats from columns.

*/

//#define RQM_MATRIX_DOT_IMPL

namespace rqm
{
    template <typename T>
    union mat22_base
    {
        struct { vec2_base<T> x; vec2_base<T> y; };
        struct { vec2_base<T> row0; vec2_base<T> row1; };
        struct { T xx; T xy; T yx; T yy; };
        T data[2][2]; // C++ is row major
        T flat_data[4];

        mat22_base() {}
        mat22_base(const T& val0, const T& val1, const T& val2, const T& val3) : xx(val0), xy(val1), yx(val2), yy(val3) {}
        mat22_base(const vec2_base<T>& vec0, const vec2_base<T>& vec1) : row0(vec0), row1(vec1) {}
        mat22_base(const mat22_base<T>& mat) : row0(mat.row0), row1(mat.row1) {}

        void set_columns(const vec2_base<T>& col0, const vec2_base<T>& col1)
        {
            xx = col0.x; xy = col1.x;
            yx = col0.y; yy = col1.y;
        }

        void transpose()
        {
            mat22_base<T> mat = *this;
            set_columns(mat.row0, mat.row1);
        }
        mat22_base<T> get_transpose() const
        {
            return mat22_base<T>(xx, yx,
                xy, yy);
        }

        // is orthogonal if all columns are orthonormal
        bool is_orthogonal() const
        {
            return *this * get_transpose() == mat22_base<T>(1, 0,
                0, 1); // test that transpose == inverse
        }

        T get_determinant() const { return xx * yy - xy * yx; }
        bool has_inverse() const { !epsilon_equals<T>(get_determinant(), 0); }

        // finds inverse the hard way, use get_transpose() if you know the mat is orthogonal
        mat22_base<T> get_inverse() const
        {
            T s = 1 / get_determinant();
            return mat22_base<T>(yy * s, -xy * s,
                -yx * s, xx * s);
        }
        void invert() { *this = get_inverse(); }

        mat22_base<T> operator+() const { return { row0, row1 }; }
        mat22_base<T> operator-() const { return { -row0, -row1 }; }

        mat22_base<T> operator+(const mat22_base<T>& other) const { return { row0 + other.row0, row1 + other.row1 }; }
        mat22_base<T> operator-(const mat22_base<T>& other) const { return { row0 - other.row0, row1 - other.row1 }; }
        // NOT element-wise, this is normal matrix multiplication
        mat22_base<T> operator*(const mat22_base<T>& other) const
        {
            // TODO: test with benchmark and see if we need to implement strassen's multiplication
            mat22_base<T> out;
#ifdef RQM_MATRIX_DOT_IMPL
            mat22_base<T> t = other.get_transpose();
            out.xx = dot(row0, t.row0);     out.xy = dot(row0, t.row1);
            out.yx = dot(row1, t.row0);     out.yy = dot(row1, t.row1);
#else
            out.xx = xx * other.xx + xy * other.yx;     out.xy = xx * other.xy + xy * other.yy;
            out.yx = yx * other.xx + yy * other.yx;     out.yy = yx * other.xy + yy * other.yy;
#endif
            return out;
        }
        vec2_base<T> operator*(const vec2_base<T>& other) const
        {
            vec2_base<T> out;
#ifdef RQM_MATRIX_DOT_IMPL
            out.x = dot(row0, other);
            out.y = dot(row1, other);
#else
            out.x = xx * other.x + xy * other.y;
            out.y = yx * other.x + yy * other.y;
#endif
            return out;
        }
        std::vector<vec2_base<T>> operator*(const std::vector<vec2_base<T>>& other) const
        {
            return mat22_mul(*this, other);
        }
        // division doesn't make sense here
        mat22_base<T> operator+(const T& val) const { return { row0 + val, row1 + val }; }
        mat22_base<T> operator-(const T& val) const { return { row0 - val, row1 - val }; }
        mat22_base<T> operator*(const T& val) const { return { row0 * val, row1 * val }; }
        mat22_base<T> operator/(const T& val) const { return { row0 / val, row1 / val }; }

        mat22_base<T>& operator=(const mat22_base<T>& other) { row0 = other.row0; row1 = other.row1; return *this; }
        mat22_base<T>& operator+=(const mat22_base<T>& other) { row0 += other.row0; row1 += other.row1; return *this; }
        mat22_base<T>& operator-=(const mat22_base<T>& other) { row0 -= other.row0; row1 -= other.row1; return *this; }
        // *= and /= don't make sense here, *= specifically does not indicate if this is A * B or B * A
        mat22_base<T>& operator=(const T& val) { row0 = val; row1 = val; return *this; }
        mat22_base<T>& operator+=(const T& val) { row0 += val; row1 += val; return *this; }
        mat22_base<T>& operator-=(const T& val) { row0 -= val; row1 -= val; return *this; }
        mat22_base<T>& operator*=(const T& val) { row0 *= val; row1 *= val; return *this; }
        mat22_base<T>& operator/=(const T& val) { row0 /= val; row1 /= val; return *this; }

        bool operator==(const mat22_base<T>& other) { return row0 == other.row0 && row1 == other.row1; }
    };
    template <typename T>
    std::ostream& operator<<(std::ostream& os, const mat22_base<T>& op1) { os << '[' << op1.row0 << '|' << op1.row1 << ']'; return os; }
    template <typename T>
    inline mat22_base<T> operator+(const T& val, const mat22_base<T>& mat) { return mat + val; }
    template <typename T>
    inline mat22_base<T> operator-(const T& val, const mat22_base<T>& mat) { return -mat + val; }
    template <typename T>
    inline mat22_base<T> operator*(const T& val, const mat22_base<T>& mat) { return mat * val; }
    template <typename T>
    inline mat22_base<T> gen_mat22_identity()
    {
        return
        {
            1, 0,
            0, 1
        };
    }
    template <typename T>
    inline mat22_base<T> gen_mat22_from_cols(const vec2_base<T>& col0, const vec2_base<T>& col1)
    {
        return
        {
            col0.x, col1.x,
            col0.y, col1.y
        };
    }
    template <typename T>
    void mat22_mul_in_place(const mat22_base<T>& mat, vec2_base<T> vecs[], size_t numVecs) // returns mat * vecs into vecs
    {
        for(size_t i = 0; i < numVecs; i++)
        {
            vec2_base<T>& vec = vecs[i];
            vec2_base<T> v = vec;
#ifdef RQM_MATRIX_DOT_IMPL
            vec.x = dot(mat.row0, v);
            vec.y = dot(mat.row1, v);
#else
            vec.x = mat.xx * v.x + mat.xy * v.y;
            vec.y = mat.yx * v.x + mat.yy * v.y;
#endif
        }
    }
    template <typename T>
    inline void mat22_mul_in_place(const mat22_base<T>& mat, std::vector<vec2_base<T>>& vecs) // returns mat * vecs into vecs
    {
        mat22_mul_in_place(mat, vecs.data(), vecs.size());
    }
    template <typename T>
    std::vector<vec2_base<T>> mat22_mul(const mat22_base<T>& mat, const std::vector<vec2_base<T>>& vecs) // returns mat * vecs
    {
        // we don't use mat22_mul_in_place here because we want to take advantage of reserve + push_back, which is faster than resize + iterate
        size_t numVecs = vecs.size();
        std::vector<vec2_base<T>> out;
        out.reserve(numVecs);
        for(size_t i = 0; i < numVecs; i++)
        {
            const vec2_base<T>& vec = vecs[i];
#ifdef RQM_MATRIX_DOT_IMPL
            out.push_back({ dot(mat.row0, vec), dot(mat.row1, vec) });
#else
            out.push_back({ mat.xx * vec.x + mat.xy * vec.y, mat.yx * vec.x + mat.yy * vec.y });
#endif
        }
        return out;
    }


    template <typename T>
    union mat33_base
    {
        struct { vec3_base<T> x; vec3_base<T> y; vec3_base<T> z; };
        struct { vec3_base<T> row0; vec3_base<T> row1; vec3_base<T> row2; };
        struct { T xx; T xy; T xz; T yx; T yy; T yz; T zx; T zy; T zz; };
        T data[3][3]; // C++ is row major
        T flat_data[9];

        mat33_base() {}
        mat33_base(
            const T& val0, const T& val1, const T& val2,
            const T& val3, const T& val4, const T& val5,
            const T& val6, const T& val7, const T& val8)
            : xx(val0), xy(val1), xz(val2), yx(val3), yy(val4), yz(val5), zx(val6), zy(val7), zz(val8)
        {
        }
        mat33_base(const vec3_base<T>& vec0, const vec3_base<T>& vec1, const vec3_base<T>& vec2) : row0(vec0), row1(vec1), row2(vec2) {}
        mat33_base(const mat33_base<T>& mat) : row0(mat.row0), row1(mat.row1), row2(mat.row2) {}

        void set_columns(const vec3_base<T>& col0, const vec3_base<T>& col1, const vec3_base<T>& col2)
        {
            xx = col0.x; xy = col1.x; xz = col2.x;
            yx = col0.y; yy = col1.y; yz = col2.y;
            zx = col0.z; zy = col1.z; zz = col2.z;
        }

        void transpose()
        {
            mat33_base<T> mat = *this;
            set_columns(mat.row0, mat.row1, mat.row2);
        }
        mat33_base<T> get_transpose() const
        {
            return mat33_base<T>(xx, yx, zx,
                xy, yy, zy,
                xz, yz, zz);
        }

        // is orthogonal if all columns are orthonormal
        bool is_orthogonal() const
        {
            return *this * get_transpose() == mat33_base<T>(1, 0, 0,
                0, 1, 0,
                0, 0, 1); // test that transpose == inverse
        }

        T get_determinant() const { return xx * (yy * zz - yz * zy) - xy * (yx * zz - yz * zx) + xz * (yx * zy - yy * zx); }
        bool has_inverse() const { return !epsilon_equals<T>(get_determinant(), 0); }

        // finds inverse the hard way, use get_transpose() if you know the mat is orthogonal
        // TODO: potentially need to use Gaussian Elimination instead, idk
        mat33_base<T> get_inverse() const
        {
            T s = 1 / get_determinant();
            mat33_base<T> inv;
            inv.xx = s * (yy * zz - yz * zy);   inv.xy = s * (xz * zy - xy * zz);   inv.xz = s * (xy * yz - xz * yy);
            inv.yx = s * (yz * zx - yx * zz);   inv.yy = s * (xx * zz - xz * zx);   inv.yz = s * (xz * yx - xx * yz);
            inv.zx = s * (yx * zy - yy * zx);   inv.zy = s * (xy * zx - xx * zy);   inv.zz = s * (xx * yy - xy * yx);
            return inv; // TODO this needs testing!
        }
        void invert() { *this = get_inverse(); }

        mat33_base<T> operator+() const { return { row0, row1, row2 }; }
        mat33_base<T> operator-() const { return { -row0, -row1, -row2 }; }

        mat33_base<T> operator+(const mat33_base<T>& other) const { return { row0 + other.row0, row1 + other.row1, row2 + other.row2 }; }
        mat33_base<T> operator-(const mat33_base<T>& other) const { return { row0 - other.row0, row1 - other.row1, row2 - other.row2 }; }
        // NOT element-wise, this is normal matrix multiplication
        mat33_base<T> operator*(const mat33_base<T>& other) const
        {
            // TODO: test with benchmark and see if we need to implement strassen's multiplication
            mat33_base<T> out;
#ifdef RQM_MATRIX_DOT_IMPL
            mat33_base<T> t = other.get_transpose();
            out.xx = dot(row0, t.row0);     out.xy = dot(row0, t.row1);     out.xz = dot(row0, t.row2);
            out.yx = dot(row1, t.row0);     out.yy = dot(row1, t.row1);     out.yz = dot(row1, t.row2);
            out.zx = dot(row2, t.row0);     out.zy = dot(row2, t.row1);     out.zz = dot(row2, t.row2);
#else
            out.xx = xx * other.xx + xy * other.yx + xz * other.zx;     out.xy = xx * other.xy + xy * other.yy + xz * other.zy;     out.xz = xx * other.xz + xy * other.yz + xz * other.zz;
            out.yx = yx * other.xx + yy * other.yx + yz * other.zx;     out.yy = yx * other.xy + yy * other.yy + yz * other.zy;     out.yz = yx * other.xz + yy * other.yz + yz * other.zz;
            out.zx = zx * other.xx + zy * other.yx + zz * other.zx;     out.zy = zx * other.xy + zy * other.yy + zz * other.zy;     out.zz = zx * other.xz + zy * other.yz + zz * other.zz;
#endif
            return out;
        }
        vec3_base<T> operator*(const vec3_base<T>& other) const
        {
            vec3_base<T> out;
#ifdef RQM_MATRIX_DOT_IMPL
            out.x = dot(row0, other);
            out.y = dot(row1, other);
            out.z = dot(row2, other);
#else
            out.x = xx * other.x + xy * other.y + xz * other.z;
            out.y = yx * other.x + yy * other.y + yz * other.z;
            out.z = zx * other.x + zy * other.y + zz * other.z;
#endif
            return out;
        }
        std::vector<vec3_base<T>> operator*(const std::vector<vec3_base<T>>& other) const
        {
            return mat33_mul(*this, other);
        }
        // division doesn't make sense here
        mat33_base<T> operator+(const T& val) const { return { row0 + val, row1 + val, row2 + val }; }
        mat33_base<T> operator-(const T& val) const { return { row0 - val, row1 - val, row2 - val }; }
        mat33_base<T> operator*(const T& val) const { return { row0 * val, row1 * val, row2 * val }; }
        mat33_base<T> operator/(const T& val) const { return { row0 / val, row1 / val, row2 / val }; }

        mat33_base<T>& operator=(const mat33_base<T>& other) { row0 = other.row0; row1 = other.row1; row2 = other.row2; return *this; }
        mat33_base<T>& operator+=(const mat33_base<T>& other) { row0 += other.row0; row1 += other.row1; row2 += other.row2; return *this; }
        mat33_base<T>& operator-=(const mat33_base<T>& other) { row0 -= other.row0; row1 -= other.row1; row2 -= other.row2; return *this; }
        // *= and /= don't make sense here, *= specifically does not indicate if this is A * B or B * A
        mat33_base<T>& operator=(const T& val) { row0 = val; row1 = val; row2 = val; return *this; }
        mat33_base<T>& operator+=(const T& val) { row0 += val; row1 += val; row2 += val; return *this; }
        mat33_base<T>& operator-=(const T& val) { row0 -= val; row1 -= val; row2 -= val; return *this; }
        mat33_base<T>& operator*=(const T& val) { row0 *= val; row1 *= val; row2 *= val; return *this; }
        mat33_base<T>& operator/=(const T& val) { row0 /= val; row1 /= val; row2 /= val; return *this; }

        bool operator==(const mat33_base<T>& other) { return row0 == other.row0 && row1 == other.row1 && row2 == other.row2; }
    };
    template <typename T>
    std::ostream& operator<<(std::ostream& os, const mat33_base<T>& op1)
    {
        os << '[' << op1.row0 << '|' << op1.row1 << '|' << op1.row2 << ']'; return os;
    }
    template <typename T>
    inline mat33_base<T> operator+(const T& val, const mat33_base<T>& mat) { return mat + val; }
    template <typename T>
    inline mat33_base<T> operator-(const T& val, const mat33_base<T>& mat) { return -mat + val; }
    template <typename T>
    inline mat33_base<T> operator*(const T& val, const mat33_base<T>& mat) { return mat * val; }
    template <typename T>
    inline mat33_base<T> gen_mat33_identity()
    {
        return
        {
            1, 0, 0,
            0, 1, 0,
            0, 0, 1
        };
    }
    template <typename T>
    inline mat33_base<T> gen_mat33_from_cols(const vec3_base<T>& col0, const vec3_base<T>& col1, const vec3_base<T>& col2)
    {
        return
        {
            col0.x, col1.x, col2.x,
            col0.y, col1.y, col2.y,
            col0.z, col1.z, col2.z
        };
    }
    template <typename T>
    void mat33_mul_in_place(const mat33_base<T>& mat, vec3_base<T> vecs[], size_t numVecs) // returns mat * vecs into vecs
    {
        for(size_t i = 0; i < numVecs; i++)
        {
            vec3_base<T>& vec = vecs[i];
            vec3_base<T> v = vec;
#ifdef RQM_MATRIX_DOT_IMPL
            vec.x = dot(mat.row0, v);
            vec.y = dot(mat.row1, v);
            vec.z = dot(mat.row2, v);
#else
            vec.x = mat.xx * v.x + mat.xy * v.y + mat.xz * v.z;
            vec.y = mat.yx * v.x + mat.yy * v.y + mat.yz * v.z;
            vec.z = mat.zx * v.x + mat.zy * v.y + mat.zz * v.z;
#endif
        }
    }
    // works with a list of vec2s
    template <typename T>
    void mat33_mul_in_place(const mat33_base<T>& mat, vec2_base<T> vecs[], size_t numVecs, const T& zval) // returns mat * vecs into vecs
    {
        for(size_t i = 0; i < numVecs; i++)
        {
            vec2_base<T>& vec = vecs[i];
#ifdef RQM_MATRIX_DOT_IMPL
            vec3_base<T> v = { vec, zval };
            vec.x = dot(mat.row0, v);
            vec.y = dot(mat.row1, v);
#else
            vec2_base<T> v = vec;
            vec.x = mat.xx * v.x + mat.xy * v.y + mat.xz * zval;
            vec.y = mat.yx * v.x + mat.yy * v.y + mat.yz * zval;
#endif
        }
    }
    template <typename T>
    inline void mat33_mul_in_place(const mat33_base<T>& mat, std::vector<vec3_base<T>>& vecs) // returns mat * vecs into vecs
    {
        mat33_mul_in_place(mat, vecs.data(), vecs.size());
    }
    // works with a list of vec2s
    template <typename T>
    inline void mat33_mul_in_place(const mat33_base<T>& mat, std::vector<vec2_base<T>>& vecs, const T& zval) // returns mat * vecs into vecs
    {
        mat33_mul_in_place(mat, vecs.data(), vecs.size(), zval);
    }
    template <typename T>
    std::vector<vec3_base<T>> mat33_mul(const mat33_base<T>& mat, const std::vector<vec3_base<T>>& vecs) // returns mat * vecs
    {
        // we don't use mat33_mul_in_place here because we want to take advantage of reserve + push_back, which is faster than resize + iterate
        size_t numVecs = vecs.size();
        std::vector<vec3_base<T>> out;
        out.reserve(numVecs);
        for(size_t i = 0; i < numVecs; i++)
        {
            const vec3_base<T>& vec = vecs[i];
#ifdef RQM_MATRIX_DOT_IMPL
            out.push_back({ dot(mat.row0, vec), dot(mat.row1, vec), dot(mat.row2, vec) });
#else
            out.push_back({ mat.xx * vec.x + mat.xy * vec.y + mat.xz * vec.z,
                            mat.yx * vec.x + mat.yy * vec.y + mat.yz * vec.z,
                            mat.zx * vec.x + mat.zy * vec.y + mat.zz * vec.z });
#endif
        }
        return out;
    }
    // works with a list of vec2s
    template <typename T>
    std::vector<vec2_base<T>> mat33_mul(const mat33_base<T>& mat, const std::vector<vec2_base<T>>& vecs, const T& zval) // returns mat * vecs
    {
        // we don't use mat33_mul_in_place here because we want to take advantage of reserve + push_back, which is faster than resize + iterate
        size_t numVecs = vecs.size();
        std::vector<vec2_base<T>> out;
        out.reserve(numVecs);
        for(size_t i = 0; i < numVecs; i++)
        {
            const vec2_base<T>& vec = vecs[i];
            out.push_back({ mat.xx * vec.x + mat.xy * vec.y + mat.xz * zval,
                            mat.yx * vec.x + mat.yy * vec.y + mat.yz * zval });
        }
        return out;
    }


    typedef mat22_base<float> mat22;
    typedef mat33_base<float> mat33;

    typedef mat22_base<double> dmat22;
    typedef mat33_base<double> dmat33;
}