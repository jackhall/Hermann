#include "mex.h"
#include "matrix.h"
#include "varith.h"
#include "geotest.h"
#include <cstdlib>

using namespace std;

void mexFunction(int nlhs, mxArray *plhs[ ],
int nrhs, const mxArray *prhs[ ])
{
	double *wQ, *wC, *wB; 
	int wQn = mxGetM(prhs[0]);
	wQ = mxGetPr(prhs[0]);
	
	plhs[0] = mxCreateDoubleMatrix(1, 3, mxREAL);
	wC = mxGetPr(plhs[0]);
	plhs[1] = mxCreateDoubleMatrix(wQn, 1, mxREAL);
	wB = mxGetPr(plhs[1]);

	PtMinNorm(wQ, wQn, wC, wB);
}
