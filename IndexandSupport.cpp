#include "mex.h"
#include "matrix.h"
#include "varith.h"
#include "geotest.h"
#include <cstdlib>

using namespace std;

void mexFunction(int nlhs, mxArray *plhs[ ],
int nrhs, const mxArray *prhs[ ])
{
	double *wQ, *wQnew, *wA, *wB, *wCart, *wBary, *wV, *wia, *wib;
	int wQn = mxGetM(prhs[0]);
	int wAn = mxGetM(prhs[1]);
	int wBn = mxGetM(prhs[2]);

	wQ = mxGetPr(prhs[0]);
	wA = mxGetPr(prhs[1]);
	wB = mxGetPr(prhs[2]);
	wCart = mxGetPr(prhs[3]);
	wBary = mxGetPr(prhs[4]);
	
	int wQnnew = LogicIndexLength(wBary, wQn);

	plhs[0] = mxCreateDoubleMatrix(wQnnew, 5, mxREAL);
	wQnew = mxGetPr(plhs[0]);
	plhs[1] = mxCreateDoubleMatrix(1, 3, mxREAL);
	wV = mxGetPr(plhs[1]);
	plhs[2] = mxCreateDoubleMatrix(1, 1, mxREAL);
	wia = mxGetPr(plhs[2]);
	plhs[3] = mxCreateDoubleMatrix(1, 1, mxREAL);
	wib = mxGetPr(plhs[3]);

	IndexandSupport(wQ, wQn, wBary, wQnew, wQnnew, wA, wAn, wB, wBn, wCart, wV, wia, wib);
}