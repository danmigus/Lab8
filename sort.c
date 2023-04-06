#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

void merge(int pData[], int l, int m, int r);

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapify(int arr[], int size, int currentParent)
{
	int temp;
	int rootNode = currentParent;
	int leftNode = 2*currentParent +1;
	int rightNode = 2*currentParent +2;

	if (rightNode < size)
		if (arr[rightNode] > arr[rootNode])
			rootNode = rightNode;

	if (leftNode < size)
		if (arr[leftNode] > arr[rootNode])
			rootNode = leftNode;

	// Swap the highest valued child with rootNode (aka temp currentParent)
	if (rootNode != currentParent)
	{
		temp = arr[currentParent];
		arr[currentParent] = arr[rootNode];
		arr[rootNode] = temp;

		heapify(arr, size, rootNode);
	}
}

void moveMaxAndHeapify(int arr[], int size, int lastLeafIndex)
{
	int temp;

	while (lastLeafIndex != -1)
	{
		// Similar to bubble sort in the sense that it's 
		// pushing the largest number to the end of the array.
		// Each iteration, another last node is being ignored.
		temp = arr[0];
		arr[0] = arr[lastLeafIndex];
		arr[lastLeafIndex] = temp;

		heapify(arr, lastLeafIndex, 0);

		lastLeafIndex--;
	}
}


void heapSort(int arr[], int size)
{
	// Part 1: Heapify unorganized tree into a max heap tree (in array form). O(log n)
	int lastSubtreeIndex = size/2 - 1;
	int lastLeafIndex = size - 1;
	int temp;

	while (lastSubtreeIndex != -1)
	{
		heapify(arr, size, lastSubtreeIndex);
		lastSubtreeIndex--;
	}

	// Part 2: Perform swaps of last leaf node with root(max) recursively. O(n)
	moveMaxAndHeapify(arr, size, lastLeafIndex);
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
	if (l < r)
	{
		int m = (l + r)/2;

		mergeSort(pData, l, m);
		mergeSort(pData, m + 1, r);

		merge(pData, l, m, r);
	}
}

void merge(int pData[], int l, int m, int r)
{
    int partition1 = m - l + 1;
    int partition2 = r - m;

    int *L = (int*) malloc(partition1*sizeof(int));
    int *R = (int*) malloc(partition2*sizeof(int));

	extraMemoryAllocated += partition1* (sizeof(int)) + partition2* (sizeof(int));

    for (int i = 0; i < partition1; i++)
        L[i] = pData[l + i];
    for (int j = 0; j < partition2; j++)
        R[j] = pData[m + 1 + j];

    int k = l; 
	int i = 0; int j = 0;

    while (i < partition1 && j < partition2)
    {
        if (L[i] <= R[j])
        {
            pData[k] = L[i];
            i++;
        }
        else
        {
            pData[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < partition1)
    {
        pData[k] = L[i];
        i++;
        k++;
    }

    while (j < partition2)
    {
        pData[k] = R[j];
        j++;
        k++;
    }
	
    free(L);
    free(R);
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i=0;i<dataSz;++i)
		{
			fscanf(inFile, "%d ",&n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	if (dataSz < 200)
	{
		for (int i=0;i<dataSz;++i)
		{
			printf("%d ",pData[i]);
		}
		printf("\n\t");

		return;
	}

	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };
	
	for (i=0;i<4;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)malloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		free(pDataCopy);
		free(pDataSrc);
	}
	
}