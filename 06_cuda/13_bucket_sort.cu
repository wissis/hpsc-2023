#include <cstdio>
#include <cstdlib>
#include <vector>

//const int n = 150;
const int n = 50;
//const int range = 10;
const int range = 5;

__device__ __managed__ int bucket[range];
__device__ __managed__ int end = 0;

__global__ void bucket_add(int *bucket, int *key){
  int index = blockIdx.x * blockDim.x + threadIdx.x;
  if(index >= n) return;
  int buffer = key[index];
  atomicAdd(&bucket[buffer], 1);
}

__global__ void bucket_remove(int *bucket, int *key, int i, int end){
  int index = blockIdx.x * blockDim.x + threadIdx.x;
  if(index >= n + end) return;
  int bucketindex = i;
//  printf("%d %d\n", bucketindex, index);
  key[index + end] = bucketindex;
//  if(index == 0) printf("key: %d\n", key[index+end]);
}

int main() {
//  int n = 50;
//  int range = 5;
//  std::vector<int> key(n);
  int *key;
  cudaMallocManaged(&key, n*sizeof(int));
  for (int i=0; i<n; i++) {
    key[i] = rand() % range;
    printf("%d ",key[i]);
  }
  printf("\n");

//  const int N = 128;
  const int M = 32;
//  std::vector<int> bucket(range);
  int *bucket;
  cudaMallocManaged(&bucket, range*sizeof(int));
  bucket_add<<<(n+M-1)/M, M>>>(bucket, key);
  cudaDeviceSynchronize();
//  for (int i=0; i<range; i++) printf("%d ", bucket[i]);
  for (int i=0; i<range; i++){
//    printf("LOOP %d \n", i);
    bucket_remove<<<(n+M-1)/M, M>>>(bucket, key, i, end);
    end = end + bucket[i];
    cudaDeviceSynchronize();
  }
 // cudaDeviceSynchronize();

/*
  std::vector<int> bucket(range); 
  for (int i=0; i<range; i++) {
    bucket[i] = 0;
  }
  for (int i=0; i<n; i++) {
    bucket[key[i]]++;
  }
  for (int i=0, j=0; i<range; i++) {
    for (; bucket[i]>0; bucket[i]--) {
      key[j++] = i;
    }
  }
*/
  for (int i=0; i<n; i++) {
    printf("%d ",key[i]);
  }
  printf("\n");
  cudaFree(bucket);
  cudaFree(key);
}
