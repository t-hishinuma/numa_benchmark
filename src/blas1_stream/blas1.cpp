#include<cblas.h>
#include<omp.h>
#include<vector>
#include<iostream>

int main(){
	std::vector<double> x(100,1.0);
	std::vector<double> y(100,2.0);

	double ans = 0;
   	ans = cblas_ddot(x.size(), x.data(), 1,  y.data(), 1);

	return 0;
}
