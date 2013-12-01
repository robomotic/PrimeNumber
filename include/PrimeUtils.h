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

#ifndef PRIMEUTILS_H
#define PRIMEUTILS_H

#include <vector>
#include <cmath>
#include <bitset>
#include <assert.h>
#include <iostream>
// OpenMP
#include <omp.h>
using namespace std;


//! Abstract class for prime number generation.
/*!
This abstract class is used as base class for the generation of prime numbers
Remember you cannot instantiate any object from this class
*/
class PrimeGenerator
{

public:
    //! A virtual function to generate a series of prime numbers
    /*!
    \param max_number the maximum integer number to generate the list
    */
    virtual int generate_primes(const int max_number)=0;
    //! A virtual function to check if a number is prime
    /*!
    \param number is the given number prime or not?
    \return A TRUE if is prime and a FALSE if not prime
    */
    virtual bool is_prime(int number)=0;
    virtual int getMaxNumber()=0;
    virtual ~PrimeGenerator();

};

//! An optimized sieve generator
/*!
I sacrificed the boost::dynamic_bitset<>  for a char array which has faster access in read/write!
*/
class PrimeGeneratorFast: public PrimeGenerator
{
protected:
    //! the list of prime numbers, this should be a bitset but in this way is faster
    /*! isPrime[i]=0 if i is not prime and viceversa */
    char* isPrime;
    //! the total memory size necessary for the bitset
    int memorySize;
public:
    //! This function generates primes starting from 2
    /*!
    \param max_number the maximum integer number to generate the list
    */
    int generate_primes(const int max_number);
    //! Returns the total number of numbers calculated
    /*!
    \return total memory size
    */
    int getMaxNumber();

    virtual ~PrimeGeneratorFast();
protected:
    //! A more optimized generator
    /*!
    \param from interval
    \param to interval
    \return number of primes found in that range
    */
    int eratosthenesOddSingleBlock(const int from, const int to,vector<int> &pool);

public:
    bool is_prime(int number);

};

//! This class has multi thread support via OpenMP
/*!
The Threads do not share the memory containing prime numbers, but only counts the number of prime founds.
Has to be improved in the future.
*/
class PrimeGeneratorMultiThread: public PrimeGeneratorFast
{
private:
    //! this is the block size each thread is going to compute
    /*! on my machine this works best .... */
    int sliceSize = 512*1024;
    //! a flag to enable OpenMP multi thread
    bool useOpenMP=true;
    //! the pool of prime numbers
    vector<int> pool;
public:

    int generate_primes(const int max_number);

    //! gets the number of numbers processed by each thread
    /*!
    \return interval for each thread
    */
    int getMaxNumber();
    void print_out();

    void enableOpenMP(bool flag);

    virtual ~PrimeGeneratorMultiThread() {};
private:

    //! A more optimized generator
    /*!
    \param from interval
    \param to interval
    \return number of primes found in that range
    */
    int eratosthenesBlockwise(int lastint);
};

//! A container class that generates left-truncatable numbers
/*!
This class is composed with an object of PrimeNumbers, I preferred
dependency injection rather than inheritance. so you can swap
different types of prime generators
*/
class Truncatable
{
private:
    //! a vector containing the first 1<= n <= 2166 truncatables
    vector<int> truncatable_primes;
    //! an object that generates a list of prime numbers
    PrimeGenerator* prime_fast_gen;

    //! a counter to know how many truncatables were found
    int found_truncatables;
    //! the number of maximum truncatables we want to generate
    int max_truncatables;
public:
    //! The constructor
    /*!
    \param prime_fast_gen an object to generate prime numbers
    \param to interval
    \return number of primes found in that range
    */
    Truncatable(PrimeGenerator* prime_fast_gen,const int max_truncatables)
    {

        this->prime_fast_gen=prime_fast_gen;
        this->max_truncatables=max_truncatables;

    };
    //! Retrieve the nth truncatable
    /*!
    \param the first t. number must be 2
    \return number itself
    */
    int get_truncatable(int order);
    //! Scan prime numbers and check if they are also trunc.
    /*!
    */
    void debug_primes();

    void print_out();

    //! Scan prime numbers and check if they are also trunc.
    /*!
    \return the number of found trunc. numbers
    */
    int find_truncatables();

private:
    //! Check if a prime number is also trunc.
    /*!
    \return true or false
    */
    bool left_trunc(int n);

};


#endif // PRIMEUTILS_H
