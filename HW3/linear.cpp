
// Return false if the somePredicate function returns false for at
// least one of the array elements; return true otherwise.
bool allTrue(const double a[], int n)
{
	if (n <= 0)
		return true;
	else
	{
		if (!somePredicate(a[n - 1]))
			return false;
		else
			return allTrue(a, n - 1);
	}
}

// Return the number of elements in the array for which the
// somePredicate function returns false.
int countFalse(const double a[], int n)
{
	if (n <= 0)
		return 0;

	int part1 = 0;

	if (!somePredicate(a[n - 1]))
		part1 = 1;

	int part2 = countFalse(a, n - 1);

	return part1 + part2;


}

// Return the subscript of the first element in the array for which
// the somePredicate function returns false.  If there is no such
// element, return -1.
int firstFalse(const double a[], int n)
{
	if (n <= 0) 
		return -1;
	if (!somePredicate(a[0])) 
		return 0;
	else {
		int result = firstFalse(a + 1, n - 1);
		if (result == -1) 
			return -1;
		return result + 1;
	}
}

// Return the subscript of the smallest double in the array (i.e.,
// the one whose value is <= the value of all elements).  If more
// than one element has the same smallest value, return the smallest
// subscript of such an element.  If the array has no elements to
// examine, return -1.
int indexOfMin(const double a[], int n)
{
	if (n <= 0)
		return -1;
	if (n == 1)
		return 0;
	if (a[0] > a[indexOfMin(a + 1, n - 1) + 1])
		return indexOfMin(a + 1, n - 1) + 1;
	else
		return 0;
}

bool includes(const double a1[], int n1, const double a2[], int n2)
{
	if (n2 <= 0)
		return true;
	if (n1 <= 0 && n2 > 0)
		return false;

	if (a1[0] == a2[0] && includes(a1 + 1, n1 - 1, a2 + 1, n2 - 1))
	{
		return true;
	}
	else 
	{
		if (includes(a1 + 1, n1 - 1, a2, n2))
			return true;
		else
			return false;
	}
}



