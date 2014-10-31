#pragma once

/////////////////////////////////////////////////////////////////////////////
//
// File: elapsed_timer.hpp
//
// Created by MOON, Eui-kwon.
// Created on Jul-21st, 2009.
//
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include <windows.h>
#include <string>



/////////////////////////////////////////////////////////////////////////////
//
// Class: elapsed_timer
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class elapsed_timer
{
public:
	virtual ~elapsed_timer();

public:
	virtual void start      (void);
	virtual void stop       (void);

	virtual signed long int get_elapsed_mllisecond        (void);
	virtual double          get_elapsed_mllisecond_double (void);
};



/////////////////////////////////////////////////////////////////////////////
//
// Class: windows_elapsed_timer
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class windows_elapsed_timer : public elapsed_timer
{
public:
	windows_elapsed_timer();

public:
	virtual void start      (void);
	virtual void stop       (void);

	virtual signed long int get_elapsed_mllisecond        (void);
	virtual double          get_elapsed_mllisecond_double (void);

public:
	void initialize (void);

public:
	LARGE_INTEGER _frequency;
	LARGE_INTEGER _start    ;
	LARGE_INTEGER _stop     ;
};



/////////////////////////////////////////////////////////////////////////////
//
// Class: elapsed_time_printer
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class elapsed_time_printer
{
private:
	elapsed_timer* _timer;
	std::string    _message;

public:
	elapsed_time_printer (elapsed_timer* t, const char* m);
	~elapsed_time_printer ();
};
