/////////////////////////////////////////////////////////////////////////////
//
// File: elapsed_timer.cpp
//
// Created by MOON, Eui-kwon.
// Created on Jul-21st, 2009.
//
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "elapsed_timer.hpp"



/////////////////////////////////////////////////////////////////////////////
//
// Class: elapsed_timer
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
elapsed_timer::~elapsed_timer()
{
}

//===========================================================================
void elapsed_timer::start (void)
{
}

void elapsed_timer::stop (void)
{
}

signed long int elapsed_timer::get_elapsed_mllisecond (void)
{
	return 0;
}

double elapsed_timer::get_elapsed_mllisecond_double (void)
{
	return 0.0;
}



/////////////////////////////////////////////////////////////////////////////
//
// Class: windows_elapsed_timer
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
windows_elapsed_timer::windows_elapsed_timer()
{
	_frequency.QuadPart=0;
	_start    .QuadPart=0;
	_stop     .QuadPart=0;

	initialize ();
}

void windows_elapsed_timer::initialize(void )
{
	QueryPerformanceFrequency(&_frequency); 
}

void windows_elapsed_timer::start(void )
{
	QueryPerformanceCounter(&_start);
}

void windows_elapsed_timer::stop(void)
{
	QueryPerformanceCounter(&_stop);
}

signed long int windows_elapsed_timer::get_elapsed_mllisecond(void)
{
	signed long long int differance;
	signed long long int frequency;

	frequency  = _frequency.QuadPart;
	differance = _stop.QuadPart - _start.QuadPart;

	return static_cast<signed long int>( differance * 1000 / frequency );
}

double windows_elapsed_timer::get_elapsed_mllisecond_double (void)
{
	signed long long int differance;
	signed long long int frequency;

	frequency  = _frequency.QuadPart;
	differance = _stop.QuadPart - _start.QuadPart;

	return static_cast<double>( differance * 1000.0 / ((double)frequency) );
}



/////////////////////////////////////////////////////////////////////////////
//
// Class: elapsed_time_printer
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
elapsed_time_printer::elapsed_time_printer (elapsed_timer* t, const char* m):
	_timer(t),
	_message(m)
{
	_timer->start();
}

elapsed_time_printer::~elapsed_time_printer ()
{
	_timer->stop();

	printf ("elapsed_time = %lf ms : ",_timer->get_elapsed_mllisecond_double());
	printf ("%s \r\n",  _message.c_str());
}

