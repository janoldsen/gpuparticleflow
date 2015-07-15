#include "Clock.h"
#include <windows.h>


Clock::Clock()
{
	LARGE_INTEGER frequency;


	QueryPerformanceFrequency(&frequency);


	frq = double(frequency.QuadPart);

	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);

	currTime = time.QuadPart;

	startTime = currTime;

}


void Clock::startNewFrame()
{
	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);

	lastTime = currTime;

	currTime = time.QuadPart;
}


float Clock::getElapsedTime()
{
	//LARGE_INTEGER time;
	//QueryPerformanceCounter(&time);

	return double(currTime - lastTime) / frq;

}

float Clock::getTotalElapsedTime()
{

	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);



	return double(time.QuadPart - startTime) / frq;

}


__int64 Clock::getCurrTime()
{
	return currTime;
}