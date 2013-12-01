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

#ifndef PRIMALITYTEST_H
#define PRIMALITYTEST_H
#include <set>
#include <iostream>
#include <vector>
#include <boost/dynamic_bitset.hpp>
#include <cmath>
#include <limits>
#include <cmath>

#include <boost/thread.hpp>
#include <boost/bind.hpp>

using namespace std;

//! static class that generates a map of prime numbers
/*!
This memory efficient class returns a dynamic bitmap
*/
class SieveGenerators
{
public:
    static boost::dynamic_bitset<> sieve_of_sundaram( std::size_t N );

    static std::vector<int> prime_number_sequence( std::size_t lbound, std::size_t ubound );

    static int nth_prime_number( std::size_t n );

};

//! static class that generates a map of prime numbers
/*!
This memory efficient class returns a dynamic bitmap
*/
class SieveGeneratorsMultiThread
{
private:
    boost::dynamic_bitset<> *sieve;
    boost::thread_group prime_group;
    std::size_t M;
    std::size_t found;
    boost::mutex found_mutex;
    const int sliceSize = 512*1024;
public:
    SieveGeneratorsMultiThread(std::size_t N);
    ~SieveGeneratorsMultiThread();
    void sieve_of_sundaram(std::size_t low,std::size_t upper);
    int run();

};

//! static class to check if a number is prime
/*!
Several wheel factorization optimization here
*/
class PrimalityTest
{
public:
    static bool IsPrime( int number );
protected:
    static long int power(int a, int n, int mod);
    static bool witness(int a, int n);
private:
};

//! Class that generates the truncatables from a set of prime numbers
/*!
Several wheel factorization optimization here
*/
class TruncatableGenerator
{
private:
    //! this is a set containing all the truncables number found so far
    set<int> truncables;
public:
    //! given a range of numbers find all the truncatables
    int generate_truncatables(std::size_t lbound, std::size_t ubound);
    //! print all the numbers
    void print_out();
    //! get the nth truncatable number
    int get_truncatable(int order);
    //! chek iteratively if a number is truncable or not
    bool left_trunc(const int candidate);

};

#endif // PRIMALITYTEST_H
