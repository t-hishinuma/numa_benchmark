#include <cuda_runtime.h>
#include "cublas_v2.h"
#include <iostream>
#include <vector>
#include<omp.h>

#define DFUNC_NAME "dgemv"
#define SFUNC_NAME "sgemv"
#define READ_WRITE size * size + 2 * size
#define ORDER 2 * size * size 

inline void func(const float* A, const float* x, float* y, const size_t n, cublasHandle_t &handle){
	float alpha = 1.0;
	float beta = 2.0;
	cublasSgemv(handle, CUBLAS_OP_N, n, n, &alpha, A, n, x, 1, &beta, y, 1);
}

inline void func(const double* A, const double* x, double* y, const size_t n, cublasHandle_t &handle){
	double alpha = 1.0;
	double beta = 2.0;
	cublasDgemv(handle, CUBLAS_OP_N, n, n, &alpha, A, n, x, 1, &beta, y, 1);
}

template <typename T>
double bench(const size_t size, const size_t iter){
	cudaError_t cudaStat;    
	cublasStatus_t stat;
	cublasHandle_t handle;

	std::vector<T> HostA(size*size, 321.0);
	std::vector<T> Hostx(size);
	std::vector<T> Hosty(size, 0.0);

	for(size_t i=0; i<size; i++){
		Hostx[i] = i;
	}

	T* DevA;
	T* Devx;
	T* Devy;

	cudaStat = cudaMalloc ((void**)&DevA, size*size * sizeof(T));
	cudaStat = cudaMalloc ((void**)&Devx, size * sizeof(T));
	cudaStat = cudaMalloc ((void**)&Devy, size * sizeof(T));

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
		cudaFree (DevA); cudaFree (Devx); cudaFree (Devy);
		cublasDestroy(handle);
		return EXIT_FAILURE;
	}
	stat = cublasSetVector(size, sizeof(T), Hostx.data(), 1, Devx, 1);
	if (stat != CUBLAS_STATUS_SUCCESS) {
		std::cout << "data download failed" << std::endl;
		cudaFree (DevA); cudaFree (Devx); cudaFree (Devy);
		cublasDestroy(handle);
		return EXIT_FAILURE;
	}
	stat = cublasSetVector(size, sizeof(T), Hosty.data(), 1, Devy, 1);
	if (stat != CUBLAS_STATUS_SUCCESS) {
		std::cout << "data download failed" << std::endl;
		cudaFree (DevA); cudaFree (Devx); cudaFree (Devy);
		cublasDestroy(handle);
		return EXIT_FAILURE;
	}


	func(DevA, Devx, Devy, size, handle);
	cudaStreamSynchronize(0);

	double time = omp_get_wtime();
	for(size_t i = 0; i < iter; i++){
		func(DevA, Devx, Devy, size, handle);
	}
	cudaStreamSynchronize(0);
	time = (omp_get_wtime() - time) / iter;

	cudaFree (DevA);
	cudaFree (Devx);
	cudaFree (Devy);
	cublasDestroy(handle);

	return time;
}

void output_result_yaml(
		const std::string func,
		const double time,
		const size_t size,
		const size_t prec
		){

	double perf = ORDER / time / 1.0e+9;
	double th = omp_get_max_threads();

	std::cout << "- {" << std::flush;

	auto out = [](const auto x, const auto y, const auto sep){
		std::cout << "\"" <<  x << "\"" << " : " << std::flush;
		if(typeid(y) == typeid(std::string) || typeid(y) == typeid(const char*)){
			std::cout << "\"" <<  y << "\"" << std::flush;
		}
		else{
			std::cout << y << std::flush;
		}
		std::cout << sep << " " << std::flush;
	};

	out("type", "blas1", ",");
	out("func", func, ",");
	out("arch", "cpu", ",");
	out("threads", th, ",");
	out("size", size, ",");
	out("time_sec", time, ",");
	//out("mem_gb_s", mem, ",");
	out("perf_gflops", perf, "}");

	std::cout << std::endl;
}

int main(int argc, char** argv){

	if(argc!=4){
		std::cout << "error $1:vector size, $2: iter, $3: precision (double or float)" << std::endl;
		return 1;
	}

	size_t size = atoi(argv[1]);
	size_t iter = atoi(argv[2]);

	if(strcmp(argv[3], "float") == 0){
		double time = bench<float>(size,iter);
		output_result_yaml(SFUNC_NAME, time, size, sizeof(float));
	}

	if(strcmp(argv[3], "double")==0){
		double time = bench<double>(size,iter);
		output_result_yaml(DFUNC_NAME, time, size, sizeof(double));
	}

	return 0;
}
