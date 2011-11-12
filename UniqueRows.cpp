#include "mex.h"
#include "matrix.h"
#include "varith.h"
#include <cstdlib>

using namespace std;

void mexFunction(int nlhs, mxArray *plhs[ ],
int nrhs, const mxArray *prhs[ ])
{
	double *wA, *wB, *wC;
	int wAn = mxGetM(prhs[0]);
	int wAm = mxGetN(prhs[0]);
	wA = mxGetPr(prhs[0]);

	wB = (double*)mxCalloc(wAn,sizeof(double));

	int wBn = UniqueRows(wA, wAn, wAm, wB);

	plhs[0] = mxCreateDoubleMatrix(wBn, wAm, mxREAL);
	wC = mxGetPr(plhs[0]);

	LogicIndex(wA, wB, wAn, wAm, wC, wBn);

	mxFree(wB);
}
