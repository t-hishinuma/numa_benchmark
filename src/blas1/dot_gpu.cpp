#include <cuda_runtime.h>
#include <cublas_v2.h>
#include <iostream>
#include <vector>
#include<omp.h>

#define DFUNC_NAME "ddot"
#define SFUNC_NAME "sdot"
#define READ_WRITE 2 * size
#define ORDER 2 * size

inline float func(const float* x, const float* y, const size_t size, cublasHandle_t &handle){
	float result = 0;
	cublasSdot(handle, size, x, 1, y, 1, &result);
	return result;
}

inline double func(const double* x, const double* y, const size_t size, cublasHandle_t &handle){
	double result = 0;
	cublasDdot(handle, size, x, 1, y, 1, &result);
	return result;
}

template <typename T>
double bench(const size_t size, const size_t iter){
	cudaError_t cudaStat;    
	cublasStatus_t stat;
	cublasHandle_t handle;

	std::vector<T> Hostx(size);
	std::vector<T> Hosty(size);

	#pragma omp parallel for
	for(size_t i=0; i<size; i++){
		Hostx[i] = i;
		Hosty[i] = 123.0;
	}

	T* Devx;
	T* Devy;

	cudaStat = cudaMalloc ((void**)&Devx, Hostx.size() * sizeof(T));
	cudaStat = cudaMalloc ((void**)&Devy, Hosty.size() * sizeof(T));

	if (cudaStat != cudaSuccess) {
		std::cout << "device memory allocation failed" << std::endl;
		return EXIT_FAILURE;
	}

	stat = cublasCreate(&handle);
	if (stat != CUBLAS_STATUS_SUCCESS) {
		std::cout << "CUBLAS initialization failed\n" << std::endl;
		return EXIT_FAILURE;
	}

	stat = cublasSetVector(Hostx.size(), sizeof(T), Hostx.data(), 1, Devx, 1);
	if (stat != CUBLAS_STATUS_SUCCESS) {
		std::cout << "data download failed" << std::endl;
		cudaFree (Devx); cudaFree (Devy);
		cublasDestroy(handle);
		return EXIT_FAILURE;
	}

	stat = cublasSetVector(Hosty.size(), sizeof(T), Hosty.data(), 1, Devy, 1);
	if (stat != CUBLAS_STATUS_SUCCESS) {
		std::cout << "data download failed" << std::endl;
		cudaFree (Devx); cudaFree (Devy);
		cublasDestroy(handle);
		return EXIT_FAILURE;
	}

	cudaStreamSynchronize(0);
	T result=0;
	double time = omp_get_wtime();
	for(size_t i = 0; i < iter; i++){
		result = func(Devx, Devy, size, handle);
	}
	cudaStreamSynchronize(0);
	time = (omp_get_wtime() - time) / iter;

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

	double mem = READ_WRITE * prec / time / 1.0e+9;
	double perf = ORDER / time / 1.0e+9;
	double th = omp_get_max_threads();

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

	std::cout << "- {" << std::flush;

	out("type", "blas1", ",");
	out("func", func, ",");
	out("arch", "gpu", ",");
	out("threads", th, ",");
	out("size", size, ",");
	out("time_sec", time, ",");
	out("mem_gb_s", mem, ",");
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
