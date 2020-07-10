#include <algorithm>
#include <iostream>
#include <utility>
#include <string>
#include <iterator>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <vector>
#include <unordered_map>
#include <map>
#include <set>
#include <unordered_set>
#include <stack>
#include <queue>
#define fast                          \
	ios_base::sync_with_stdio(false); \
	cin.tie(NULL);
#define endl '\n'
#define ll long long int
using namespace std;

struct point
{
	int x, y;
	int index;
};

struct distancer
{
	double area;
	int index;
};

// A global point needed for  sorting points with reference
// to  the first point Used in compare function of qsort()
point p0;

// A utility function to find next to top in a stack
point nextToTop(stack<point> &S)
{
	point p = S.top();
	S.pop();
	point res = S.top();
	S.push(p);
	return res;
}

// A utility function to swap two points
void swap(point &p1, point &p2)
{
	point temp = p1;
	p1 = p2;
	p2 = temp;
}

// A utility function to return square of distance
// between p1 and p2
int distSq(point p1, point p2)
{
	return (p1.x - p2.x) * (p1.x - p2.x) +
		   (p1.y - p2.y) * (p1.y - p2.y);
}

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int orientation(point p, point q, point r)
{
	int val = (q.y - p.y) * (r.x - q.x) -
			  (q.x - p.x) * (r.y - q.y);

	if (val == 0)
		return 0;			  // colinear
	return (val > 0) ? 1 : 2; // clock or counterclock wise
}

// A function used by library function qsort() to sort an array of
// points with respect to the first point
int compare(const void *vp1, const void *vp2)
{
	point *p1 = (point *)vp1;
	point *p2 = (point *)vp2;

	// Find orientation
	int o = orientation(p0, *p1, *p2);
	if (o == 0)
		return (distSq(p0, *p2) >= distSq(p0, *p1)) ? -1 : 1;

	return (o == 2) ? -1 : 1;
}

// Prints convex hull of a set of n points.
vector<point> convexHull(point *points, int n)
{
	// Find the bottommost point
	int ymin = points[0].y, min = 0;
	for (int i = 1; i < n; i++)
	{
		int y = points[i].y;

		// Pick the bottom-most or chose the left
		// most point in case of tie
		if ((y < ymin) || (ymin == y &&
						   points[i].x < points[min].x))
			ymin = points[i].y, min = i;
	}

	// Place the bottom-most point at first position
	swap(points[0], points[min]);

	// Sort n-1 points with respect to the first point.
	// A point p1 comes before p2 in sorted output if p2
	// has larger polar angle (in counterclockwise
	// direction) than p1
	p0 = points[0];
	qsort(&points[1], n - 1, sizeof(point), compare);

	// If two or more points make same angle with p0,
	// Remove all but the one that is farthest from p0
	// Remember that, in above sorting, our criteria was
	// to keep the farthest point at the end when more than
	// one points have same angle.
	int m = 1; // Initialize size of modified array
	for (int i = 1; i < n; i++)
	{
		// Keep removing i while angle of i and i+1 is same
		// with respect to p0
		while (i < n - 1 && orientation(p0, points[i],
										points[i + 1]) == 0)
			i++;

		points[m] = points[i];
		m++; // Update size of modified array
	}

	// If modified array of points has less than 3 points,
	// convex hull is not possible
	if (m < 3)
	{
		vector<point> empty_vector;
		return empty_vector;
	}

	// Create an empty stack and push first three points
	// to it.
	stack<point> S;
	S.push(points[0]);
	S.push(points[1]);
	S.push(points[2]);

	// Process remaining n-3 points
	for (int i = 3; i < m; i++)
	{
		// Keep removing top while the angle formed by
		// points next-to-top, top, and points[i] makes
		// a non-left turn
		while (orientation(nextToTop(S), S.top(), points[i]) != 2)
			S.pop();
		S.push(points[i]);
	}

	// Now stack has the output points, return contents of stack
	vector<point> final_vector;
	while (!S.empty())
	{
		point p = S.top();
		final_vector.push_back(p);
		S.pop();
	}
	return final_vector;
}

void print(vector<point> v)
{
	cout << endl;
	for (int i = 0; i < v.size(); i++)
	{
		cout << v[i].x << " " << v[i].y << " " << v[i].index << endl;
	}
	cout << endl;
}

