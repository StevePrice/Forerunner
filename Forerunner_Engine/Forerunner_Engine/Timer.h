/*
Creator: Haval Ahmed
Notice: All Code is © Haval Ahmed
Description: Timer Header File for Forerunner Engine
*/

#ifndef _TIMER_H_
#define _TIMER_H


class Timer
{
public:
	// Default Constructor and De-Constructor
	Timer();
	virtual ~Timer();

	// Functions: totalTime, deltaTime, Reset, Start, Pause, Count
	float totalTime() const; // Total Time In Seconds
	float deltaTime() const; // Delta Time In Seconds
	void reset(); // Resets Timer
	void start(); // Starts Timer
	void pause(); // Pause Timer
	void count(); // Count Frames

private:
	// Boolean For Time Stoppage
	bool timeStopped;

	// Overall Counter for Seconds and Delta Time
	double mainCounterPerSeconds;
	double mainDeltaTime;

	// 64 Bit Ints for Time Measurements
	__int64 mainOverallTime;
	__int64 mainPausedTime;
	__int64 mainStoppedTime;
	__int64 mainPreviousTime;
	__int64 mainCurrentTime;

};

#endif // !_TIMER_H_