#include <cmath>

#include "gg.h"
using namespace gg;

#include "quaternion.h"

//
// 軸と回転角から単位四元数を求める
//
//   q ← 軸 (x, y, z) 角度 (a)
//
void qmake(float *q, float x, float y, float z, float a)
{
  float n = x * x + y * y + z * z;
  
  if (n != 0.0)
  {
    float s = sin(a *= 0.5f) / sqrt(n);
    
    q[0] = x * s;
    q[1] = y * s;
    q[2] = z * s;
    q[3] = cos(a);
  }
}

//
// 四元数の積を求める
//
//   p ← q * r
//
void qmul(float *p, const float *q, const float *r)
{
  p[0] = q[1]*r[2] - q[2]*r[1] + r[3]*q[0] + q[3]*r[0];
  p[1] = q[2]*r[0] - q[0]*r[2] + r[3]*q[1] + q[3]*r[1];
  p[2] = q[0]*r[1] - q[1]*r[0] + r[3]*q[2] + q[3]*r[2];
  p[3] = q[3]*r[3] - q[0]*r[0] - q[1]*r[1] - q[2]*r[2];
}

//
// 四元数から回転変換行列を求める
//
//   m ← 四元数 q
//
void qrot(GLfloat *m, const float *q)
{
  float xx = q[0] * q[0] * 2.0f;
  float yy = q[1] * q[1] * 2.0f;
  float zz = q[2] * q[2] * 2.0f;
  float xy = q[0] * q[1] * 2.0f;
  float yz = q[1] * q[2] * 2.0f;
  float zx = q[2] * q[0] * 2.0f;
  float xw = q[0] * q[3] * 2.0f;
  float yw = q[1] * q[3] * 2.0f;
  float zw = q[2] * q[3] * 2.0f;
  
  m[ 0] = 1.0f - yy - zz;
  m[ 1] = xy + zw;
  m[ 2] = zx - yw;
  m[ 4] = xy - zw;
  m[ 5] = 1.0f - zz - xx;
  m[ 6] = yz + xw;
  m[ 8] = zx + yw;
  m[ 9] = yz - xw;
  m[10] = 1.0f - xx - yy;
  m[ 3] = m[ 7] = m[11] =
  m[12] = m[13] = m[14] = 0.0f;
  m[15] = 1.0f;
}

//
// 球面線形補間
//
//   p ← 四元数 q と 四元数 r を t で補間する
//
void slerp(float *p, const float *q, const float *r, float t)
{
  float qr = q[0] * r[0] + q[1] * r[1] + q[2] * r[2] + q[3] * r[3];
  float ss = 1.0f - qr * qr;
  
  if (ss == 0.0)
  {
    p[0] = q[0];
    p[1] = q[1];
    p[2] = q[2];
    p[3] = q[3];
  }
  else
  {
    float sp = sqrt(ss);
    float ph = acos(qr);
    float pt = ph * t;
    float t1 = sin(pt) / sp;
    float t0 = sin(ph - pt) / sp;
    
    p[0] = q[0] * t0 + r[0] * t1;
    p[1] = q[1] * t0 + r[1] * t1;
    p[2] = q[2] * t0 + r[2] * t1;
    p[3] = q[3] * t0 + r[3] * t1;
  }
}

//
// 複数のクォータニオン間の球面線形補間（折れ線）
//
//   p ← t[i] におけるクォータニオン q[i], 0 <= i < n に対する u における補間値
//
void mslerp(float *p, const float *t, const float (*q)[4], int n, float u)
{
  int i = 0, j = n - 1;
  
  // u を含む t の区間 [t[i], t[i+1]) を二分法で求める
  while (i < j)
  {
    int k = (i + j) / 2;
    if (t[k] < u)
      i = k + 1;
    else
      j = k;
  }
  if (i > 0) --i;
  
  slerp(p, q[i], q[i + 1], (u - t[i]) / (t[i + 1] - t[i]));
}
