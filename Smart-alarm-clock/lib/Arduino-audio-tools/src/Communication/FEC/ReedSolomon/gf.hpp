/* Author: Mike Lubinets (aka mersinvald)
 * Date: 29.12.15
 *
 * See LICENSE */

#ifndef GF_H
#define GF_H
#include <stdint.h>
#include <string.h>
#include "poly.hpp"

#if !defined DEBUG && !defined __CC_ARM
#include <assert.h>
#else
#define assert(dummy)
#endif

/// @brief AudioTools internal: Reed-Solomon
namespace RS {

namespace gf {


/* GF tables pre-calculated for 0x11d primitive polynomial */

const uint8_t exp[255] = {
    0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80, 0x1d, 0x3a, 0x74, 0xe8, 0xcd, 0x87, 0x13, 0x26, 0x4c,
    0x98, 0x2d, 0x5a, 0xb4, 0x75, 0xea, 0xc9, 0x8f, 0x3, 0x6, 0xc, 0x18, 0x30, 0x60, 0xc0, 0x9d,
    0x27, 0x4e, 0x9c, 0x25, 0x4a, 0x94, 0x35, 0x6a, 0xd4, 0xb5, 0x77, 0xee, 0xc1, 0x9f, 0x23, 0x46,
    0x8c, 0x5, 0xa, 0x14, 0x28, 0x50, 0xa0, 0x5d, 0xba, 0x69, 0xd2, 0xb9, 0x6f, 0xde, 0xa1, 0x5f,
    0xbe, 0x61, 0xc2, 0x99, 0x2f, 0x5e, 0xbc, 0x65, 0xca, 0x89, 0xf, 0x1e, 0x3c, 0x78, 0xf0, 0xfd,
    0xe7, 0xd3, 0xbb, 0x6b, 0xd6, 0xb1, 0x7f, 0xfe, 0xe1, 0xdf, 0xa3, 0x5b, 0xb6, 0x71, 0xe2, 0xd9,
    0xaf, 0x43, 0x86, 0x11, 0x22, 0x44, 0x88, 0xd, 0x1a, 0x34, 0x68, 0xd0, 0xbd, 0x67, 0xce, 0x81,
    0x1f, 0x3e, 0x7c, 0xf8, 0xed, 0xc7, 0x93, 0x3b, 0x76, 0xec, 0xc5, 0x97, 0x33, 0x66, 0xcc, 0x85,
    0x17, 0x2e, 0x5c, 0xb8, 0x6d, 0xda, 0xa9, 0x4f, 0x9e, 0x21, 0x42, 0x84, 0x15, 0x2a, 0x54, 0xa8,
    0x4d, 0x9a, 0x29, 0x52, 0xa4, 0x55, 0xaa, 0x49, 0x92, 0x39, 0x72, 0xe4, 0xd5, 0xb7, 0x73, 0xe6,
    0xd1, 0xbf, 0x63, 0xc6, 0x91, 0x3f, 0x7e, 0xfc, 0xe5, 0xd7, 0xb3, 0x7b, 0xf6, 0xf1, 0xff, 0xe3,
    0xdb, 0xab, 0x4b, 0x96, 0x31, 0x62, 0xc4, 0x95, 0x37, 0x6e, 0xdc, 0xa5, 0x57, 0xae, 0x41, 0x82,
    0x19, 0x32, 0x64, 0xc8, 0x8d, 0x7, 0xe, 0x1c, 0x38, 0x70, 0xe0, 0xdd, 0xa7, 0x53, 0xa6, 0x51,
    0xa2, 0x59, 0xb2, 0x79, 0xf2, 0xf9, 0xef, 0xc3, 0x9b, 0x2b, 0x56, 0xac, 0x45, 0x8a, 0x9, 0x12,
    0x24, 0x48, 0x90, 0x3d, 0x7a, 0xf4, 0xf5, 0xf7, 0xf3, 0xfb, 0xeb, 0xcb, 0x8b, 0xb, 0x16, 0x2c,
    0x58, 0xb0, 0x7d, 0xfa, 0xe9, 0xcf, 0x83, 0x1b, 0x36, 0x6c, 0xd8, 0xad, 0x47, 0x8e
};

const uint8_t log[256] = {
    0x0, 0x0, 0x1, 0x19, 0x2, 0x32, 0x1a, 0xc6, 0x3, 0xdf, 0x33, 0xee, 0x1b, 0x68, 0xc7, 0x4b, 0x4,
    0x64, 0xe0, 0xe, 0x34, 0x8d, 0xef, 0x81, 0x1c, 0xc1, 0x69, 0xf8, 0xc8, 0x8, 0x4c, 0x71, 0x5,
    0x8a, 0x65, 0x2f, 0xe1, 0x24, 0xf, 0x21, 0x35, 0x93, 0x8e, 0xda, 0xf0, 0x12, 0x82, 0x45, 0x1d,
    0xb5, 0xc2, 0x7d, 0x6a, 0x27, 0xf9, 0xb9, 0xc9, 0x9a, 0x9, 0x78, 0x4d, 0xe4, 0x72, 0xa6, 0x6,
    0xbf, 0x8b, 0x62, 0x66, 0xdd, 0x30, 0xfd, 0xe2, 0x98, 0x25, 0xb3, 0x10, 0x91, 0x22, 0x88, 0x36,
    0xd0, 0x94, 0xce, 0x8f, 0x96, 0xdb, 0xbd, 0xf1, 0xd2, 0x13, 0x5c, 0x83, 0x38, 0x46, 0x40, 0x1e,
    0x42, 0xb6, 0xa3, 0xc3, 0x48, 0x7e, 0x6e, 0x6b, 0x3a, 0x28, 0x54, 0xfa, 0x85, 0xba, 0x3d, 0xca,
    0x5e, 0x9b, 0x9f, 0xa, 0x15, 0x79, 0x2b, 0x4e, 0xd4, 0xe5, 0xac, 0x73, 0xf3, 0xa7, 0x57, 0x7,
    0x70, 0xc0, 0xf7, 0x8c, 0x80, 0x63, 0xd, 0x67, 0x4a, 0xde, 0xed, 0x31, 0xc5, 0xfe, 0x18, 0xe3,
    0xa5, 0x99, 0x77, 0x26, 0xb8, 0xb4, 0x7c, 0x11, 0x44, 0x92, 0xd9, 0x23, 0x20, 0x89, 0x2e, 0x37,
    0x3f, 0xd1, 0x5b, 0x95, 0xbc, 0xcf, 0xcd, 0x90, 0x87, 0x97, 0xb2, 0xdc, 0xfc, 0xbe, 0x61, 0xf2,
    0x56, 0xd3, 0xab, 0x14, 0x2a, 0x5d, 0x9e, 0x84, 0x3c, 0x39, 0x53, 0x47, 0x6d, 0x41, 0xa2, 0x1f,
    0x2d, 0x43, 0xd8, 0xb7, 0x7b, 0xa4, 0x76, 0xc4, 0x17, 0x49, 0xec, 0x7f, 0xc, 0x6f, 0xf6, 0x6c,
    0xa1, 0x3b, 0x52, 0x29, 0x9d, 0x55, 0xaa, 0xfb, 0x60, 0x86, 0xb1, 0xbb, 0xcc, 0x3e, 0x5a, 0xcb,
    0x59, 0x5f, 0xb0, 0x9c, 0xa9, 0xa0, 0x51, 0xb, 0xf5, 0x16, 0xeb, 0x7a, 0x75, 0x2c, 0xd7, 0x4f,
    0xae, 0xd5, 0xe9, 0xe6, 0xe7, 0xad, 0xe8, 0x74, 0xd6, 0xf4, 0xea, 0xa8, 0x50, 0x58, 0xaf
};



/* ################################
 * # OPERATIONS OVER GALOIS FIELDS #
 * ################################ */

/* @brief Addition in Galois Fields
 * @param x - left operand
 * @param y - right operand
 * @return x + y */
inline uint8_t add(uint8_t x, uint8_t y) {
    return x^y;
}

/* ##### GF subtraction ###### */
/* @brief Subtraction in Galois Fields
 * @param x - left operand
 * @param y - right operand
 * @return x - y */
inline uint8_t sub(uint8_t x, uint8_t y) {
    return x^y;
}

/* @brief Multiplication in Galois Fields
 * @param x - left operand
 * @param y - right operand
 * @return x * y */
inline uint8_t mul(uint16_t x, uint16_t y){
    if (x == 0 || y == 0)
        return 0;
    return exp[(log[x] + log[y]) % 255];
}

/* @brief Division in Galois Fields
 * @param x - dividend
 * @param y - divisor
 * @return x / y */
inline uint8_t div(uint8_t x, uint8_t y){
    assert(y != 0);
    if(x == 0) return 0;
    return exp[(log[x] + 255 - log[y]) % 255];
}

/* @brief X in power Y w
 * @param x     - operand
 * @param power - power
 * @return x^power */
inline uint8_t pow(uint8_t x, intmax_t power){
    intmax_t i = log[x];
    i *= power;
    i %= 255;
    if(i < 0) i = i + 255;
    return exp[i % 255];
}

/* @brief Inversion in Galois Fields
 * @param x - number
 * @return inversion of x */
inline uint8_t inverse(uint8_t x){
    return exp[(255 - log[x]) % 255]; /* == div(1, x); */
}

/* ##########################
 * # POLYNOMIALS OPERATIONS #
 * ########################## */

/* @brief Multiplication polynomial by scalar
 * @param &p    - source polynomial
 * @param &newp - destination polynomial
 * @param x     - scalar */
inline void
poly_scale(const Poly *p, Poly *newp, uint16_t x) {
    newp->length = p->length;
    for(uint16_t i = 0; i < p->length; i++){
        newp->at(i) = mul(p->at(i), x);
    }
}

/* @brief Addition of two polynomials
 * @param &p    - right operand polynomial
 * @param &q    - left operand polynomial
 * @param &newp - destination polynomial */
inline void
poly_add(const Poly *p, const Poly *q, Poly *newp) {
    newp->length = poly_max(p->length, q->length);
    memset(newp->ptr(), 0, newp->length * sizeof(uint8_t));

    for(uint8_t i = 0; i < p->length; i++){
        newp->at(i + newp->length - p->length) = p->at(i);
    }

    for(uint8_t i = 0; i < q->length; i++){
        newp->at(i + newp->length - q->length) ^= q->at(i);
    }
}


/* @brief Multiplication of two polynomials
 * @param &p    - right operand polynomial
 * @param &q    - left operand polynomial
 * @param &newp - destination polynomial */
inline void
poly_mul(const Poly *p, const Poly *q, Poly *newp) {
    newp->length = p->length + q->length - 1;
    memset(newp->ptr(), 0, newp->length * sizeof(uint8_t));
    /* Compute the polynomial multiplication (just like the outer product of two vectors,
     * we multiply each coefficients of p with all coefficients of q) */
    for(uint8_t j = 0; j < q->length; j++){
        for(uint8_t i = 0; i < p->length; i++){
            newp->at(i+j) ^= mul(p->at(i), q->at(j)); /* == r[i + j] = gf_add(r[i+j], gf_mul(p[i], q[j])) */
        }
    }
}

/* @brief Division of two polynomials
 * @param &p    - right operand polynomial
 * @param &q    - left operand polynomial
 * @param &newp - destination polynomial */
inline void
poly_div(const Poly *p, const Poly *q, Poly *newp) {
    if(p->ptr() != newp->ptr()) {
        memcpy(newp->ptr(), p->ptr(), p->length*sizeof(uint8_t));
    }

    newp->length = p->length;

    uint8_t coef;

    for(int i = 0; i < (p->length-(q->length-1)); i++){
        coef = newp->at(i);
        if(coef != 0){
            for(uint8_t j = 1; j < q->length; j++){
                if(q->at(j) != 0)
                    newp->at(i+j) ^= mul(q->at(j), coef);
            }
        }
    }

    size_t sep = p->length-(q->length-1);
    memmove(newp->ptr(), newp->ptr()+sep, (newp->length-sep) * sizeof(uint8_t));
    newp->length = newp->length-sep;
}

/* @brief Evaluation of polynomial in x
 * @param &p - polynomial to evaluate
 * @param x  - evaluation point */
inline int8_t
poly_eval(const Poly *p, uint16_t x) {
    uint8_t y = p->at(0);
    for(uint8_t i = 1; i < p->length; i++){
        y = mul(y, x) ^ p->at(i);
    }
    return y;
}

} /* end of gf namespace */

}
#endif // GF_H

