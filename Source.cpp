#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glut.h>
#include <sstream>
using namespace std;

#define SORT_NO 6  // Number of sorting algorithms
#define MAX 50	   // Number of values in the array
#define SPEED 700  // Speed of sorting, must be greater than MAX always
int a[MAX];		   // Array
int swapflag = 0;  // Flag to check if swapping has occured
int i = 0, j = 0;  // To iterate through the array
int k1;			   // To iterate through the array

int low = 0;
int high = MAX - 1;
int flag = 0;	 // For Insertion Sort
int dirflag = 0; // For Ripple Sort, to change direction at the ends
int count;		 // For Ripple Sort, to keep count of how many are sorted at the end
int k = 0;		 // To Switch from Welcome screen (landing page) to Main Screen
int sorting = 0; // 0 if sorted "or" not started yet "or" halted bcz of some reason, 1 if sorting is going on
const char* sort_string[] = { "Bubble Sort", "Heap Sort", "Insertion Sort", "Quick Sort", "Ripple Sort", "Selection Sort" };
int sort_count = 0; // To cycle through the string
int l = low;
int h = high;
// ctrl+f
// Create an auxiliary stack
int stack[MAX];
// initialize top of stack
int top = -1;
int p; //pause

// Function to display text on screen char by char
void bitmap_output(int x, int y, const char* string, void* font)
{
	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
}

// Function to convert integer to string
void int_str(int rad, string r)
{
	r = to_string(rad);
}


void display_text()
{
	glColor3f(1, 1, 1);
	bitmap_output(250, 665, "EasyAlgo, algorithms made easier", GLUT_BITMAP_HELVETICA_18);
	//glBegin(GL_POINTS);
	glVertex2f(145, 660);
	glVertex2f(520, 660);
	glEnd();

	// other text small font
	bitmap_output(10, 625, "The project EasyAlgo shows how different sorting algorithms - Bubble Sort, Heap Sort, Insertion Sort, Quick Sort, Ripple Sort &", GLUT_BITMAP_9_BY_15);
	bitmap_output(10, 605, "Selection Sort sort a random set of numbers in ascending order displaying them graphically as bars with varying height.", GLUT_BITMAP_9_BY_15);

	if (sorting == 0) // if not sorting display menu
	{
		bitmap_output(10, 575, "MENU", GLUT_BITMAP_9_BY_15);
		bitmap_output(10, 555, "Press a to SELECT the sort algorithm", GLUT_BITMAP_9_BY_15);
		bitmap_output(10, 535, "Press r to RANDOMISE", GLUT_BITMAP_9_BY_15);
		bitmap_output(10, 515, "Press s to SORT", GLUT_BITMAP_9_BY_15);
		bitmap_output(10, 495, "Esc to QUIT", GLUT_BITMAP_9_BY_15);
		bitmap_output(10, 475, "You've selected:", GLUT_BITMAP_9_BY_15);
		bitmap_output(115, 475, *(sort_string + sort_count), GLUT_BITMAP_9_BY_15);
	}
	else if (sorting == 1) // while sorting
	{
		glColor3f(0.5, 0.5, 0.5);
		bitmap_output(10, 555, "Sorting in progress...", GLUT_BITMAP_9_BY_15);
		bitmap_output(10, 535, "Press p to PAUSE", GLUT_BITMAP_9_BY_15);
		bitmap_output(10, 515, "Press s to resume sorting", GLUT_BITMAP_9_BY_15);
		glColor3f(0.0, 0.0, 0.0);
	}
}

void front()
{
	glColor3f(1.0, 1.0, 1.0);
	bitmap_output(310, 565, "EasyAlgo", GLUT_BITMAP_TIMES_ROMAN_24);
	glVertex2f(295, 560);
	glVertex2f(395, 560);
	glEnd();
	bitmap_output(280, 515, "", GLUT_BITMAP_TIMES_ROMAN_24);
	glVertex2f(325, 521);
	glVertex2f(360, 521);
	glEnd();
	bitmap_output(230, 450, "Made by: Purnima Lal", GLUT_BITMAP_TIMES_ROMAN_24);
	glVertex2f(145, 470);
	glVertex2f(520, 470);
	glEnd();

	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_QUADS);
	glVertex2f(520, 120.0);
	glVertex2f(520, 170);
	glVertex2f(796, 170);
	glVertex2f(796, 120.0);
	glEnd();
	glColor3f(0.0, 0.0, 0.0);
	bitmap_output(530, 125, "Press Enter to continue...", GLUT_BITMAP_TIMES_ROMAN_24);
}

void heapify(int arr[], int n, int i);

void Initialize()
{
	int temp1;

	// Reset the array
	for (temp1 = 0; temp1 < MAX; temp1++)
	{
		a[temp1] = rand() % 100 + 1;
		printf("%d ", a[temp1]);
	}
	// Build heap (rearrange array)
	for (int i = MAX / 2 - 1; i >= 0; i--)
		heapify(a, MAX, i);

	// Reset all values
	i = j = 0;
	dirflag = 0;
	//count = 1;
	flag = 0;
	k1 = MAX - 1;
	// push initial values of l and h to stack
	stack[++top] = l;
	stack[++top] = h;

	glClearColor(1, 1, 1, 1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 699, 0, 799);
}

