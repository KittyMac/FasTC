/* FasTC
 * Copyright (c) 2014 University of North Carolina at Chapel Hill.
 * All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for educational, research, and non-profit purposes, without
 * fee, and without a written agreement is hereby granted, provided that the
 * above copyright notice, this paragraph, and the following four paragraphs
 * appear in all copies.
 *
 * Permission to incorporate this software into commercial products may be
 * obtained by contacting the authors or the Office of Technology Development
 * at the University of North Carolina at Chapel Hill <otd@unc.edu>.
 *
 * This software program and documentation are copyrighted by the University of
 * North Carolina at Chapel Hill. The software program and documentation are
 * supplied "as is," without any accompanying services from the University of
 * North Carolina at Chapel Hill or the authors. The University of North
 * Carolina at Chapel Hill and the authors do not warrant that the operation of
 * the program will be uninterrupted or error-free. The end-user understands
 * that the program was developed for research purposes and is advised not to
 * rely exclusively on the program for any reason.
 *
 * IN NO EVENT SHALL THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL OR THE
 * AUTHORS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING OUT OF THE USE OF
 * THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF NORTH CAROLINA
 * AT CHAPEL HILL OR THE AUTHORS HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL AND THE AUTHORS SPECIFICALLY
 * DISCLAIM ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE AND ANY 
 * STATUTORY WARRANTY OF NON-INFRINGEMENT. THE SOFTWARE PROVIDED HEREUNDER IS ON
 * AN "AS IS" BASIS, AND THE UNIVERSITY  OF NORTH CAROLINA AT CHAPEL HILL AND
 * THE AUTHORS HAVE NO OBLIGATIONS TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, 
 * ENHANCEMENTS, OR MODIFICATIONS.
 *
 * Please send all BUG REPORTS to <pavel@cs.unc.edu>.
 *
 * The authors may be contacted via:
 *
 * Pavel Krajcevski
 * Dept of Computer Science
 * 201 S Columbia St
 * Frederick P. Brooks, Jr. Computer Science Bldg
 * Chapel Hill, NC 27599-3175
 * USA
 * 
 * <http://gamma.cs.unc.edu/FasTC/>
 */

#include "gtest/gtest.h"
#include "VectorBase.h"

static const float kEpsilon = 1e-6;

