#include "Header.h"

#define SORT_SIZE	2048
#define NUM_THREADS (SORT_SIZE/2)
//#define ITERATIONS = (HALF_SIZE > 1024 ? HALF_SIZE/1024 : 1)

RWStructuredBuffer<AliveData> Data;


cbuffer SortConstantsBuffer
{
	int numElements;
	int compareDistance;
	int2 sortConstants;
};

groupshared AliveData groupData[SORT_SIZE];


inline int2 getSortIndex(int compareDistance, int GI, int2 sortConstants)
{
	int indexLow = GI & (compareDistance - 1);
	int indexHigh = 2 * (GI - indexLow);

	int index = indexHigh + indexLow;
	int swapElem = indexHigh + sortConstants.x + sortConstants.y * indexLow;

	return int2(index, swapElem);

}


[numthreads(NUM_THREADS,1,1)]
void CSSortInitial(uint3 Gid : SV_GroupID,
	uint3 DTid : SV_DispatchThreadID,
	uint3 GTid : SV_GroupThreadID,
	uint GI : SV_GroupIndex)
{
	// index to global data buffer
	int globalBaseIndex = (Gid.x * SORT_SIZE) + GTid.x;
	// index to group data buffer
	int localBaseIndex = GI;

	int numElementsInThreadGroup = min(SORT_SIZE, numElements - (Gid.x * SORT_SIZE));

	// load shared data

	for (int i = 0; i < 2; ++i)
	{
		if (localBaseIndex + i * NUM_THREADS < numElementsInThreadGroup)
			groupData[localBaseIndex + i * NUM_THREADS] = Data[globalBaseIndex + i * NUM_THREADS];
	}
	GroupMemoryBarrierWithGroupSync();


	// sort

	for (int subArraySize = 2; subArraySize <= SORT_SIZE; subArraySize = subArraySize *2)
	{
		int compareDistance = subArraySize >> 1;
		
		int2 indices = getSortIndex(compareDistance, GI, int2(2*compareDistance-1, -1));

		if (indices.y < numElementsInThreadGroup)
		{
			AliveData a = groupData[indices.x];
			AliveData b = groupData[indices.y];
			if (a.depth < b.depth)
			{
				groupData[indices.x] = b;
				groupData[indices.y] = a;
			}
		}
		GroupMemoryBarrierWithGroupSync();


		int2 sortConstants;
		for (compareDistance = compareDistance >> 1; compareDistance > 0; compareDistance = compareDistance >> 1)
		{
			sortConstants = int2(compareDistance, 1);
			indices = getSortIndex(compareDistance, GI, sortConstants);

			if (indices.y < numElementsInThreadGroup)
			{
				AliveData a = groupData[indices.x];
				AliveData b = groupData[indices.y];
				if (a.depth < b.depth)
				{
					groupData[indices.x] = b;
					groupData[indices.y] = a;
				}
			}

			GroupMemoryBarrierWithGroupSync();
		}
	}

	//store shared data
	for (int i = 0; i < 2; ++i)
	{
		if (localBaseIndex + i * NUM_THREADS < numElementsInThreadGroup)
			Data[globalBaseIndex + i * NUM_THREADS] = groupData[localBaseIndex + i * NUM_THREADS];
	}

}


[numthreads(NUM_THREADS, 1, 1)]
void CSSortOuter(uint3 Gid : SV_GroupID,
	uint3 GTid : SV_GroupThreadID)
{

	int localID = Gid.x * NUM_THREADS + GTid.x;
	int2 indices = getSortIndex(compareDistance, localID, sortConstants);

	if (indices.y < numElements)
	{
		AliveData a = Data[indices.x];
		AliveData b = Data[indices.y];
		if (a.depth < b.depth)
		{
			Data[indices.x] = b;
			Data[indices.y] = a;
		}
	}

}


[numthreads(NUM_THREADS, 1, 1)]
void CSSortInner(uint3 Gid : SV_GroupID,
	uint3 DTid : SV_DispatchThreadID,
	uint3 GTid : SV_GroupThreadID,
	uint GI : SV_GroupIndex)
{


	int globalBaseIndex = Gid.x * SORT_SIZE + GTid.x;
	int localBaseIndex = GI;

	int numElementsInThreadGroup = min(SORT_SIZE, numElements.x - (Gid.x * SORT_SIZE));

	//load shared data
	for (int i = 0; i < 2; ++i)
	{
		if (GI + i * NUM_THREADS < (int)numElementsInThreadGroup)
			groupData[localBaseIndex + i * NUM_THREADS] = Data[globalBaseIndex + i * NUM_THREADS];
	}
	GroupMemoryBarrierWithGroupSync();

	// sort
	for (int compareDistance = SORT_SIZE >> 1; compareDistance > 0; compareDistance = compareDistance >> 1)
	{
		int2 sortConstants = int2(compareDistance, 1);
		int2 indices = getSortIndex(compareDistance, GI, sortConstants);

		if (indices.y < numElementsInThreadGroup)
		{
			AliveData a = groupData[indices.x];
			AliveData b = groupData[indices.y];
			if (a.depth < b.depth)
			{
				groupData[indices.x] = b;
				groupData[indices.y] = a;
			}
		}
		GroupMemoryBarrierWithGroupSync();
	}

	//store shared data
	for (int i = 0; i < 2; ++i)
	{
		if (GI + i * NUM_THREADS < (int)numElementsInThreadGroup)
			Data[globalBaseIndex + i * NUM_THREADS] = groupData[localBaseIndex + i * NUM_THREADS];
	}
}