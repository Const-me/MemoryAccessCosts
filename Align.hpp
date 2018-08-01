#pragma once
#include <vector>

// Mostly taken from https://github.com/Const-me/CollectionMicrobench/tree/master/Source/PlexAlloc

// Allocate aligned block of memory
inline void* alignedMalloc( size_t size, size_t alignment )
{
#ifdef _MSC_VER
	return _aligned_malloc( size, alignment );
#else
	return aligned_alloc( alignment, size );
#endif
}

// Free aligned block of memory
inline void alignedFree( void* ptr )
{
#ifdef _MSC_VER
	_aligned_free( ptr );
#else
	free( ptr );
#endif
}

template <typename T, size_t align = alignof ( T )>
class Allocator
{
public:
	using value_type = T;
	using size_type = std::size_t;
	using propagate_on_container_move_assignment = std::true_type;

	template<class U>
	struct rebind { using other = Allocator<U, align>; };

	Allocator() noexcept {};

	Allocator( const Allocator& ) noexcept {};

	template <typename U, size_t a>
	Allocator( const Allocator<U,  a>& ) noexcept {};

	Allocator( Allocator&& other ) noexcept { }

	Allocator& operator = ( const Allocator& ) noexcept
	{
		return *this;
	}

	Allocator& operator = ( Allocator&& other ) noexcept
	{
		return *this;
	}

	~Allocator() noexcept = default;

	T* allocate( size_type n )
	{
		void* const pArray = alignedMalloc( n * sizeof( T ), align );
		if( nullptr == pArray )
			throw std::bad_alloc();
		return static_cast<T*>( pArray );
	}

	void deallocate( T* ptr, size_type n ) noexcept
	{
		alignedFree( ptr );
	}
};

template<class T>
using alignedVector = std::vector<T, ::Allocator<T, 32>>;