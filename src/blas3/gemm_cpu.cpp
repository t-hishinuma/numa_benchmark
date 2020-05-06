#include<cblas.h>
#include<omp.h>
#include<vector>
#include<iostream>
#include<string.h>

#define DFUNC_NAME "dgemm"
#define SFUNC_NAME "sgemm"
#define READ_WRITE size * size * 3
#define ORDER 2 * size * size * size

inline void func(std::vector<float> &A, std::vector<float> &B, std::vector<float> &C, const size_t size){
	int n = size;
	cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1.0, A.data(), n, B.data(), n, 1.0, C.data(), n);
}

inline void func(std::vector<double> &A, std::vector<double> &B, std::vector<double> &C, const size_t size){
	int n = size;
	cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1.0, A.data(), n, B.data(), n, 1.0, C.data(), n);
}

template<typename T>
double bench(const size_t size, const size_t iter){
	
	std::vector<T> A(size*size);
	std::vector<T> B(size*size);
	std::vector<T> C(size*size, 0.0);
	openblas_set_num_threads(omp_get_max_threads());

 	if(omp_get_max_threads() != openblas_get_num_threads()){
 		printf("# error, OpenBLAS does not support multi-threading? (env=%d, openblas:%d)", omp_get_max_threads(), openblas_get_num_threads());
		exit;
 	}

	for(size_t i=0; i<size*size; i++){
		A[i] = i;
		B[i] = 123.0;
	}

	func(A, B, C, size);
	double time = omp_get_wtime();
	for(size_t i = 0; i < iter; i++){
		func(A, B, C, size);
	}
	time = (omp_get_wtime() - time) / iter;

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
	std::cout << "\"type\" : " << "\"blas3\"" << std::flush;
	std::cout << ", " << std::flush;

	// func name
	std::cout << "\"func\" : " << "\"" << func << "\"" << std::flush;
	std::cout << ", " << std::flush;

	// arch. name
	std::cout << "\"arch\" : " << "\"cpu\"" << std::flush;
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
		std::cout << "error $1:matrix size n (n=m=k), $2: iter, $3: precision (double or float)" << std::endl;
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

