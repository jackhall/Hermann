/*
These functions are subroutines for GJK.
They include: PtSideTriangle, ClosestPtPointSegment, ClosestPtPointTriangle,
ClosestPtPointTetrahdron, SupportVertex, PtMinNorm, IndexandSupport.
*/

bool PtSideTriangle(double a[], double b[], double c[], double p[], double q[])
{
	//a, b, and c are points defining a plane, and p and q are the queried points
	//the function returns true if both queried points lie on the same side of the plane
	
	double *ap, *aq, *ab, *ac, *ione;

	ap = (double*)mxCalloc(3, sizeof(double));
	aq = (double*)mxCalloc(3, sizeof(double));
	ab = (double*)mxCalloc(3, sizeof(double));
	ac = (double*)mxCalloc(3, sizeof(double));
	ione = (double*)mxCalloc(3, sizeof(double));

	Subtract(p, a, ap);
	Subtract(q, a, aq);
	Subtract(b, a, ab);
	Subtract(c, a, ac);

	Cross(ab, ac, ione);

	double signp = Dot(ap, ione);
	double signq = Dot(aq, ione);

	mxFree(ap);
	mxFree(aq);
	mxFree(ab);
	mxFree(ac);
	mxFree(ione);

	return ((signp*signq) > 0);
}

void ClosestPtPointSegment(double a[], double b[], double p[], double c[], double d[])
{
	//a and b are points defining the line segment, and p is the queried point
	//e is the closest point in cartesian coordinates, and f is that point in barycentric coordinates
	
	double *ab, *ap, t, *ione;
	
	ab = (double*)mxCalloc(3, sizeof(double));
	ap = (double*)mxCalloc(3, sizeof(double));
	ione = (double*)mxCalloc(3, sizeof(double));

	Subtract(b, a, ab);

	Subtract(p, a, ap);
	
	t = (Dot(ap, ab) / Dot(ab, ab));
	if (t < 0) t = 0;
	if (t > 1) t = 1;
	
	ScalarMultiply(t, ab, ione);
	Add(a, ione, c);

	d[0] = 1 - t;
	d[1] = t;

	mxFree(ab);
	mxFree(ap);
	mxFree(ione);
}

void ClosestPtPointTriangle(double a[], double b[], double c[], double p[], double d[], double e[])
{
	//a, b, and c are points defining the triangle, and p is the queried point
	//e is the closest point in cartesian coordinates, and f is that point in barycentric coordinates
	
	double *ab, *ac, *ap, dotone, dottwo;

	ab = (double*)mxCalloc(3, sizeof(double));
	ac = (double*)mxCalloc(3, sizeof(double));
	ap = (double*)mxCalloc(3, sizeof(double));
	
	e[0] = 0.0;
	e[1] = 0.0;
	e[2] = 0.0;

	Subtract(b, a, ab);
	Subtract(c, a, ac);
	Subtract(p, a, ap);

	dotone = Dot(ab, ap);
	dottwo = Dot(ac, ap);

	if ((dotone <= 0) && (dottwo <= 0)) //if p is in vertex region a
	{
		Assign(a, d);
		e[0] = 1.0;
		return;
	}

	double *bp, dotthree, dotfour;

	bp = (double*)mxCalloc(3, sizeof(double));

	Subtract(p, b, bp);

	dotthree = Dot(ab, bp);
	dotfour = Dot(ac, bp);

	if ((dotthree >= 0) && (dotfour <= dotthree)) //if p is in vertex region b
	{
		Assign(b, d);
		e[1] = 1.0;
		return;
	}

	double vc = dotone*dotfour - dotthree*dottwo;

	if ((vc <= 0) && (dotone >= 0) && (dotthree <= 0)) //if p is in edge region ab
	{
		double t = dotone / (dotone - dotthree);
		double *ione;

		ione = (double*)mxCalloc(3, sizeof(double));

		ScalarMultiply(t, ab, ione);
		Add(a, ione, d);
		e[0] = 1 - t;
		e[1] = t;

		mxFree(ione);

		return;
	}

	double *cp, dotfive, dotsix;

	cp = (double*)mxCalloc(3, sizeof(double));

	Subtract(p, c, cp);

	dotfive = Dot(ab, cp);
	dotsix = Dot(ac, cp);

	if ((dotsix >=0) && (dotfive <= dotsix)) //if p is in vertex region c
	{
		Assign(c, d);
		e[2] = 1.0;
		return;
	}

	double vb = dotfive*dottwo - dotone*dotsix;

	if ((vb <= 0) && (dottwo >= 0) && (dotsix <= 0)) //if p is in edge region ac
	{
		double t = dottwo / (dottwo - dotsix);
		double ione[3];
		ScalarMultiply(t, ac, ione);
		Add(a, ione, d);
		e[0] = 1 - t;
		e[2] = t;
		return;
	}

	double *bc;
	double va = dotthree*dotsix - dotfive*dotfour;
	double dotseven = dotfour - dotthree;
	double doteight = dotfive - dotsix;

	bc = (double*)mxCalloc(3, sizeof(double));

	Subtract(c, b, bc);

	if ((va <= 0) && (dotseven >= 0) && (doteight >= 0)) //if p is in edge region bc
	{
		double t = dotseven / (dotseven + doteight);
		double *ione;

		ione = (double*)mxCalloc(3, sizeof(double));

		ScalarMultiply(t, bc, ione);
		Add(b, ione, d);
		e[1] = 1 - t;
		e[2] = t;

		mxFree(ione);

		return;
	}

	double dotnine = 1 / (va + vb + vc); //p is in face region
	double baryb = vb * dotnine;
	double baryc = vc * dotnine;
	double *ione, *itwo, *ithree;

	ione = (double*)mxCalloc(3, sizeof(double));
	itwo = (double*)mxCalloc(3, sizeof(double));
	ithree = (double*)mxCalloc(3, sizeof(double));

	ScalarMultiply(baryb, ab, ione);
	ScalarMultiply(baryc, ac, itwo);
	Add(a, ione, ithree);
	Add(ithree, itwo, d);
	e[0] = 1 - baryb - baryc;
	e[1] = baryb;
	e[2] = baryc;

	mxFree(ab);
	mxFree(ac);
	mxFree(ap);
	mxFree(bp);
	mxFree(cp);
	mxFree(bc);
	mxFree(ione);
	mxFree(itwo);
	mxFree(ithree);

	return;
}

