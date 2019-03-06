#include <ctime>
#include <string>

/*
	SelectionSort, Bubble sort have time complexity of O(n^2) at an avarage case
	Merge Sort has time complexity of O(n log n) at worst case
*/
#pragma once

class Sorting
{
public:
	static void SelectionSort(int input[], int size);
	static void BubbleSort(int input[], int size);
	static void MergeSortExecutor(int input[], int size);
	static void QuickSortExecutor(int input[], int size);
	static void QuickSortExecutor_Optimized(int input[], int size);

	static void Print(int input[], int size);

	//	Used to check time complexity
	static void Test();

	static int* generateArray(int size, int low, int high);
	static bool isSorted(int array[], int n);
	static bool isSortedDecreasing(int array[], int n);
	

		//You never want these methods acessed outside of the class, so I protected them
protected:

	static void MergeSort(int input[], int from, int to);
	static void Merge(int input[], int from, int middle, int to);
	static int Partition(int input[], int from, int to);
	static void QuickSort(int input[], int from, int to);
	static void QuickSort_Optimized(int input[], int from, int to);

	static int MedianOfThree(int input[], int first, int middle, int last);
	static int PartitionMedian(int input[], int from, int to);

	
	

};

 void Sorting::Print(int input[], int size)
 {
	 for (int i = 0; i < size; i++)
	 {
		 std::cout << input[i] << " ";
	 }

	 std::cout << std::endl;
 }
void Sorting::SelectionSort(int input[], int size)
{
	for (int i = 0; i < size - 1; i++)//Loop through container except last element
	{
		int IndexOfSmallest = i;//First number behind our "wall"

		for (int j = i + 1; j < size; j++)//the right number that will be compared, this will go all the way to the end
		{
			if (input[j] < input[IndexOfSmallest])
			{
				IndexOfSmallest = j;
			}
		}

		//swap
		int temp = input[i];
		input[i] = input[IndexOfSmallest];
		input[IndexOfSmallest] = temp;
	}
}

/*Bubble sort will trickle the largest number to the back of the array*/
void Sorting::BubbleSort(int input[], int size)
{
	for (int i = 0; i < size - 1; i++)
	{
		//i represents how many elements are correctly sorted

		bool swapped = false;//for efficiency we will stop if no numbers are sorted on first pass

		for (int j = 0; j + 1 < size - i; j++)
		{
			if (input[j] > input[j + 1])
			{
				swapped = true;
				int temp = input[j];
				input[j] = input[j + 1];
				input[j + 1] = temp;
			}
		}

		if (!swapped) // if we never swapped then the array is sorted
		{
			return;
		}
	}
}

#pragma region Merge Sort
/*Merge Sort is composed of three sperate methods, 
Merge: Handles the merging of all seperate sorted arrays
MergeSort: Will Recursively call MergeSort and Merge until the array is sorted
MergeSortExecutor: The helper function to be called in main. You dont want to create empty arrays just to use the function, so use this instead*/



/*Takes care of merging two arrays for the MergeSort
	@param lengthLeft   : Calculates the left length of the array, it contains the element in the middle, so add one to the result
	@param lengthRight  : Calculates the right length of the array, the middle is NEVER on this side.
	@param *left
	@param *right	    : arrays that will store the sorted part of their respective sides. 
	@param leftPointer
	@param rightPointer	: two ints that will point to what element we are at on each side of the array
*/
void Sorting::Merge(int input[], int from, int middle, int to)
{
	int lengthLeft = middle - from + 1; 
	int lengthRight = to - middle; 

	//The two lengths we just calculated, add one 
	int *left = new int[lengthLeft + 1];
	int *right = new int[lengthRight + 1];

	/*Next step is to fill the arrays with their corresponding elements,
	for every number in input array, starting with index from to index middle,
	we already calculated the length of the left array, so we can use it in this loop.
	we get all elements from index from to index lengthleft - 1.
	The same is done for the right side, only on this side we add elements stating from index middle + 1 and 
	go to index of middle + lengthRight.*/
	for (int i = 0; i < lengthLeft; i++)
	{
		left[i] = input[from + i];
	}

	for (int i = 0; i < lengthRight; i++)
	{
		right[i] = input[middle + 1 + i];
	}

	//Set the very last element to max integer used as stopping points
	left[lengthLeft] = INT_MAX;
	right[lengthRight] = INT_MAX;

	//Merge the two arrays
	/*since the output array is the array that is passed to this function as an argument, and we have already
	 stored the number from the part we are sorting in to left and right parts, we can re write the values in every index
	 the two ints represent at what number we are currently pointing to*/

	int leftPointer = 0;
	int rightPointer = 0;

	for (int i = from; i <= to; i++)
	{
		if (left[leftPointer] > right[rightPointer])
		{
			input[i] = right[rightPointer];
			rightPointer++;
		}
		else
		{
			input[i] = left[leftPointer];
			leftPointer++;
		}
	}
}

