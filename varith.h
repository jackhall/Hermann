 /*
These functions are meant for use on arrays of length=3 only, with the exception of LogicIndex and LogicIndexLength.
*/
void Assign(double x[], double y[])
{
	y[0] = x[0];
	y[1] = x[1];
	y[2] = x[2];
}

void ScalarMultiply(double x, double y[], double z[])
{
	z[0] = x * y[0];
	z[1] = x * y[1];
	z[2] = x * y[2];
}

double Dot(double x[], double y[])
{
	return (x[0]*y[0] + x[1]*y[1] + x[2]*y[2]);
}

void Cross(double x[], double y[], double z[])
{
	z[0] = x[1]*y[2] - x[2]*y[1];
	z[1] = x[2]*y[0] - x[0]*y[2];
	z[2] = x[0]*y[1] - x[1]*y[0];
}

void Add(double x[], double y[], double z[])
{
	z[0] = x[0] + y[0];
	z[1] = x[1] + y[1];
	z[2] = x[2] + y[2];
}

void Subtract(double x[], double y[], double z[])
{
	z[0] = x[0] - y[0];
	z[1] = x[1] - y[1];
	z[2] = x[2] - y[2];
}

void RowIndexAssign(double x[], int n, int m, int y, double z[], int nnew, int w)
{
	//x is the indexed matrix with n rows and m columns. 
	//y is the indexed row in x. 
	//z is the assigned matix with nnew rows. 
	//w is the assigned row in z.
	for(int i=0;i<m;i++)
	{
		z[w+i*nnew] = x[y+i*n];
	}
}

void LogicIndex(double x[], double y[], int n, int m, double z[], int nnew)
{
	//x is the set of points, y is the array of boolean values, n is the number of points, and m is the number of columns
	//z is the result, which has nnew rows

	int i, j;
	j = 0;

	for (i=0;i<n;i++)
	{
		if ((y[i]) != 0.0) 
		{
			RowIndexAssign(x, n, m, i, z, nnew, j);
			j++;
		}
	}
}

int LogicIndexLength(double x[], int y)
{
	//x is the array of boolean values and y is the length of x
	//the function returns the number of true values in x

	int i, z;
	z = 0;

	for (i=0;i<y;i++)
	{
		if (x[i] != 0.0)
		{
			z++;
		}
	}

	return z;
}

int UniqueRows(double A[], int An, int Am, double B[])
{
	//A is a matrix, and B is a logical array
	//in B, a 0 denotes a redundant row
	//the first of any redundant rows is kept
	int count;
	count = 0;

	for (int i=0;i<An;i++)
	{
		B[i] = 1.0;
	}

	for (int i=0;i<(An-1);i++)
	{
		if (B[i] == 1.0)
		{
			for (int j=(i+1);j<An;j++)
			{
				if (B[j] == 1.0)
				{
					for (int k=0;k<Am;k++)
					{
						if (A[i+k*An] != A[j+k*An])
						{
							break;
						}
						else
						{
							if (k == (Am-1))
							{
								B[j] = 0.0;
								count++;
							}
						}
					}
				}
			}
		}
	}

	return An-count;
}