void ClosestPtPointTetrahedron(double a[], double b[], double c[], double d[], double p[], double e[], double f[])
{
	//a, b, c, and d are points defining the tetrahedron, and p is the queried point
	//e is the closest point in cartesian coordinates, and f is that point in barycentric coordinates
	
	Assign(p, e);
	double *te, *tf, *pe;

	te = (double*)mxCalloc(3, sizeof(double));
	tf = (double*)mxCalloc(3, sizeof(double));
	pe = (double*)mxCalloc(3, sizeof(double));

	f[0] = 1.0;
	f[1] = 1.0;
	f[2] = 1.0;
	f[3] = 1.0;

	double best = 1000000000.0;
	double dist;
	
	if (!PtSideTriangle(a, b, c, p, d))
	{
		ClosestPtPointTriangle(a, b, c, p, te, tf);
		Subtract(te, p, pe);
		dist = Dot(pe, pe);

		if (dist < best)
		{
			best = dist;
			Assign(te, e);
			f[0] = tf[0];
			f[1] = tf[1];
			f[2] = tf[2];
			f[3] = 0.0;
		}
	}

	if (!PtSideTriangle(a, c, d, p, b))
	{
		ClosestPtPointTriangle(a, c, d, p, te, tf);
		Subtract(te, p, pe);
		dist = Dot(pe, pe);

		if (dist < best)
		{
			best = dist;
			Assign(te, e);
			f[0] = tf[0];
			f[1] = 0.0;
			f[2] = tf[1];
			f[3] = tf[2];
		}
	}

	if (!PtSideTriangle(a, b, d, p, c))
	{
		ClosestPtPointTriangle(a, b, d, p, te, tf);
		Subtract(te, p, pe);
		dist = Dot(pe, pe);

		if (dist < best)
		{
			best = dist;
			Assign(te, e);
			f[0] = tf[0];
			f[1] = tf[1];
			f[2] = 0.0;
			f[3] = tf[2];
		}
	}

	if (!PtSideTriangle(b, c, d, p, a))
	{
		ClosestPtPointTriangle(b, c, d, p, te, tf);
		Subtract(te, p, pe);
		dist = Dot(pe, pe);

		if (dist < best)
		{
			Assign(te, e);
			f[0] = 0.0;
			f[1] = tf[0];
			f[2] = tf[1];
			f[3] = tf[2];
		}
	}

	mxFree(te);
	mxFree(tf);
	mxFree(pe);
}