// Return 1 if not sorted, 0 if sorted
int notsorted()
{
	int q;
	for (q = 0; q < MAX - 1; q++)
	{
		if (a[q] > a[q + 1])
			return 1;
	}
	return 0;
}

// Main function for display
void display()
{
	int ix, temp;
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);

	if (k == 0)
		front();
	else
	{
		display_text();
		char text[10];

		for (ix = 0; ix < MAX; ix++)
		{
			// 66, 239, 245
			glColor3f(0.25, 0.93, 0.96);
			//glBegin(GL_POLYGON);
			glBegin(GL_LINE_LOOP);
			glVertex2f(10 + (700 / (MAX + 1)) * ix, 50);
			glVertex2f(10 + (700 / (MAX + 1)) * (ix + 1), 50);
			glVertex2f(10 + (700 / (MAX + 1)) * (ix + 1), 50 + a[ix] * 4);
			glVertex2f(10 + (700 / (MAX + 1)) * ix, 50 + a[ix] * 4);
			glEnd();

			int_str(a[ix], text);
			glColor3f(0, 0, 0);
			bitmap_output(12 + (700 / (MAX + 1)) * ix, 35, text, GLUT_BITMAP_TIMES_ROMAN_10);
		}

		if (swapflag || sorting == 0)
		{
			glColor3f(1, 1, 1);
			glBegin(GL_POLYGON);
			glVertex2f(10 + (700 / (MAX + 1)) * j, 50);
			glVertex2f(10 + (700 / (MAX + 1)) * (j + 1), 50);
			glVertex2f(10 + (700 / (MAX + 1)) * (j + 1), 50 + a[j] * 4);
			glVertex2f(10 + (700 / (MAX + 1)) * j, 50 + a[j] * 4);
			glEnd();
			swapflag = 0;
		}
	}
	glFinish();
}

//Bubble Sort
void bubblesort()
{
	int temp;
	while (notsorted())
	{
		while (j < MAX - 1)
		{
			if (a[j] > a[j + 1])
			{
				swapflag = 1;
				temp = a[j];
				a[j] = a[j + 1];
				a[j + 1] = temp;

				goto A;
			}
			j++;
			if (j == MAX - 1)
				j = 0;
			printf("swap %d and %d\n", a[j], a[j + 1]);
		}
	}
	sorting = 0;
	i = j = 0;
A:
	printf("");
}

// Heap Sort
#include <iostream>
using namespace std;

void heapify(int arr[], int n, int i)
{
	int temp;
	int largest = i;   // Initialize largest as root
	int l = 2 * i + 1; // left = 2*i + 1
	int r = 2 * i + 2; // right = 2*i + 2

	// If left child is larger than root
	if (l < n && arr[l] > arr[largest])
		largest = l;

	// If right child is larger than largest so far
	if (r < n && arr[r] > arr[largest])
		largest = r;

	// If largest is not root
	if (largest != i)
	{
		swapflag = 1;
		temp = arr[i];
		arr[i] = arr[largest];
		arr[largest] = temp;
		printf("\nswapping %d and %d", arr[i], arr[largest]);
		//swap(arr[i], arr[largest]);
		// Recursively heapify the affected sub-tree
		heapify(arr, n, largest);
	}
}
// main function to do heap sort
void heapSort()
{
	int temp;

	while (notsorted())
	{

		// One by one extract an element from heap
		while (k1 > 0)
		{
			//for (int i = MAX - 1; i > 0; i--) {
			// Move current root to end
			//swap(a[0], a[i]);
			swapflag = 1;
			j = k1;
			temp = a[0];
			a[0] = a[j];
			a[j] = temp;
			printf("swap %d and %d\n", a[0], a[k1]);
			// call max heapify on the reduced heap
			heapify(a, k1, 0);
			k1--;
			goto A;
		}
	}
	sorting = 0;
	k1 = MAX - 1;
	j = 0;
A:
	printf("");
}

// Insertion Sort
void insertionsort()
{
	int temp;
	j = 0;

	while (i < MAX)
	{
		if (flag == 0)
		{
			j = i;
			flag = 1;
		}
		while (j < MAX - 1)
		{
			if (a[j] > a[j + 1])
			{
				swapflag = 1;
				temp = a[j];
				a[j] = a[j + 1];
				a[j + 1] = temp;

				goto A;
			}
			j++;
			if (j == MAX - 1)
			{
				flag = 0;
			}
			printf("swap %d and %d\n", a[j], a[j + 1]);
		}
		i++;
	}
	sorting = 0;
	i = j = 0;

A:
	printf("");
}

