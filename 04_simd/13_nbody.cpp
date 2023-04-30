#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <immintrin.h>

int main() {
  const int N = 8;
  float x[N], y[N], m[N], fx[N], fy[N];
  for(int i=0; i<N; i++) {
    x[i] = drand48();
    y[i] = drand48();
    m[i] = drand48();
    fx[i] = fy[i] = 0;
  }
  float storage[N];
  for(int i=0; i<N; i++) {
      /*Coordinate calculation*/
      __m256 xivec = _mm256_set1_ps(x[i]);
      __m256 xjvec = _mm256_load_ps(x);
      __m256 yivec = _mm256_set1_ps(y[i]);
      __m256 yjvec = _mm256_load_ps(y);
      __m256 rxvec = _mm256_sub_ps(xivec, xjvec);
      __m256 ryvec = _mm256_sub_ps(yivec, yjvec);
      __m256 reci_rvec  = _mm256_blendv_ps(rxvec, _mm256_rsqrt_ps(_mm256_add_ps(_mm256_mul_ps(rxvec, rxvec), _mm256_mul_ps(ryvec, ryvec))), rxvec); // masking
      /*Force calculation*/
      __m256 mvec = _mm256_load_ps(m);
      __m256 fxvec = _mm256_mul_ps(_mm256_mul_ps(_mm256_mul_ps(_mm256_mul_ps(_mm256_mul_ps( rxvec, mvec ), reci_rvec), reci_rvec), reci_rvec), _mm256_set1_ps(-1));
      __m256 fyvec = _mm256_mul_ps(_mm256_mul_ps(_mm256_mul_ps(_mm256_mul_ps(_mm256_mul_ps( ryvec, mvec ), reci_rvec), reci_rvec), reci_rvec), _mm256_set1_ps(-1));
      /*summation*/
      __m256 temp = _mm256_permute2f128_ps(fxvec, fxvec, 1);
      temp = _mm256_add_ps(temp, fxvec);
      temp = _mm256_hadd_ps(temp, temp);
      temp = _mm256_hadd_ps(temp, temp);
      _mm256_store_ps(storage, temp);
      fx[i] = storage[0];
      temp = _mm256_permute2f128_ps(fyvec, fyvec, 1);
      temp = _mm256_add_ps(temp, fyvec);
      temp = _mm256_hadd_ps(temp, temp);
      temp = _mm256_hadd_ps(temp, temp);
      _mm256_store_ps(storage, temp);
      fy[i] = storage[0];
/*      
      if(i != j) {
        float rx = x[i] - x[j];
        float ry = y[i] - y[j];
        float r = std::sqrt(rx * rx + ry * ry);
        fx[i] -= rx * m[j] / (r * r * r);
        fy[i] -= ry * m[j] / (r * r * r);
      }
    }
*/
    printf("%d %g %g\n",i,fx[i],fy[i]);
  }
}
