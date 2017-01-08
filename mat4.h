#ifndef MAT4_H
#define MAT4_H

#include <cmath>
#include <ostream>

namespace graphix {
namespace engine {
namespace math {

class Mat4 {
    friend std::ostream& operator<<(std::ostream& out, const Mat4& mat);

public:
    Mat4(float a, float b, float c, float d,
            float e, float f, float g, float h,
            float i, float j, float k, float l,
            float q, float n, float o, float p) {

        m[0][0] = a; m[0][1] = b; m[0][2] = c; m[0][3] = d;
        m[1][0] = e; m[1][1] = f; m[1][2] = g; m[1][3] = h;
        m[2][0] = i; m[2][1] = j; m[2][2] = k; m[2][3] = l;
        m[3][0] = q; m[3][1] = n; m[3][2] = o; m[3][3] = p;
    }

    Mat4() {
        to_identity_matrix();
    }

    void to_identity_matrix() {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                if (i == j) {
                    m[i][j] = 1.0f;
                } else {
                    m[i][j] = 0.0f;
                }
            }
        }
    }

    void to_null_matrix() {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                m[i][j] = 0.0f;
            }
        }
    }

    void translate(float x, float y, float z) {
        m[0][3] += x;
        m[1][3] += y;
        m[2][3] += z;
    }

    void rotate_x_axis(float degree) {
        Mat4 rotation_x_mat;
        rotation_x_mat.m[1][1] = std::cos(degree);
        rotation_x_mat.m[1][2] = -std::sin(degree);
        rotation_x_mat.m[2][1] = std::sin(degree);
        rotation_x_mat.m[2][2] = std::cos(degree);

        rotation_x_mat *= *this;
        *this = rotation_x_mat;
    }

    //TODO: check if this one is correct
    void rotate_y_axis(float degree) {
        Mat4 rotation_y_mat;
        rotation_y_mat.m[0][0] = std::cos(degree);
        rotation_y_mat.m[0][2] = -std::sin(degree);
        rotation_y_mat.m[2][0] = std::sin(degree);
        rotation_y_mat.m[2][2] = std::cos(degree);

        rotation_y_mat *= *this;
        *this = rotation_y_mat;
    }

    void rotate_z_axis(float degree) {
        Mat4 rotation_z_mat;
        rotation_z_mat.m[0][0] = std::cos(degree);
        rotation_z_mat.m[0][1] = -std::sin(degree);
        rotation_z_mat.m[1][0] = std::sin(degree);
        rotation_z_mat.m[1][1] = std::cos(degree);

        rotation_z_mat *= *this;
        *this = rotation_z_mat;
    }

    Mat4& operator=(const Mat4& other) {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                m[i][j] = other.m[i][j];
            }
        }
        return *this;
    }

    Mat4& operator*=(const Mat4& other) {
        Mat4 result;
        result.to_null_matrix();

        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                for (int k = 0; k < 4; ++k) {
                    result.m[i][j] += m[i][k] * other.m[k][j];
                }
            }
        }

        return (*this = result);
    }

private:
    float m[4][4];
};

std::ostream& operator<<(std::ostream& out, const Mat4& mat) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            out << mat.m[i][j] << " ";
        }
        out << std::endl;
    }
    return out;
}

} /* math */
} /* engine */
} /* graphix */

#endif /* MAT4_H */