// Quick Sort
// A utility function to swap two elements
void swap(int* a, int* b)
{
	int t = *a;
	*a = *b;
	*b = t;
}

/* This function is same in both iterative and recursive*/
int partition(int arr[], int l, int h)
{
	int x = arr[l];
	int i = (h + 1);

	for (int j = h; j >= l + 1; j--)
	{
		if (arr[j] >= x)
		{
			i--;
			swap(&arr[i], &arr[j]);
		}
	}
	swap(&arr[i - 1], &arr[l]);
	return (i - 1);
}

/* A[] --> Array to be sorted,
l --> Starting index,
h --> Ending index */

void quickSort()
{
	printf("quick sort\n");

	// Keep popping from stack while is not empty
	while (top >= 0)
	{
		// Pop h and l

		h = stack[top--];
		l = stack[top--];

		// Set pivot element at its correct position
		// in sorted array
		// p = partition(a, l, h);
		int x = a[l];
		int i = (h + 1);

		for (int j = h; j >= l + 1; j--)
		{
			if (a[j] >= x)
			{
				i--;
				swap(&a[i], &a[j]);
			}
		}
		swap(&a[i - 1], &a[l]);
		p = (i - 1);
		j = p;
		printf("%d", j);

		// If there are elements on left side of pivot,
		// then push left side to stack
		if (p - 1 > l)
		{
			stack[++top] = l;
			stack[++top] = p - 1;
		}

		// If there are elements on right side of pivot,
		// then push right side to stack
		if (p + 1 < h)
		{
			stack[++top] = p + 1;
			stack[++top] = h;
		}
		goto A;
	}
	sorting = 0;
	l = low;
	h = high;
A:
	swapflag = 1;
	printf("");
}

//Ripple Sort
void ripplesort()
{
	int temp;
	int count = 1;
	while (notsorted() && sorting)
	{
		if (dirflag == 0)
		{
			while (j < MAX - 1)
			{
				if (a[j] > a[j + 1])
				{
					swapflag = 1;
					temp = a[j];
					a[j] = a[j + 1];
					a[j + 1] = temp;

					goto A;
				}
				j++;
				if (j == MAX - 1)
				{
					count++;
					j = MAX - count;
					dirflag = 1 - dirflag;
				}
				printf("j=%d forward swap %d and %d\n", j, a[j], a[j + 1]);
			}
		}
		else
		{
			while (j >= 0)
			{
				if (a[j] > a[j + 1])
				{
					swapflag = 1;
					temp = a[j];
					a[j] = a[j + 1];
					a[j + 1] = temp;

					goto A;
				}
				j--;
				if (j == 0)
				{
					dirflag = 1 - dirflag;
				}
				printf("j=%d backward swap %d and %d\n", j, a[j], a[j + 1]);
			}
		}
	}
	sorting = 0;
A:
	printf("");
}

// Selection Sort
void selectionsort()
{
	int temp, min, pos;

	while (notsorted())
	{

		while (i < MAX - 1)
		{
			min = a[i + 1];
			pos = i + 1;
			if (i != MAX - 1)
			{
				for (int j1 = i + 2; j1 < MAX; j1++)
				{
					if (min > a[j1])
					{
						min = a[j1];
						pos = j1;
					}
				}
			}
			printf("\ni=%d min=%d at %d", i, min, pos);
			printf("\nchecking %d and %d", min, a[i]);
			swapflag = 0;
			if (min < a[i])
			{
				j = i;
				swapflag = 1;
				//j=pos;
				printf("\nswapping %d and %d", min, a[i]);
				temp = a[pos];
				a[pos] = a[j];
				a[j] = temp;
				goto A;
			}
			i++;
		}
	}
	sorting = 0;
	i = j = 0;
A:
	printf("");
}

// Timer Function, takes care of slection sorting algorithm
void makedelay(int)
{
	if (sorting)
	{
		switch (sort_count)
		{
		case 0:
			bubblesort();
			break;
		case 1:
			heapSort();
			break;
		case 2:
			insertionsort();
			break;
		case 3:
			quickSort();
			break;
		case 4:
			ripplesort();
			break;
		case 5:
			selectionsort();
			break;
		}
	}
	glutPostRedisplay();
	glutTimerFunc(SPEED / MAX, makedelay, 1);
}

// Keyboard Function
void keyboard(unsigned char key, int x, int y)
{
	if (key == 13)
		k = 1;
	if (k == 1 && sorting != 1)
	{
		switch (key)
		{
		case 27:
			exit(0); // 27 is the ascii code for the ESC key
		case 's':
			sorting = 1;
			break;
		case 'r':
			Initialize();
			break;
		case 'a':
			sort_count = (sort_count + 1) % SORT_NO;
			break;
		}
	}
	if (k == 1 && sorting == 1)
		if (key == 'p')
			sorting = 0;
}

// Main Function
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(1000, 600);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("EasyAlgo | Dynamic Sorting Algo");
	Initialize();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(1000, makedelay, 1);
	glutMainLoop();
}
