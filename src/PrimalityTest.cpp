#include "PrimalityTest.h"
#include "PrimeUtils.h"

SieveGeneratorsMultiThread::SieveGeneratorsMultiThread(std::size_t N)
{
    M = N / 2 ;
    sieve=new boost::dynamic_bitset<>(M);
    sieve->flip();
}

SieveGeneratorsMultiThread::~SieveGeneratorsMultiThread()
{
    delete sieve;
}

void SieveGeneratorsMultiThread::sieve_of_sundaram(std::size_t low,std::size_t upper)
{
    for( std::size_t i = low ; i < upper ; ++i )
    {
        const std::size_t L = (upper-i) / ( 2*i + 1 ) ;
        for(std::size_t j = i ; j <= L ; ++j )
        {
            sieve->set( i + j + 2*i*j , false);

        }
    }
    boost::lock_guard<boost::mutex> lock(found_mutex);
    found++;
}
int SieveGeneratorsMultiThread::run()
{
    cout<<"Memory size "<<M<<" Block size "<<sliceSize<<endl;
    for (std::size_t from = 1; from <= M; from += sliceSize)
    {
        std::size_t to = from + sliceSize;
        if (to > M) to = M;
        sieve_of_sundaram(from,to);
        //prime_group.create_thread(
        //            boost::bind(&SieveGeneratorsMultiThread::sieve_of_sundaram,this,from,to));

    }
    //prime_group.join_all();
    return found;
}


boost::dynamic_bitset<> SieveGenerators::sieve_of_sundaram( std::size_t N )
{

   const std::size_t M = N / 2 ;
    boost::dynamic_bitset<> sieve(M) ;
    sieve.flip() ;
#pragma omp parallel for
    for( std::size_t i = 1 ; i < M ; ++i )
    {
        const std::size_t L = (M-i) / ( 2*i + 1 ) ;
        for( std::size_t j = i ; j <= L ; ++j ) sieve[ i + j + 2*i*j ] = false ;
    }

    return sieve ;
}


std::vector<int> SieveGenerators::prime_number_sequence( std::size_t lbound, std::size_t ubound )
{
    auto sieve = sieve_of_sundaram(ubound) ;
    std::vector<int> primes ;
    if( lbound < 3 )
    {
        primes.push_back(2) ;
        lbound = 3 ;
    }

    for( std::size_t i = (lbound-1)/2 ; i < sieve.size() ; ++i )
        if( sieve[i] ) primes.push_back( i*2 + 1 ) ;

    return primes ;
}

int SieveGenerators::nth_prime_number( std::size_t n )
{
    int number = 2 ;
    if( n > 1 )
    {
        // the nth prime number is approximately equal to n * log(n)
        double ubound = std::max( 100.0, n * std::log(n) * 1.5 ) ;
        assert( ubound < std::numeric_limits<std::size_t>::max() ) ;
        auto sieve = sieve_of_sundaram(ubound) ;
        std::size_t pos = 1 ;
        for( std::size_t i = pos ; n > 1 ; ++i ) if( sieve[i] )
            {
                pos = i ;
                --n ;
            }
        assert( pos < std::numeric_limits<int>::max() / 2 ) ;
        number = pos * 2 + 1 ;
    }

    return number ;
}


long int PrimalityTest::power(int a, int n, int mod)
{
    long int power=a,result=1;

    while(n)
    {
        if(n&1)
            result=(result*power)%mod;
        power=(power*power)%mod;
        n>>=1;
    }
    return result;
}

bool PrimalityTest::witness(int a, int n)
{
    int t,u,i;
    long int prev,curr;

    u=n/2;
    t=1;
    while(!(u&1))
    {
        u/=2;
        ++t;
    }

    prev=power(a,u,n);
    for(i=1; i<=t; ++i)
    {
        curr=(prev*prev)%n;
        if((curr==1)&&(prev!=1)&&(prev!=n-1))
            return true;
        prev=curr;
    }
    if(curr!=1)
        return true;
    return false;
}

bool PrimalityTest::IsPrime( int number )
{
    if ( ( (!(number & 1)) && number != 2 ) || (number < 2) || (number % 3 == 0 && number != 3) )
        return (false);

    if(number<1373653)
    {
        for( int k = 1; 36*k*k-12*k < number; ++k)
            if ( (number % (6*k+1) == 0) || (number % (6*k-1) == 0) )
                return (false);

        return true;
    }

    if(number < 9080191)
    {
        if(witness(31,number)) return false;
        if(witness(73,number)) return false;
        return true;
    }


    if(witness(2,number)) return false;
    if(witness(7,number)) return false;
    if(witness(61,number)) return false;
    return true;

    /*WARNING: Algorithm deterministic only for numbers < 4,759,123,141 (unsigned int's max is 4294967296)
      if n < 1,373,653, it is enough to test a = 2 and 3.
      if n < 9,080,191, it is enough to test a = 31 and 73.
      if n < 4,759,123,141, it is enough to test a = 2, 7, and 61.
      if n < 2,152,302,898,747, it is enough to test a = 2, 3, 5, 7, and 11.
      if n < 3,474,749,660,383, it is enough to test a = 2, 3, 5, 7, 11, and 13.
      if n < 341,550,071,728,321, it is enough to test a = 2, 3, 5, 7, 11, 13, and 17.*/
}


int TruncatableGenerator::generate_truncatables(std::size_t lbound, std::size_t ubound)
{

    auto sieve = SieveGenerators::sieve_of_sundaram(ubound) ;

    std::vector<int> primes ;
    if( lbound < 10 )
    {
        truncables.insert(2);
        truncables.insert(3);
        truncables.insert(5);
        truncables.insert(7);
        lbound = 10 ;
    }
    int candidate;
#pragma omp parallel for
    for( std::size_t i = (lbound-1)/2 ; i < sieve.size() ; ++i )
        if( sieve[i] )
        {
            candidate=i*2 + 1;
            if(left_trunc(candidate))
                truncables.insert( candidate ) ;
        }

    return truncables.size();

}

int TruncatableGenerator::get_truncatable(int order)
{
    std::set<int>::iterator it;
    it=truncables.begin();

    for (int i=0; i<order-1; i++) it++;

    return *it;
}
bool TruncatableGenerator::left_trunc(const int candidate)
{
    int decimals = 1;
    while (decimals < candidate) decimals*= 10;
    int chunk=candidate;
    while (chunk)
    {
        decimals /= 10;
        if (chunk < decimals) return false;
        //divide to get the right most decimals
        chunk %= decimals;
        //check for zeros and quit if necessary
        if(chunk<= decimals/10) return false;
        // check recursively if the remaning number is truncatable or not
        if ( truncables.find(chunk)!=truncables.end() )
            return true;
        else return false;
    }
    return true;
}

void TruncatableGenerator::print_out()
{
    std::set<int>::iterator it;
    for (it=truncables.begin(); it!=truncables.end(); ++it)
        std::cout << endl << *it;
    std::cout << '\n';
}

