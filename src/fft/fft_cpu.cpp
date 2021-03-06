#include<fftw3.h>
#include<omp.h>
#include<math.h>
#include<vector>
#include<iostream>
#include<string.h>

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

	FFT_NTHREAD(omp_num);

	in = (FFT_TYPE *)FFT_MALLOC(sizeof(FFT_TYPE) * n);
	out = (FFT_TYPE *)FFT_MALLOC(sizeof(FFT_TYPE) * n);

	bool forward;
	if( strcmp(type, "forward") == 0) {
		// PLAN
		if( y == 1 && z == 1){
			plan = FFT_PLAN_DFT_1D(x, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
		}
		else if( z == 1){
			plan = FFT_PLAN_DFT_2D(x, y, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
		}
		else{
			plan = FFT_PLAN_DFT_3D(x, y, z, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
		}

		FFT_EXE(plan);
		//#EXEC
		time = omp_get_wtime();
		for(size_t i = 0; i < iter; ++i) {
			FFT_EXE(plan);
		}
		time = (omp_get_wtime() - time) / (double)iter;

	}
	else if( strcmp(type, "backward") == 0) {
		// PLAN
		if( y == 1 && z == 1){
			plan = FFT_PLAN_DFT_1D(x, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);
		}
		else if( z == 1){
			plan = FFT_PLAN_DFT_2D(x, y, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);
		}
		else{
			plan = FFT_PLAN_DFT_3D(x, y, z, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);
		}

		FFT_EXE(plan);
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

void output_result_yaml(
		const std::string func,
		const double time,
		const size_t x,
		const size_t y,
		const size_t z
		){

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

	out("type", "fft", ",");
	out("func", func, ",");
	out("arch", "cpu", ",");
	out("threads", th, ",");
	out("size", x, ",");
	out("time_sec", time, ",");
	out("perf_gflops", perf, "}");

	std::cout << std::endl;
}
 
int main(int argc, char** argv){

	if(argc!=5){
		//std::cout << "error $1:vector size, $2: iter, $3: precision (double or float)" << std::endl;
		std::cout << "error $1:size, $2: iter, $3: \"forward\" or \"backward\" $4: dim.(1 or 2 or 3)" << std::endl;
		return 1;
	}

	size_t x = atoi(argv[1]);
	size_t iter = atoi(argv[2]);
	char* type = argv[3];
	size_t dim = atoi(argv[4]);
	size_t z, y;

	char funcname[64] = "";
	if( dim == 3){
		y = x;
		z = x;
		strcat(funcname, "fft3d_");
	}
	else if( dim == 2){
		y = x;
		z = 1;
		strcat(funcname, "fft2d_");
	}
	else if( dim == 1){
		y = 1;
		z = 1;
		strcat(funcname, "fft1d_");
	}
	else{
		std::cout << "error, dim is " << dim << std::endl;

	}
	strcat(funcname, PREC);
	strcat(funcname, "_");
	strcat(funcname, type);

	z = 1;

	double time = bench(x, y, z, iter, type);
	output_result_yaml(funcname, time, x, y, z);

	return 0;
}