/*Merge Sort will recursivley split the array into two seperate parts until either pairs are present or, only 1 element is present in the right sub array. 
You then sort the numbers and merge them back together

It would be inneficinet to create new arrays for recurrsive calls. I used two index which indicate the part of the array we are sorting*/
void Sorting::MergeSort(int input[], int from, int to)
{
	//if the current array has less than 2 elements than the array its going into, its already sorted so exit.

	if (from < to)
	{
		int middle = (from + to) / 2;
		MergeSort(input, from, middle);
		MergeSort(input, middle + 1, to);
		Merge(input, from, middle, to);
	}
}

//This is what you would call in main, it will generate to and from arrays for you.
void Sorting::MergeSortExecutor(int input[], int size)
{
	MergeSort(input, 0, size - 1);
}

#pragma endregion

#pragma region Quick Sort
/*Takes an array and two indexes, the two indexes represent what part of the array we are sorting
 in initial call this should be 0, and n - 1.
 
 The last number in the array becomes a pivot point. A wall moves through the array and compares every number against the pivot point,
 smaller numbers go to the left, larger numbers on the right. When a number that is smaller than the pivot is found, it moves to the left of the wall, and the wall
 incremennts in position. Once every number is initially compared, the pivot is swapped with the first number behind our wall, guaranteeing it is in the correct position
 Now we recursivley call Quick Sort on the left and right side.*/
void Sorting::QuickSort(int input[], int from, int to)
{
	//when this is equal, there is no need to sort as we only have one number in the array
	//if more than one number, we partition
	if (from < to) 
	{
		int indexOfPivot = Sorting::Partition(input, from, to);
		Sorting::QuickSort(input, from, indexOfPivot - 1);
		Sorting::QuickSort(input, indexOfPivot + 1, to);
	}
}

void Sorting::QuickSort_Optimized(int input[], int from, int to)
{
	if (from < to)
	{
		int indexOfPivot = Sorting::PartitionMedian(input, from, to);
		Sorting::QuickSort_Optimized(input, from, indexOfPivot - 1);
		Sorting::QuickSort_Optimized(input, indexOfPivot + 1, to);
	}
}

//Helper function that will be used in main
void Sorting::QuickSortExecutor(int input[], int size)
{
	Sorting::QuickSort(input, 0, size - 1);
}

//Executes the optimized version of quicksort. The original version is kept in to experiement with time complexity
void Sorting::QuickSortExecutor_Optimized(int input[], int size)
{
	Sorting::QuickSort_Optimized(input, 0, size - 1);
}

/*(Partition takes 2 arguments, array to and from indexes and grabs the pivot, moves smaller on left side and bigger on right. 
	do reccursive calls on left and right side
	
	The best possible case we need to ensure is that pivot is always somewhere in the middle of all data, or else it 
	destroys the time complexity of this algorithm, there won't be anything on the left side to sort. This turns the time complexity into (Log n^2)*/

int Sorting::Partition(int input[], int from, int to)
{
	int pivot = input[to]; //for now pivot is always last, this is not the most effective implementation.
	int wall = from;
	for (int i = from; i < to; i++)
	{
		if (input[i] <= pivot)
		{


			int temp = input[wall];
			input[wall] = input[i];
			input[i] = temp;
			wall++;
		}
	}

	input[to] = input[wall];
	input[wall] = pivot;

	return wall;
}

//returns the median of them index of an array. used to improve time complexity in quick sort
int Sorting::MedianOfThree(int input[], int first, int middle, int last)
{
	int array[] = { input[first], input[middle], input[last] }; 
	Sorting::SelectionSort(array, 3);

	if (array[1] == input[first])
	{
		return first;
	}

	if (array[1] == input[middle])
	{
		return middle;
	}
	else
	{
		return last;
	}
}

