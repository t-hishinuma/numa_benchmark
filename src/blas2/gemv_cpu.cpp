#include<cblas.h>
#include<omp.h>
#include<vector>
#include<iostream>
#include<string.h>

#define DFUNC_NAME "dgemv"
#define SFUNC_NAME "sgemv"
#define READ_WRITE size * size + 2 * size
#define ORDER 2 * size * size

inline void func(const std::vector<float>& A, const std::vector<float> &x, std::vector<float> &y){
	cblas_sgemv(CblasRowMajor, CblasNoTrans, x.size(), x.size(), 1.0, A.data(), x.size(), x.data(), 1, 2.0, y.data(), 1);
}

inline void func(const std::vector<double>& A, const std::vector<double> &x, std::vector<double> &y){
	cblas_dgemv(CblasRowMajor, CblasNoTrans, x.size(), x.size(), 1.0, A.data(), x.size(), x.data(), 1, 2.0, y.data(), 1);
}

template<typename T>
double bench(const size_t size, const size_t iter){
	
	std::vector<T> x(size);
	std::vector<T> y(size, 0.0);
	std::vector<T> A(size*size, 321.0);
	T ans = 0;

	for(size_t i=0; i<size; i++){
		x[i] = i;
	}


	double time = omp_get_wtime();
	for(size_t i = 0; i < iter; i++){
		func(A, x, y);
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
	std::cout << "\"type\" : " << "\"blas2\"" << std::flush;
	std::cout << ", " << std::flush;

	// func name
	std::cout << "\"func\" : " << "\"" << func << "\"" << std::flush;
	std::cout << ", " << std::flush;

	// arch. name
	std::cout << "\"arch\" : " << "\"cpu\"" << std::flush;
	std::cout << ", " << std::flush;

	// thread
	std::cout << "\"threads\" : " << th << std::flush;
	std::cout << ", " << std::flush;

	// vector_size
	std::cout << "\"size\" : " << size << std::flush;
	std::cout << ", " << std::flush;

	// time
	std::cout << "\"time_sec\" : " << time << std::flush;
	std::cout << ", " << std::flush;

// 	// memory B/W
// 	std::cout << "\"mem_gb_s\" : " << mem << std::flush;
// 	std::cout << ", " << std::flush;

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

