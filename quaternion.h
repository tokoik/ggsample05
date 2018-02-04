//
// 軸と回転角から単位四元数を求める
//
//   q ← 軸 (x, y, z) 角度 (a)
//
extern void qmake(float *q, float x, float y, float z, float a);

//
// 四元数の積を求める
//
//   p ← q * r
//
extern void qmul(float *p, const float *q, const float *r);

//
// 四元数から回転変換行列を求める
//
//   m ← 四元数 q
//
extern void qrot(GLfloat *m, const float *q);

//
// 球面線形補間
//
//   p ← 四元数 q と 四元数 r を t で補間する
//
extern void slerp(float *p, const float *q, const float *r, float t);

//
// 複数のクォータニオン間の球面線形補間（折れ線）
//
//   p ← t[i] におけるクォータニオン q[i], 0 <= i < n に対する u における補間値
//
extern void mslerp(float *p, const float *t, const float (*q)[4], int n, float u);
