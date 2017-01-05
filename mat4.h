#ifndef MAT4_H
#define MAT4_H

namespace graphix {
namespace engine {
namespace math {

class Mat4 {
public:
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

    void translate(float x, float y, float z) {
        m[0][3] += x;
        m[1][3] += y;
        m[2][3] += z;
    }

    const float* data_pointer() {
        return &m[0][0];
    }

private:
    float m[4][4];
};

} /* math */
} /* engine */
} /* graphix */

#endif /* MAT4_H */
