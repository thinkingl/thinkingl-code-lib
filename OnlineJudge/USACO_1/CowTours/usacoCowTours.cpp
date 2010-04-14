#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#define INF    (1e40)

typedef struct Point Point;
struct Point {
	int x, y;
};

#define MAXPASTURE 150

int n;
double dist[MAXPASTURE][MAXPASTURE];
double diam[MAXPASTURE];
double fielddiam[MAXPASTURE];
Point pt[MAXPASTURE];
int field[MAXPASTURE];
int nfield;

double
ptdist(Point a, Point b)
{
	return sqrt((double)(b.x-a.x)*(b.x-a.x)+(double)(b.y-a.y)*(b.y-a.y));
}

/* mark the field containing pasture i with number m */
void
mark(int i, int m)
{
	int j;
	if(field[i] != -1) {
		assert(field[i] == m);
		return;
	}

	field[i] = m;
	for(j=0; j<n; j++)
		if(dist[i][j] < INF/2)
			mark(j, m);
}

void
main(void)
{
	FILE *fin, *fout;
	int i, j, k, c;
	double newdiam, d;

	fin = fopen("cowtour.in", "r");
	fout = fopen("cowtour.out", "w");
	assert(fin != NULL && fout != NULL);

	fscanf(fin, "%d\n", &n);
	for(i=0; i<n; i++)
		fscanf(fin, "%d %d\n", &pt[i].x, &pt[i].y);

	for(i=0; i<n; i++) {
		for(j=0; j<n; j++) {
			c = getc(fin);
			if(i == j)
				dist[i][j] = 0;
			else if(c == '0')
				dist[i][j] = INF;        /* a lot */
			else
				dist[i][j] = ptdist(pt[i], pt[j]);
		}
		assert(getc(fin) == '\n');
	}

	/* Floyd-Warshall all pairs shortest paths */
	for(k=0; k<n; k++)
		for(i=0; i<n; i++)
			for(j=0; j<n; j++)
				if(dist[i][k]+dist[k][j] < dist[i][j])
					dist[i][j] = dist[i][k]+dist[k][j];

	/* mark fields */
	for(i=0; i<n; i++)
		field[i] = -1;
	for(i=0; i<n; i++)
		if(field[i] == -1)
			mark(i, nfield++);

	/* find worst diameters involving pasture i, and for whole field */
	for(i=0; i<n; i++) {
		for(j=0; j<n; j++)
			if(diam[i] < dist[i][j] && dist[i][j] < INF/2)
				diam[i] = dist[i][j];
		if(diam[i] > fielddiam[field[i]])
			fielddiam[field[i]] = diam[i];
	}

	/* consider a new path between i and j */
	newdiam = INF;
	for(i=0; i<n; i++)
		for(j=0; j<n; j++) {
			if(field[i] == field[j])
				continue;

			d = diam[i]+diam[j]+ptdist(pt[i], pt[j]);
			if(d < fielddiam[field[i]])
				d = fielddiam[field[i]];
			if(d < fielddiam[field[j]])
				d = fielddiam[field[j]];

			if(d < newdiam)
				newdiam = d;
		}

		fprintf(fout, "%.6lf\n", newdiam);
		exit(0);
}