#include "performance_analyzer.h"
#include <iostream>
#include <math.h>
#include <algorithm>

double compute_pi( int n ) 
{
	performance_analyzer::begin( "compute pi" ); 

	double res = 0;
	for( int k=1; k<n; k++ ) 
	{
		res += 4 * pow(-1.0, k+1) / ( 2 * k - 1 ); 
	}

	performance_analyzer::end( "compute pi" ); 
	return res;
}

int compute_fibonacci( int n ) 
{
	performance_analyzer::begin( "compute fibonaci" ); 

	int res = 1, tmp = 1;
	for( int i=0; i<n; i++ ) 
	{
		tmp = res + tmp; 
		std::swap( res, tmp ); 
	}

	performance_analyzer::end( "compute fibonaci" ); 
	return res;
}


int main(int argc, char** argv) 
{
	long int num = 1000000;
	
	performance_analyzer::begin( "main fibonaci" ); 
	compute_pi( num );
	compute_fibonacci( num );
	compute_fibonacci( num );
	compute_fibonacci( num );
	compute_pi( num );
	performance_analyzer::end( "main fibonaci" ); 

	std::cout << performance_analyzer::summery() << std::endl; 

	return 0; 
}
