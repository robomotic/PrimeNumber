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


#include <iostream>
#include <stdio.h>

#include "PrimalityTest.h"


using namespace std;

#include "TimeUtils.hpp"
//! a macro to define unit testing procedures uncomment to do uni testing
//#define TEST false


// number of primes between 0 and x:
//             10 =>           5
//            100 =>          26
//          1,000 =>         169
//         10,000 =>       1,230
//        100,000 =>       9,593
//      1,000,000 =>      78,499
//     10,000,000 =>     664,580
//    100,000,000 =>   5,761,456
//  1,000,000,000 =>  50,847,535
// 10,000,000,000 => 455,052,512
const int max_number = 1*1000*1000*1000LL;
const int max_truncatables=2166;




#ifdef TEST
#include "TimeUtils.hpp"

#include "PrimeUtils.h"
#define TEST_MIN_NUMBER 0
#define TEST_MAX_NUMBER 100000
#define KNOWN_PRIMES 9592


//! test serial prime number generators and truncatables
void unit_test_serial()
{
    //test the prime number generator first
    int found=0;
    cout<<"Sundaram Sieve Generator"<<endl;
    cout<<"Primes between "<<TEST_MIN_NUMBER<<" and "<<TEST_MAX_NUMBER<<endl;

    double startTime = seconds();

    for( int n : SieveGenerators::prime_number_sequence( TEST_MIN_NUMBER, TEST_MAX_NUMBER) ){
        //std::cout << n << " , ";
        found++ ;
    }

    //for( std::size_t n = 100 ; n < 1000000 ; n *= 10 )
    //std::cout << n+1 << ": " << nth_prime_number(n+1) << '\n' ;

    double duration  = seconds() - startTime;
    printf("\n %d primes found in %.3fs \n\n", found, duration);

    assert(found==KNOWN_PRIMES);
    cout<<"Sundaram test generator is correct."<<endl;
    return;

}

//! test serial prime number generators and truncatables
void unit_test_parallel()
{

    int found=0;
    cout<<"Sundaram Sieve Generator"<<endl;
    cout<<"Primes between "<<TEST_MIN_NUMBER<<" and "<<TEST_MAX_NUMBER<<endl;
    int cores=boost::thread::hardware_concurrency();
    cout<<"Number of hw cores available "<<cores<<endl;


    double startTime = seconds();
    PrimeGeneratorMultiThread multi_prime;
    found=multi_prime.generate_primes(TEST_MAX_NUMBER);
    double duration  = seconds() - startTime;

    printf("%d primes found in %.3fs\n\n", found, duration);

    //multi_prime.print_out();
    assert(found==KNOWN_PRIMES);
    cout<<"Parallel sieve generator is correct."<<endl;
    return;

}

#endif

int main()
{

#ifdef TEST
    cout<<"Serial"<<endl;
    unit_test_serial();
    cout<<"Parallel"<<endl;
    unit_test_parallel();

#else

    TruncatableGenerator *generator=new TruncatableGenerator();
double startTime = seconds();
    int max_index=generator->generate_truncatables(1,max_number);
    //generator->print_out();
double duration  = seconds() - startTime;

    printf("%d primes found in %.3fs\n\n", max_index, duration);
    cout<<"Input truncatable index prime as an integer (min=1, max ="<<max_index<<")"<<endl;
    cout<<"To quit this program type 0 "<<endl;

    //read an input from the user
    int nth_tprime = 1;
    while(nth_tprime != 0 )
    {
        cout<<">>  ";

        if ( !(cin >> nth_tprime) )
        {
            cin.clear(); //clear the failure flag if there is an error when reading!
            std::string garbage;
            std::getline(cin, garbage); //read the garbage from the stream and throw it away
        }
        //retrieve the truncatable
        if(nth_tprime>=1 && nth_tprime<=max_index)
            cout<<generator->get_truncatable(nth_tprime)<<endl;
    }

    delete generator;
#endif
    return 0;
}
