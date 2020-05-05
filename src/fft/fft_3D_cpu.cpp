#include<fftw3.h>
#include<omp.h>
#include<math.h>
#include<vector>
#include<iostream>
#include<string.h>

#define DFUNC_NAME "fft-3d"
#define READ_WRITE 2 * size
#define ORDER 5.0*(double)x*(double)y*(double)z*(log((double)x)+log((double)y)+log((double)z))/log(2.0)


double bench(const size_t x, const size_t y, const size_t z, const size_t iter, const char* type){
	size_t n = x * y * z;
	FFT_TYPE *in, *out;
	FFT_PLAN plan;
	double time = 0;

	//set thread num
	size_t omp_num;
	#pragma omp parallel
	omp_num = omp_get_num_threads();

	fftwf_plan_with_nthreads(omp_num);

	in = (FFT_TYPE *)FFT_MALLOC(sizeof(FFT_TYPE) * n);
	out = (FFT_TYPE *)FFT_MALLOC(sizeof(FFT_TYPE) * n);

	//exec
	bool forward;
	if( strcmp(type, "forward") == 0) {
		plan = FFT_PLAN_DFT_3D(x, y, z, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
		time = omp_get_wtime();
		for(size_t i = 0; i < iter; ++i) {
			FFT_EXE(plan);
		}
		time = (omp_get_wtime() - time) / (double)iter;
	}
	else if( strcmp(type, "backward") == 0) {
		plan = FFT_PLAN_DFT_3D(x, y, z, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);
		time = omp_get_wtime();
		for(size_t i = 0; i < iter; ++i) {
			FFT_EXE(plan);
		}
		time = (omp_get_wtime() - time) / (double)iter;
	}
	else{
		printf("error, type = %s\n", type);
	}
	FFT_DESTROY(plan);
	FFT_FREE(in);
	FFT_FREE(out);

	return time;
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

void output_result_yaml(
		const std::string func,
		const double time,
		const size_t x,
		const size_t y,
		const size_t z
		){

	double perf = ORDER / time / 1.0e+9;
	double th = omp_get_max_threads();

	// output yaml format
	std::cout << "- {" << std::flush;

	// type name
	std::cout << "\"type\" : " << "\"fft\"" << std::flush;
	std::cout << ", " << std::flush;

	// func name
	std::cout << "\"func\" : " << "\"" << func << "\"" << std::flush;
	std::cout << ", " << std::flush;

	// arch. name
	std::cout << "\"arch\" : " << "\"cpu\"" << std::flush;
	std::cout << ", " << std::flush;

	// vector_size
	std::cout << "\"threads\" : " << th << std::flush;
	std::cout << ", " << std::flush;

	// vector_size
	std::cout << "\"size\" : " << "\"" << x << std::flush;
	std::cout << ", " << std::flush;

	// func name
	std::cout << "\"time_sec\" : " << time << std::flush;
	std::cout << ", " << std::flush;

	// perf
	std::cout << "\"perf_gflops\" : " << perf << std::flush;
	std::cout << "}" << std::endl;
}
 
int main(int argc, char** argv){

	if(argc!=6){
		//std::cout << "error $1:vector size, $2: iter, $3: precision (double or float)" << std::endl;
		std::cout << "error $1:vector size, $2: iter, $3: \"forward\" or \"backward\"" << std::endl;
		std::cout << "if 1D, y=1, z=1. else if 2D, z=1." << std::endl;
		return 1;
	}

	size_t x = atoi(argv[1]);
	size_t y = atoi(argv[2]);
	size_t z = atoi(argv[3]);
	size_t iter = atoi(argv[4]);
	char* type = argv[5];

	double time = 0;

	char funcname[64];
	if( y == 1 && z == 1){
		strcat(funcname,"fft-1D_");
	}
	else if( z == 1){
		strcat(funcname,"fft-2D_");
	}
	else{
		strcat(funcname,"fft-3D_");
	}
	strcat(funcname, PREC);
	strcat(funcname, "_");
	strcat(funcname, type);

	z = 1;

	time = bench(x, y, z, iter, type);
	output_result_yaml(funcname, time, x, y, z);

	return 0;
}