//Same idea as other partition, the only difference is it calculates the middle value of the array, and places it at the end as the pivot
int Sorting::PartitionMedian(int input[], int from, int to)
{
	int indexOfPivot = Sorting::MedianOfThree(input, from, (from + to) / 2, to);
	int pivot = input[indexOfPivot];

	//check if pivot is last number, if its not we swap with last number, the way the algorithm is implemented it is assumed pivot is last number
	if (indexOfPivot != to)
	{
		input[indexOfPivot] = input[to];
	}

	int wall = from;
	for (int i = from; i < to; i++)
	{
		if (input[i] <= pivot)
		{
			int temp = input[wall];
			input[wall] = input[i];
			input[i] = temp;
			wall++;
		}
	}

	input[to] = input[wall];
	input[wall] = pivot;

	return wall;
}

#pragma endregion

#pragma region TimeComplexity

//Generates an array of random numbers with a specified size and number range
int* Sorting::generateArray(int size, int low, int high)
{
	int* result = new int[size];
	for (int i = 0; i < size; i++)
	{
		result[i] = low + (rand() % static_cast<int>(high - low + 1));
	}
	return result;
}

//an array is sorted if all numbers appear from low to high. we just compare side by side numbers, if at any point the left number is bigger, the array is not sorted.
bool Sorting::isSorted(int array[], int n)
{
	for (int i = 0; i < n - 1; i++)
	{
		if (array[i] > array[i + 1])
		{
			return false;
		}
	}
	return true;
}

//same idea as isSorted, but were checking to see if the right element is larger than the left, which tells us it is not in decending order.
bool Sorting::isSortedDecreasing(int array[], int n)
{
	for (int i = 0; i < n - 1; i++)
	{
		if (array[i] < array[i + 1])
		{
			return false;
		}
	}
	return true;
}

void Sorting::Test()
{
	const int SIZE_OF_TESING_ARRAY = 1000;
	const int NUMBER_OF_ARRAYS = 1000;
	const int RANGE_LOW = 1;
	const int RANGE_HIGH = 1000;

	const int NUMBER_OF_ALGORITHMS = 5; //DO NOT CHANGE UNLESS MORE ALGORITHMS ARE ADDED
	std::string functionNames[NUMBER_OF_ALGORITHMS] = { "Selection Sort", "Bubble Sort", "Merge Sort", "Quick Sort", "Quick Sort Optimized" };
	void(*functions[NUMBER_OF_ALGORITHMS])(int*, int) = { SelectionSort, BubbleSort, MergeSortExecutor, QuickSortExecutor, QuickSortExecutor_Optimized };

	for (int i = 0; i < NUMBER_OF_ALGORITHMS; i++)
	{
		bool allSorted = true;
		clock_t begin = clock();

		for (int j = 0; j < NUMBER_OF_ARRAYS; j++)
		{
			int* inputArray = generateArray(SIZE_OF_TESING_ARRAY, RANGE_LOW, RANGE_HIGH);
			int cloneArray[SIZE_OF_TESING_ARRAY];
			memcpy(cloneArray, inputArray, 4 * sizeof(int)); //copy all numbers from input array to clone array used to check input of array if it fails to sort

			(functions[i])(inputArray, SIZE_OF_TESING_ARRAY);
			if (!isSorted(inputArray, SIZE_OF_TESING_ARRAY))
			{
				allSorted = false;
				std::cout << functionNames[i] << std::endl;
				std::cout << "ERROR this array is not sorted" << std::endl;
				Print(cloneArray, SIZE_OF_TESING_ARRAY);
				std::cout << "RETURNED " << std::endl;
				Print(inputArray, SIZE_OF_TESING_ARRAY);
			}
		}

		clock_t end = clock();
		double seconds = double(end - begin) / CLOCKS_PER_SEC;

		if (allSorted)
		{
			std::cout << "**********************************************" << std::endl;
			std::cout << "Finished " << NUMBER_OF_ARRAYS << " Tests." << std::endl;
			std::cout << functionNames[i] << std::endl;
			std::cout << "Took : " << seconds << " seconds." << std::endl;
		}
	}
}

#pragma endregion