void SupportVertex(double x[], int n, double q[], double v[], double *ix, double r[])
{
	//x[] is the set of points, n is the number of points, and q[] is the negative of the direction vector
	//v[] is the resulting point, chosen from x[], and r[] is the direction vector

	double *a, tdot, dotmax;
	int i;

	a = (double*)mxCalloc(3, sizeof(double));
	
	ScalarMultiply(-1.0, q, r);

	for (i=0;i<n;i++)
	{
		a[0] = x[i];
		a[1] = x[i+n];
		a[2] = x[i+(2*n)];

		tdot = Dot(a, r);

		if ((tdot > dotmax) || (i == 0))
		{
			dotmax = tdot;
			Assign(a, v);
			*ix = i + 0.0;
		}
	}

	mxFree(a);
}

void PtMinNorm(double *Q, int Qn, double *C, double *B)
{
	double *a, *b, *c, *d, *p;
	
	switch (Qn)
		{
		case 1:
			
			B[0] = 1.0;
			//mexPrintf("Case 1\n");
			Assign(Q, C);
			break;

		case 2:

			//mexPrintf("Case 2\n");
			
			a = (double*)mxCalloc(3, sizeof(double));
			b = (double*)mxCalloc(3, sizeof(double));
			p = (double*)mxCalloc(3, sizeof(double));

			RowIndexAssign(Q, Qn, 3, 0, a, 1, 0);
			RowIndexAssign(Q, Qn, 3, 1, b, 1, 0);
			ClosestPtPointSegment(a, b, p, C, B);

			mxFree(a);
			mxFree(b);
			mxFree(p);

			break;

		case 3:

			//mexPrintf("Case 3\n");
			
			a = (double*)mxCalloc(3, sizeof(double));
			b = (double*)mxCalloc(3, sizeof(double));
			c = (double*)mxCalloc(3, sizeof(double));
			p = (double*)mxCalloc(3, sizeof(double));

			RowIndexAssign(Q, Qn, 3, 0, a, 1, 0);
			RowIndexAssign(Q, Qn, 3, 1, b, 1, 0);
			RowIndexAssign(Q, Qn, 3, 2, c, 1, 0);
			ClosestPtPointTriangle(a, b, c, p, C, B);

			mxFree(a);
			mxFree(b);
			mxFree(c);
			mxFree(p);

			break;

		case 4:

			//mexPrintf("Case 4\n");
			
			a = (double*)mxCalloc(3, sizeof(double));
			b = (double*)mxCalloc(3, sizeof(double));
			c = (double*)mxCalloc(3, sizeof(double));
			d = (double*)mxCalloc(3, sizeof(double));
			p = (double*)mxCalloc(3, sizeof(double));

			RowIndexAssign(Q, Qn, 3, 0, a, 1, 0);
			RowIndexAssign(Q, Qn, 3, 1, b, 1, 0);
			RowIndexAssign(Q, Qn, 3, 2, c, 1, 0);
			RowIndexAssign(Q, Qn, 3, 3, d, 1, 0);
			ClosestPtPointTetrahedron(a, b, c, d, p, C, B);

			mxFree(a);
			mxFree(b);
			mxFree(c);
			mxFree(d);
			mxFree(p);

			break;

		default:

			mexErrMsgTxt("Too many points added to Q!");

		}
}

void IndexandSupport(double *Q, int Qn, double *Bary, double *Qnew, int Qnnew, double *A, int An, double *B, int Bn, double *Cart, double *V, double *ia, double *ib)
{
	double *dir, *Va, *Vb;
	dir = (double*)mxCalloc(3, sizeof(double));
	Va = (double*)mxCalloc(3, sizeof(double));
	Vb = (double*)mxCalloc(3, sizeof(double));

	int Qm = 5;
	
	LogicIndex(Q, Bary, Qn, Qm, Qnew, Qnnew);

	SupportVertex(A, An, Cart, Va, ia, dir);
	SupportVertex(B, Bn, dir, Vb, ib, Cart);

	Subtract(Va, Vb, V);

	mxFree(dir);
	mxFree(Va);
	mxFree(Vb);
}