TEST(VectorBase, Constructors) {
  FasTC::VectorBase<float, 3> v3f;
  FasTC::VectorBase<double, 1> v1d;
  FasTC::VectorBase<int, 7> v7i;
  FasTC::VectorBase<unsigned, 16> v16u;

#define TEST_VECTOR_COPY_CONS(v, t, n)          \
  do {                                          \
    FasTC::VectorBase<t, n> d##v (v);           \
    for(int i = 0; i < n; i++) {                \
      EXPECT_EQ(d##v [i], v[i]);                \
    }                                           \
  } while(0)                                    \

  TEST_VECTOR_COPY_CONS(v3f, float, 3);
  TEST_VECTOR_COPY_CONS(v1d, double, 1);
  TEST_VECTOR_COPY_CONS(v7i, int, 7);
  TEST_VECTOR_COPY_CONS(v16u, unsigned, 16);

#undef TEST_VECTOR_COPY_CONS
}

TEST(VectorBase, Accessors) {
  FasTC::VectorBase<float, 3> v3f;
  v3f[0] = 1.0f;
  v3f[1] = -2.3f;
  v3f[2] = 1000;

  for(int i = 0; i < 3; i++) {
    EXPECT_EQ(v3f[i], v3f(i));
  }

  v3f(0) = -1.0f;
  v3f(1) = 2.3f;
  v3f(2) = -1000;

  for(int i = 0; i < 3; i++) {
    EXPECT_EQ(v3f(i), v3f[i]);
  }
}

TEST(VectorBase, PointerConversion) {
  FasTC::VectorBase<float, 3> v3f;
  v3f[0] = 1.0f;
  v3f[1] = -2.3f;
  v3f[2] = 1000;

  float cmp[3] = { 1.0f, -2.3f, 1000 };
  const float *v3fp = v3f;
  int result = memcmp(cmp, v3fp, 3 * sizeof(float));
  EXPECT_EQ(result, 0);

  cmp[0] = -1.0f;
  cmp[1] = 2.3f;
  cmp[2] = 1000.0f;
  v3f = cmp;
  for(int i = 0; i < 3; i++) {
    EXPECT_EQ(v3f[i], cmp[i]);
  }
}

TEST(VectorBase, CastVector) {
  FasTC::VectorBase<float, 3> v3f;
  FasTC::VectorBase<double, 3> v3d = v3f;
  FasTC::VectorBase<int, 3> v3i = v3f;
  for(int i = 0; i < 3; i++) {
    EXPECT_EQ(v3d(i), static_cast<double>(v3f(i)));
    EXPECT_EQ(v3i(i), static_cast<int>(v3f(i)));
  }
}

TEST(VectorBase, DotProduct) {
  int iv[5] = { -2, -1, 0, 1, 2 };
  FasTC::VectorBase<int, 5> v5i(iv);

  unsigned uv[5] = { 1, 2, 3, 4, 5 };
  FasTC::VectorBase<unsigned, 5> v5u(uv);

  EXPECT_EQ(v5i.Dot(v5u), 10);
  EXPECT_EQ(v5u.Dot(v5i), 10);
}

TEST(VectorBase, Length) {
  int iv[5] = { 1, 2, 3, 4, 5 };
  FasTC::VectorBase<int, 5> v5i (iv);

  EXPECT_EQ(v5i.LengthSq(), 55);
  EXPECT_EQ(v5i.Length(), 7);

  float fv[6] = {1, 2, 3, 4, 5, 6};
  FasTC::VectorBase<float, 6> v6f (fv);
  
  EXPECT_EQ(v6f.LengthSq(), 91);
  EXPECT_NEAR(v6f.Length(), sqrt(91.0f), kEpsilon);
}

TEST(VectorBase, Normalization) {
  float fv[2] = {1, 0};
  FasTC::VectorBase<float, 2> v2f (fv);
  v2f.Normalize();
  EXPECT_EQ(v2f[0], 1);
  EXPECT_EQ(v2f[1], 0);

  // Normalized vector should be sqrt(2) for each axis, although
  // this can't be represented as integers...
  unsigned uv[2] = {2, 2};
  FasTC::VectorBase<unsigned, 2> v2u (uv);
  v2u.Normalize();
  EXPECT_EQ(v2u[0], 1);
  EXPECT_EQ(v2u[1], 1);

  const float sqrt2 = sqrt(2)/2.0f;
  for(int i = 2; i < 10; i++) {
    v2f[0] = static_cast<float>(i);
    v2f[1] = static_cast<float>(i);
    v2f.Normalize();
    EXPECT_NEAR(v2f[0], sqrt2, kEpsilon);
    EXPECT_NEAR(v2f[1], sqrt2, kEpsilon);
  }
}

TEST(VectorBase, Scaling) {
  float fv[2] = {1.0f, 3.0f};
  FasTC::VectorBase<float, 2> v2f (fv);
  FasTC::VectorBase<float, 2> v2fd = v2f * 3.0f;
  EXPECT_NEAR(v2fd[0], 3.0f, kEpsilon);
  EXPECT_NEAR(v2fd[1], 9.0f, kEpsilon);

  v2fd = -1.0 * v2f;
  EXPECT_NEAR(v2fd[0], -1.0f, kEpsilon);
  EXPECT_NEAR(v2fd[1], -3.0f, kEpsilon);

  v2fd = v2f / 3;
  EXPECT_NEAR(v2fd[0], 1.0f / 3.0f, kEpsilon);
  EXPECT_NEAR(v2fd[1], 1.0f, kEpsilon);
}

TEST(VectorBase, Addition) {
  float fv[2] = {1.1f, 3.2f};
  FasTC::VectorBase<float, 2> v2f (fv);

  unsigned uv[2] = {5, 2};
  FasTC::VectorBase<unsigned, 2> v2u (uv);

  FasTC::VectorBase<unsigned, 2> au = v2u + v2f;
  EXPECT_EQ(au[0], 6);
  EXPECT_EQ(au[1], 5);

  FasTC::VectorBase<float, 2> af = v2f + v2u;
  EXPECT_NEAR(af[0], 6.1f, kEpsilon);
  EXPECT_NEAR(af[1], 5.2f, kEpsilon);

  au = v2u - v2f;
  EXPECT_EQ(au[0], 3);
  EXPECT_EQ(au[1], 0);

  af = v2f - v2u;
  EXPECT_NEAR(af[0], -3.9f, kEpsilon);
  EXPECT_NEAR(af[1], 1.2f, kEpsilon);  
}
