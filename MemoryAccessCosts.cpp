#include "Utils.hpp"

using tVector = alignedVector<uint32_t>;

#ifdef _MSC_VER
using tTimer = 
	QpcTimer;
	// CyclesTimer;
#else
using tTimer = CyclesTimer;
#endif

constexpr uint32_t cbCacheLine = 32;
constexpr uint32_t nScalarsPerLine = cbCacheLine / sizeof( uint32_t );

constexpr size_t iterationsCount = 1000 * 1000;

template<uint32_t CacheLinesLog2>
struct Dims
{
	// Index of the start of a random cache line in the uint32_t vector
	static size_t randomLineIndex( Random &rng )
	{
		const uint32_t r = rng.next();
		constexpr uint32_t mask = ( 1 << CacheLinesLog2 ) - 1;
		return ( r & mask ) * (size_t)nScalarsPerLine;
	}

	static size_t scalarsCount()
	{
		return (size_t)nScalarsPerLine * ( 1 << (size_t)CacheLinesLog2 );
	}

	static double megabytes()
	{
		return scalarsCount() * sizeof( uint32_t ) / ( 1024.0 * 1024.0 );
	}
};

template<class tDims>
tVector createRandomArray()
{
	tVector vec;
	vec.resize( tDims::scalarsCount() );
	Random rng;
	for( uint32_t& v : vec )
		v = rng.next() & 0x7FFFFFF;
	return vec;
}

template<class tDims, uint32_t nIntegers>
inline uint32_t sumRandomsScalar( const tVector& vec )
{
	Random rng;
	const uint32_t* const pData = vec.data();
	uint32_t counter = 0;
	for( size_t i = 0; i < iterationsCount; i++ )
	{
		const uint32_t* const pLine = pData + tDims::randomLineIndex( rng );
		for( size_t j = 0; j < nIntegers; j++ )
			counter += pLine[ j ];
	}
	return counter;
}

template<class tFunc>
bool calcBestTime( const char* msg, tFunc func )
{
	constexpr int bestTimeIterations = 16;

	const uint32_t referenceValue = func();

	uint64_t minTime = std::numeric_limits<uint64_t>::max();
	for( int i = 0; i < bestTimeIterations; i++ )
	{
		const tTimer timer;
		const uint32_t currentValue = func();
		const uint64_t elapsed = timer.elapsed();
		if( currentValue == referenceValue )
		{
			minTime = std::min( minTime, elapsed );
			continue;
		}
		printf( "Error in the algorithm\n" );
		return false;
	}
	printf( "%s: %g seconds/test, %i cycles/iteration\n", msg, tTimer::seconds( minTime ), tTimer::cyclesPerIteration( minTime, iterationsCount ) );
	return true;
}

template<uint32_t CacheLinesLog2>
void demo()
{
	using tDims = Dims<CacheLinesLog2>;
	printf( "Testing on %g MB vector\n", tDims::megabytes() );

	const alignedVector<uint32_t> vec = createRandomArray<tDims>();

	calcBestTime( "16", [ &vec ]() { return sumRandomsScalar<tDims, 16>( vec ); } );
	calcBestTime( "8", [ &vec ]() { return sumRandomsScalar<tDims, 8>( vec ); } );
	calcBestTime( "4", [ &vec ]() { return sumRandomsScalar<tDims, 4>( vec ); } );
	calcBestTime( "1", [ &vec ]() { return sumRandomsScalar<tDims, 1>( vec ); } );

	printf( "\n" );
}

int main()
{
	demo<28>();
	demo<27>();
	demo<25>();
	demo<20>();
	return 0;
}