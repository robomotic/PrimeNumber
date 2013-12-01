# Author: Paolo Di Prodi
# email: paolo@robomotic.com
# Copyright 2013 Robomotic ltd

/*
This file is part of PrimeNumber.

PrimeNumber is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

PrimeNumber is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar. If not, see <http://www.gnu.org/licenses/>.

*/

// timing
#ifdef USE_WINDOWS_TIMER
#include <windows.h>
#else
#include <sys/time.h>
#endif

double seconds()
{
#ifdef USE_WINDOWS_TIMER
  LARGE_INTEGER frequency, now;
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter  (&now);
  return now.QuadPart / double(frequency.QuadPart);
#else
  timeval now;
  gettimeofday(&now, NULL);
  return now.tv_sec + now.tv_usec/1000000.0;
#endif
}

