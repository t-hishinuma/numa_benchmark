#include<cblas.h>
#include<omp.h>
#include<vector>
#include<iostream>
#include<string.h>

#define DFUNC_NAME "ddot"
#define SFUNC_NAME "sdot"
#define READ_WRITE 2 * size
#define ORDER 2 * size

inline float func(const std::vector<float> &x, const std::vector<float> &y){
	return  cblas_sdot(x.size(), x.data(), 1,  y.data(), 1);
}

inline double func(const std::vector<double> &x, const std::vector<double> &y){
	return  cblas_ddot(x.size(), x.data(), 1,  y.data(), 1);
}

template<typename T>
double bench(const size_t size, const size_t iter){
	
	std::vector<T> x(size);
	std::vector<T> y(size);
	T ans = 0;
	openblas_set_num_threads(omp_get_max_threads());

 	if(omp_get_max_threads() != openblas_get_num_threads()){
 		printf("# error, OpenBLAS does not support multi-threading? (env=%d, openblas:%d)", omp_get_max_threads(), openblas_get_num_threads());
		exit;
 	}

	#pragma omp parallel for
	for(size_t i=0; i<size; i++){
		x[i] = i;
		y[i] = 123.0;
	}

	ans = func(x, y);

	double time = omp_get_wtime();
	for(size_t i = 0; i < iter; i++){
		ans = func(x, y);
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
	std::cout << "\"type\" : " << "\"blas1\"" << std::flush;
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

	// memory B/W
	std::cout << "\"mem_gb_s\" : " << mem << std::flush;
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

