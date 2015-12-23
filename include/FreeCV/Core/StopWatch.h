/*
 * StopWatch.h
 *
 *  Created on: 12.11.2015
 *      Author: andreas
 */

#ifndef FREECV_INCLUDE_FREECV_CORE_STOPWATCH_H_
#define FREECV_INCLUDE_FREECV_CORE_STOPWATCH_H_
#include <iostream>
#include <string.h>
#include <sys/time.h>
#include "FreeCV/Core/Logger.h"

namespace fcv {

class StopWatch {
public:
	StopWatch()
	{
		m_name = "unnamed";
		m_logOnDestroy = true;
		m_startTime = -1;
		m_endTime = -1;
		m_isRunning = false;

		start();
	}
	/**
	 * Creates a stopwatch with the specified name.
	 * @param autoStart: Starts the stop watch after construction
	 * @param logOnDestroy: Prints a log message with the elapsed time when the object will be destroyed
	 */
	StopWatch(std::string name, bool autoStart = true, bool logOnDestroy = true)
	{
		m_name = name;
		m_logOnDestroy = logOnDestroy;
		m_startTime = -1;
		m_endTime = -1;
		m_isRunning = false;

		if(autoStart)
			start();
	}

	virtual ~StopWatch(){
		if(m_logOnDestroy)
		{
			float msTime = getTimeDelta();
			if(msTime < 1000)
				LOG_FORMAT_INFO("Stopwatch %s: %.0f us elapsed.", m_name.c_str(), msTime);
			else if(msTime < 1000000L)
				LOG_FORMAT_INFO("Stopwatch %s: %.3f ms elapsed.", m_name.c_str(), msTime/1000);
			else if(msTime < 1000000L*60)
				LOG_FORMAT_INFO("Stopwatch %s: %.3f sek elapsed.", m_name.c_str(), msTime/1000000);
			else if(msTime < 1000000L*60*60)
				LOG_FORMAT_INFO("Stopwatch %s: %.3f min elapsed.", m_name.c_str(), msTime/60000000);
		}
	}

	void start(){
		m_endTime = -1;
		m_startTime = getCurrentTime();
		m_isRunning = true;
	}
	void stop(){
		if(m_isRunning)
			m_endTime = getCurrentTime();
		m_isRunning = false;
	}

	static long int getCurrentTime(){
		struct timeval tp;
		gettimeofday(&tp, NULL);
		return tp.tv_sec * 1000000 + tp.tv_usec;
	}

	/**
	 * Returns time in microseconds
	 */
	long int getTimeDelta(){
		if(m_isRunning)
			return (getCurrentTime() - m_startTime);
		else if(m_startTime != -1 && m_endTime != -1)
			return (m_endTime - m_startTime);
		else
			return 0;
	}

private:
	bool m_logOnDestroy;
	bool m_isRunning;
	long int m_startTime;
	long int m_endTime;
	std::string m_name;
};

} /* namespace fcv */

#endif /* FREECV_INCLUDE_FREECV_CORE_STOPWATCH_H_ */
