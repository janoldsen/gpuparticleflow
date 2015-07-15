#ifndef CLOCK_H
#define CLOCK_H


class Clock
{
public:
	Clock();
	
	void startNewFrame();

	float getElapsedTime();
	__int64 getCurrTime();

	float getTotalElapsedTime();


private:
	
	double frq;

	__int64 startTime;
	__int64 currTime;
	__int64 lastTime;

};



#endif