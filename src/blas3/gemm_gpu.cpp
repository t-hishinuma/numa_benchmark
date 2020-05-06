#include <cuda_runtime.h>
#include "cublas_v2.h"
#include <iostream>
#include <vector>
#include<omp.h>

#define DFUNC_NAME "dgemm"
#define SFUNC_NAME "sgemm"
#define READ_WRITE size * size * 3
#define ORDER 2 * size * size * size

inline void func(float* A, float* B, float* C, const size_t size, cublasHandle_t &handle){
	int n = size;
	float alpha = 1.0;
	float beta = 1.0;
	cublasSgemm(handle, CUBLAS_OP_N, CUBLAS_OP_N, n, n, n, &alpha, A, n, B, n, &beta, C, n);
}

inline void func(double* A, double* B, double* C, const size_t size, cublasHandle_t &handle){
	int n = size;
	double alpha = 1.0;
	double beta = 1.0;
	cublasDgemm(handle, CUBLAS_OP_N, CUBLAS_OP_N, n, n, n, &alpha, A, n, B, n, &beta, C, n);
}

template <typename T>
double bench(const size_t size, const size_t iter){
	cudaError_t cudaStat;    
	cublasStatus_t stat;
	cublasHandle_t handle;

	std::vector<T> HostA(size*size);
	std::vector<T> HostB(size*size);
	std::vector<T> HostC(size*size, 0.0);

	for(size_t i=0; i<size*size; i++){
		HostA[i] = i;
		HostB[i] = 123.0;
	}

	T* DevA;
	T* DevB;
	T* DevC;

	cudaStat = cudaMalloc ((void**)&DevA, size*size * sizeof(T));
	cudaStat = cudaMalloc ((void**)&DevB, size*size * sizeof(T));
	cudaStat = cudaMalloc ((void**)&DevC, size*size * sizeof(T));

	if (cudaStat != cudaSuccess) {
		std::cout << "device memory allocation failed" << std::endl;
		return EXIT_FAILURE;
	}

	stat = cublasCreate(&handle);
	if (stat != CUBLAS_STATUS_SUCCESS) {
		std::cout << "CUBLAS initialization failed\n" << std::endl;
		return EXIT_FAILURE;
	}

	stat = cublasSetMatrix(size, size, sizeof(T), HostA.data(), size, DevA, size);
	if (stat != CUBLAS_STATUS_SUCCESS) {
		std::cout << "data download failed" << std::endl;
		cudaFree (DevA);
		cublasDestroy(handle);
		return EXIT_FAILURE;
	}
	stat = cublasSetMatrix(size, size, sizeof(T), HostB.data(), size, DevB, size);
	if (stat != CUBLAS_STATUS_SUCCESS) {
		std::cout << "data download failed" << std::endl;
		cudaFree (DevB);
		cublasDestroy(handle);
		return EXIT_FAILURE;
	}
	stat = cublasSetMatrix(size, size, sizeof(T), HostC.data(), size, DevC, size);
	if (stat != CUBLAS_STATUS_SUCCESS) {
		std::cout << "data download failed" << std::endl;
		cudaFree (DevC);
		cublasDestroy(handle);
		return EXIT_FAILURE;
	}


	cudaStreamSynchronize(0);

	double time = omp_get_wtime();
	for(size_t i = 0; i < iter; i++){
		func(DevA, DevB, DevC, size, handle);
	}
	cudaStreamSynchronize(0);
	time = (omp_get_wtime() - time) / iter;

	cudaFree (DevA);
	cudaFree (DevB);
	cudaFree (DevC);
	cublasDestroy(handle);

	return time;
}

void output_result_yaml(
		const std::string func,
		const double time,
		const size_t size,
		const size_t prec
		){

	double mem = READ_WRITE * prec / time / 1.0e+9;
	double perf = ORDER / time / 1.0e+9;
	double th = omp_get_max_threads();

	// output yaml format
	std::cout << "- {" << std::flush;

	// type name
	std::cout << "\"type\" : " << "\"blas1\"" << std::flush;
	std::cout << ", " << std::flush;

	// func name
	std::cout << "\"func\" : " << "\"" << func << "\"" << std::flush;
	std::cout << ", " << std::flush;

	// arch. name
	std::cout << "\"arch\" : " << "\"gpu\"" << std::flush;
	std::cout << ", " << std::flush;

	// threads
	std::cout << "\"threads\" : " << th << std::flush;
	std::cout << ", " << std::flush;

	// vector_size
	std::cout << "\"size\" : " << size << std::flush;
	std::cout << ", " << std::flush;

	// time
	std::cout << "\"time_sec\" : " << time << std::flush;
	std::cout << ", " << std::flush;

	// perf
	std::cout << "\"perf_gflops\" : " << perf << std::flush;
	std::cout << "}" << std::endl;
}

int main(int argc, char** argv){

	if(argc!=4){
		std::cout << "error $1:vector size, $2: iter, $3: precision (double or float)" << std::endl;
		return 1;
	}

	size_t size = atoi(argv[1]);
	size_t iter = atoi(argv[2]);

	double time = 0;

	if(strcmp(argv[3], "float") == 0){
		time = bench<float>(size,iter);
		output_result_yaml(SFUNC_NAME, time, size, sizeof(float));
	}

	if(strcmp(argv[3], "double")==0){
		time = bench<double>(size,iter);
		output_result_yaml(DFUNC_NAME, time, size, sizeof(double));
	}

	return 0;
}

