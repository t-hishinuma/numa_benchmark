#include<cblas.h>
#include<omp.h>
#include<vector>
#include<iostream>

#define DFUNC_NAME "ddot"
#define SFUNC_NAME "sdot"
#define DFUNC cblas_ddot
#define SFUNC cblas_sdot
#define ARGS (x.size(), x.data(), 1,  y.data(), 1);

#define SIZE 2 * size
#define ORDER 2 * size

void output_result_yaml(
		const std::string func,
		const double time,
		const size_t size,
		const size_t prec
		){

	double mem = SIZE * prec / time / 1.0e+9;
	double perf = ORDER / time / 1.0e+9;
	double th = omp_get_max_threads();

	// output yaml format
	std::cout << "- {" << std::flush;

	// func name
	std::cout << "\"func\" : " << "\"" << func << "\"" << std::flush;
	std::cout << ", " << std::flush;

	// vector_size
	std::cout << "\"# of threads\" : " << th << std::flush;
	std::cout << ", " << std::flush;

	// vector_size
	std::cout << "\"size\" : " << size << std::flush;
	std::cout << ", " << std::flush;

	// func name
	std::cout << "\"time [s]\" : " << time << std::flush;
	std::cout << ", " << std::flush;

	// memory B/W
	std::cout << "\"mem [GB/s]\" : " << mem << std::flush;
	std::cout << ", " << std::flush;

	// perf
	std::cout << "\"perf [GFLOPS]\" : " << perf << std::flush;
	std::cout << "}" << std::endl;
}

inline double func(std::vector<double> x, std::vector<double> y){
	return  DFUNC ARGS;
}

inline float func(std::vector<float> x, std::vector<float> y){
	return  SFUNC ARGS;
}

template<typename T>
double bench(const size_t size, const size_t iter){
	
	std::vector<T> x;
	std::vector<T> y;
	T ans = 0;

	for(size_t i=0; i<size; i++){
		x.push_back(rand());
		y.push_back(rand());
	}

	double time = omp_get_wtime();
	for(size_t i = 0; i < iter; i++){
		ans = func(x, y);
	}
	time = (omp_get_wtime() - time) / iter;

	return time;
}

int main(int argc, char** argv){

	if(argc!=3){
		std::cout << "error $1:vector size, $2: iter" << std::endl;
		return 1;
	}

	size_t size = atoi(argv[1]);
	size_t iter = atoi(argv[2]);

	double time = 0;

	time = bench<double>(size,iter);
	output_result_yaml(DFUNC_NAME, time, size, sizeof(double));

	time = bench<float>(size,iter);
	output_result_yaml(SFUNC_NAME, time, size, sizeof(float));

	return 0;
}
