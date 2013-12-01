#include "PrimeUtils.h"
#include <assert.h>


PrimeGenerator::~PrimeGenerator()
{
    //dtor
}


int PrimeGeneratorFast::generate_primes(const int max_number)
{

    int j;
    int i = 2;
    memorySize=max_number+1;
    int k= 0;
    isPrime = new char[memorySize];
    for (int i = 0; i < memorySize; i++)
        isPrime[i] = 1;
    //the first two numbers are not prime
    isPrime[0]=0;
    isPrime[1]=0;

    while (i * i < max_number)
    {
        for (j = i * 2; j < max_number; j += i)
            isPrime[j]=0;

        while (++i < max_number && !isPrime[i]);
    }
    for (int i = 0; i < memorySize; i++)
        if(isPrime[i] )k++;
    return k;
}

int PrimeGeneratorFast::getMaxNumber()
{
    return this->memorySize-1;
}

int PrimeGeneratorFast::eratosthenesOddSingleBlock(const int from, const int to,vector<int> &pool)
{
    const int memorySize = (to - from + 1) / 2;
      // initialize
    char* isPrime = new char[memorySize];
    for (int i = 0; i < memorySize; i++)
        isPrime[i] = 1;

    for (int i = 3; i*i <= to; i += 2)
    {
        // skip multiples of three: 9, 15, 21, 27, ...
        if (i >= 3*3 && i % 3 == 0)
            continue;
        // skip multiples of five
        if (i >= 5*5 && i % 5 == 0)
            continue;
        // skip multiples of seven
        if (i >= 7*7 && i % 7 == 0)
            continue;
        // skip multiples of eleven
        if (i >= 11*11 && i % 11 == 0)
            continue;
        // skip multiples of thirteen
        if (i >= 13*13 && i % 13 == 0)
            continue;

        // skip ints before current slice
        int minJ = ((from+i-1)/i)*i;
        if (minJ < i*i)
            minJ = i*i;

        // start value must be odd
        if ((minJ & 1) == 0)
            minJ += i;

        // find all odd non-primes
        for (int j = minJ; j <= to; j += 2*i)
        {
            int index = j - from;
            isPrime[index/2] = 0;
        }
    }

    // count primes in this block
    int found = 0;
    // 2 is not odd => include on demand
    if (from <= 2)
    {
        found++;
        pool.push_back(2);
    }

    for (int i = 0; i < memorySize; i++)
    {
        found += isPrime[i];

        if(isPrime[i])pool.push_back(from+1+2*i);
    }


    return found;
}

bool PrimeGeneratorFast::is_prime(int number)
{
    //make sure we are not out of bounds
    assert(number>=0 && number< memorySize );
    return this->isPrime[number];
}


PrimeGeneratorFast::~PrimeGeneratorFast()
{
    //dtor
}

int PrimeGeneratorMultiThread::generate_primes(const int max_number)
{

    return this->eratosthenesBlockwise(max_number);
}

int PrimeGeneratorMultiThread::getMaxNumber()
{
    return sliceSize;
}

void PrimeGeneratorMultiThread::enableOpenMP(bool flag)
{

    this->useOpenMP=flag;
}

int PrimeGeneratorMultiThread::eratosthenesBlockwise(int lastint)
{
    // enable/disable OpenMP
    omp_set_num_threads(this->useOpenMP ? omp_get_num_procs() : 1);

    int found = 0;

    // each slices covers ["from" ... "to"], incl. "from" and "to"
    #pragma omp parallel for shared(pool) reduction(+:found)
    for (int from = 2; from <= lastint; from += sliceSize)
    {
        int to = from + sliceSize;
        if (to > lastint)
            to = lastint;

        found += eratosthenesOddSingleBlock(from, to,pool);
    }

    return found;
}
void PrimeGeneratorMultiThread::print_out()
{
    vector<int>::const_iterator cii;
    cout << endl << "Prime numbers:" << endl;
    for(cii=pool.begin(); cii!=pool.end(); cii++)
    {
        cout << *cii <<" , ";
    }
}


bool Truncatable::left_trunc(int n)
{
    int tens = 1;
    while (tens < n) tens *= 10;

    while (n)
    {
        if (!prime_fast_gen->is_prime(n)) return 0;
        tens /= 10;
        if (n < tens) return 0;
        n %= tens;
    }
    return 1;
}
int Truncatable::find_truncatables()
{
    assert (this->prime_fast_gen!=NULL);
    int number=0;
    while(found_truncatables<=max_truncatables)
    {
        if(number>prime_fast_gen->getMaxNumber()) break;
        // if it's a prime number and also a left truncatable
        if(prime_fast_gen->is_prime(number) && left_trunc(number))
        {

            //add it into the list of elements found so far
            truncatable_primes.push_back(number);
            found_truncatables++;
        }
        number++;
    }

    return found_truncatables;
}

void Truncatable::debug_primes()
{
    assert (this->prime_fast_gen!=NULL);
    int number=0;
    while(number <= prime_fast_gen->getMaxNumber())
    {
        cout<<"Number "<<number<<" is prime: "<<prime_fast_gen->is_prime(number)<<endl;
        cout<<"Number "<<number<<" is trunc: "<<left_trunc(number)<<endl;
        number++;
    }
}

void Truncatable::print_out()
{
    vector<int>::const_iterator cii;
    cout << endl << "Trunc. numbers:" << endl;
    for(cii=truncatable_primes.begin(); cii!=truncatable_primes.end(); cii++)
    {
        cout << *cii << endl;
    }
}

int Truncatable::get_truncatable(int order)
{
    assert(order>=1 && order<= found_truncatables);
    return truncatable_primes[order-1];
}