bool find_point(vector<point> v, point p)
{
	for (int i = 0; i < v.size(); i++)
	{
		if (v[i].x == p.x && v[i].y == p.y)
		{
			return true;
		}
	}
	return false;
}
double distance(point p1, point p2)
{
	int x1 = p1.x;
	int y1 = p1.y;
	int x2 = p2.x;
	int y2 = p2.y;
	// Calculating distance
	return sqrt(pow(x2 - x1, 2) +
				pow(y2 - y1, 2) * 1.0);
}
// Function to return the minimum distance
// between a line segment AB and a point E
double minDistance(point A, point B, point E)
{

	// vector AB
	point AB;
	AB.x = B.x - A.x;
	AB.y = B.y - A.y;

	// vector BP
	point BE;
	BE.x = E.x - B.x;
	BE.y = E.y - B.y;

	// vector AP
	point AE;
	AE.x = E.x - A.x,
	AE.y = E.y - A.y;

	// Variabley to ytore dot product
	double AB_BE, AB_AE;

	// Calculating the dot product
	AB_BE = (AB.x * BE.x + AB.y * BE.y);
	AB_AE = (AB.x * AE.x + AB.y * AE.y);

	// Minimum distance xrom
	// point E to the line segment
	double reqAns = 0;

	// Case 1
	if (AB_BE > 0)
	{

		// Finding the magnitude
		double y = E.y - B.y;
		double x = E.x - B.x;
		reqAns = sqrt(x * x + y * y);
	}

	// Case 2
	else if (AB_AE < 0)
	{
		double y = E.y - A.y;
		double x = E.x - A.x;
		reqAns = sqrt(x * x + y * y);
	}

	// Case 3
	else
	{

		// Finding the perpendicular distance
		double x1 = AB.x;
		double y1 = AB.y;
		double x2 = AE.x;
		double y2 = AE.y;
		double mod = sqrt(x1 * x1 + y1 * y1);
		reqAns = abs(x1 * y2 - y1 * x2) / mod;
	}
	return reqAns;
} //minDistance, distance
bool areaSorter(distancer a, distancer b)
{
	if(a.area>b.area)
	{
		return true;
	}
	else if(a.area==b.area)
	{
		return true;
	}
	else
	{
		return false;
	}
	
}
void perfect_polygon(point *arr, int n, int m, vector<point> hull)
{
	vector<distancer> hullsArea;
	int hull_size = hull.size();
	distancer d;
	d.area = (0.5) * (distance(hull[hull_size - 1], hull[1])) * (minDistance(hull[hull_size - 1], hull[1], hull[0]));
	d.index = hull[0].index;
	hullsArea.push_back(d);

	for (int i = 1; i < hull_size - 1; i++)
	{
		d.area = (0.5) * (distance(hull[i - 1], hull[i + 1])) * (minDistance(hull[i - 1], hull[i + 1], hull[i]));
		d.index = hull[i].index;
		hullsArea.push_back(d);
	}

	d.area = (0.5) * (distance(hull[hull_size - 2], hull[0])) * (minDistance(hull[hull_size - 2], hull[0], hull[hull_size-1]));
	d.index = hull[hull_size-1].index;
	hullsArea.push_back(d);

	sort(hullsArea.begin(), hullsArea.end(), areaSorter);

	for(int i=0; i<hullsArea.size(); i++)
	{
		cout<<hullsArea[i].index<<" ";
	}
	cout<<endl;
}

void imperfect_polygon(point *arr, int n, int m, vector<point> hull, vector<point> insideHull)
{
}

void solve(point *arr, int n, int m)
{
	//keeping a copy of arr
	point *temp = new point[n];
	for (int i = 0; i < n; i++)
	{
		temp[i] = arr[i];
	}
	/* all the convex hull points stored in hull vector */
	vector<point> hull = convexHull(temp, n);
	print(hull);
	vector<point> insideHull;
	for (int i = 0; i < n; i++)
	{
		if (!find_point(hull, arr[i]))
		{
			insideHull.push_back(arr[i]);
		}
	}

	if (insideHull.size() == 0)
	{
		perfect_polygon(arr, n, m, hull);
	}
	else
	{
		imperfect_polygon(arr, n, m, hull, insideHull);
	}
}

// Driver program to test above functions
int32_t main()
{
	fast;
	int n;
	cin >> n;
	point *arr = new point[n];
	for (int i = 0; i < n; i++)
	{
		cin >> arr[i].x >> arr[i].y;
		arr[i].index = i + 1;
	}
	int m;
	cin >> m;
	solve(arr, n, m);
}