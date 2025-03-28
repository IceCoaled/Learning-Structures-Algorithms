#include "ClassBase.hpp"
#include <array>


/*
* For the sliding window search,
* We get a random number for an
* Index into the array, We then
* Use use this and get a random
* Number for a sub array length
* And get the sum of the sub array
* 
* The min sub array length is 11
* This is to eliminate edge cases
* For the purpose of just learning 
* The algorithm itself
* 
* We are also just returning array indexes
* For the result as to use for double
* Checking we got the correct result
*/

template <typename T>
	requires NumericConstraint<T>
class SearchAlogrithms: public AlgorithmsBase<T>
{
private:
	std::array<T, 2> sValues;
	T sSumValue;
	std::size_t sSumStart;
	std::size_t sSumLen;

public:

	SearchAlogrithms()
	{
		InitData();
	}
	~SearchAlogrithms() = default;

	void TestAllSearchAlgorithms()
	{
		auto lSResult = LinearSearch();
		if ( lSResult.has_value() && this->array[ lSResult.value() ] == sValues[ 1 ] )
		{
			PrintResults( lSResult.value() );
		}
		auto sWResult = SlidingWindow();
		if ( sWResult.has_value() && std::get< 0 >( sWResult.value() ) == sSumStart )
		{
			SWPrintResults( sWResult.value() );
		}
		auto bSResult = BinarySearch();
		if ( bSResult.has_value() && this->array[ bSResult.value() ] == sValues[ 0 ] )
		{
			PrintResults( bSResult.value() );
		}		
	}

private:


	/// <summary>
	/// Linear search algorithm
	/// </summary>
	/// <returns>std::nullopt_t if not found, else index of value to search for</returns>
	std::optional<std::size_t> LinearSearch()
	{
		// Print algorithm name
		this->PrintAlgoName( "Linear Search" );

		/// our result for search
		std::size_t result = SIZE_MAX;

		/// Start Timer
		this->timer.Start();
		
		for ( int i = 0; i < this->szArray; ++i )
		{
			if ( this->array[ i ] == sValues[ 1 ] )
			{
				result = i;
				break;
			}
		}

		// End Timer
		this->timer.Stop();

		if ( result != SIZE_MAX )
		{
			return result;
		} else
		{
			std::cout << "Failed To Find Value In Data\n";
			return std::nullopt;
		}
	}

	/// <summary>
	/// Binary search algorithm
	/// </summary>
	/// <returns>std::nullopt_t if not found, else index of value to search for</returns>
	std::optional<std::size_t> BinarySearch()
	{
		// Print algorithm name
		this->PrintAlgoName( "Binary Search" );

		/// our result for search
		std::size_t result = SIZE_MAX;

		/// Start Timer
		this->timer.Start();
		
		/// Our pointer window
		std::size_t pLower = 0;
		std::size_t pUpper = this->szArray - 1;

		/// Calculate the middle point 
		std::size_t mid = std::midpoint( pLower, pUpper );

		/// Loop through array searching for value
		while ( pLower < pUpper || pUpper - pLower == 1 )
		{
			if ( this->array[ mid ] == sValues[ 0 ] )
			{
				result = mid;
				break;
			} else if ( sValues[ 0 ] > this->array[ mid ]  )
			{
				pLower = mid + 1;
				mid = std::midpoint( pLower, pUpper );
			} else if ( sValues[ 0 ] < this->array[ mid ] )
			{
				pUpper = mid - 1;
				mid = std::midpoint( pLower, pUpper );
			}
		}

		// End Timer
		this->timer.Stop();

		if ( result != SIZE_MAX )
		{
			return result;
		} else
		{
			std::cout << "Failed To Find Value In Data\n";
			return std::nullopt;
		}		
	}


	/// <summary>
	/// Sliding window search algorithm, in this context
	/// We are using it to find the sub array that contains
	/// The sum of values that sSumValues is
	/// </summary>
	/// <returns>std::nullopt_t if not found,Tuple, first var in the tuple is the 0th index of the subarray. The second is the length of the subarray</returns>
	std::optional<std::tuple<std::size_t, std::size_t>> SlidingWindow()
	{
		// Print algorithm name
		this->PrintAlgoName( "Sliding Window Search" );
		
		/// Start Timer
		this->timer.Start();
		
		/// Our pointer window
		std::size_t pLower = 0;
		std::size_t pUpper = 0;

		/// The current sum of the window
		T cSum = this->array[ 0 ];

		/// Loop through array looking for subarray
		while ( cSum != sSumValue && pUpper < this->szArray )
		{
			if ( cSum > sSumValue || pUpper - pLower > sSumLen )
			{				
				cSum -= this->array[ pLower ];
				++pLower;
			} else
			{
				cSum += this->array[ ++pUpper ];
			}
		}

		// End Timer
		this->timer.Stop();

		if ( cSum == sSumValue )
		{
			return std::make_tuple( pLower, pUpper - pLower );
		} else
		{
			std::cout << "Failed To Find Sub Array In Data\n";
			return std::nullopt;
		}
	}

	///---------------Merge-Sort-Start---------------///

	void MergeSortInit()
	{
		this->InitArray();
		// Edge case check
		if ( this->szArray == 0 || this->szArray == 1 )
		{
			return;
		}
		/// Set tempBuffer size
		this->tempBuffer.resize( this->szArray );
		/// Call the traditional merge sort
		MergeSort( 0, this->szArray - 1 );
	}

	void MergeSort( T start, T end )
	{
		/// Base case: if the subarray has 0 or 1 element, it's already sorted
		if ( start >= end ) return;

		/// Calculate the middle point to divide the array into two halves
		const T mid = std::midpoint( start, end );

		/// Recursively sort the first half
		MergeSort( start, mid );

		/// Recursively sort the second half
		MergeSort( mid + 1, end );

		/// Merge the sorted halves
		Merge( start, mid, end );
	}

	void Merge( T start, T mid, T end )
	{
		/// Copy elements to the temp buffer
		for ( T i = start; i <= end; i++ )
		{
			this->tempBuffer[ i ] = this->array[ i ];
		}

		/// Initial index of first, second halves
		T i = start;
		T j = mid + 1;
		/// Initial index of merged array
		T k = start;

		/// Merge the two halves back into the original array
		while ( i <= mid && j <= end )
		{
			if ( this->tempBuffer[ i ] <= this->tempBuffer[ j ] )
			{
				this->array[ k++ ] = this->tempBuffer[ i++ ];
			} else
			{
				this->array[ k++ ] = this->tempBuffer[ j++ ];
			}
		}

		/// Copy remaining elements of the first half, if any
		while ( i <= mid )
		{
			this->array[ k++ ] = this->tempBuffer[ i++ ];
		}
	}


	///------------Class-Utils--------------///

	void PrintResults( const std::size_t& index )
	{
		std::cout << "<Performance>\n";
		std::cout << "Total Time Taken : " << this->timer.GetElapsed() << " us.\n";
		std::cout << "Index of Value: " << index << "\n";
	}

	void SWPrintResults( const std::tuple<std::size_t, std::size_t>& slidingWindowResult )
	{
		std::cout << "<Performance>\n";
		std::cout << "Total Time Taken : " << this->timer.GetElapsed() << " us.\n";
		std::cout << "Index of sub array: " << std::get<0>( slidingWindowResult ) << "\n";
		std::cout << "Length of sub array: " << std::get<1>( slidingWindowResult ) << "\n";
	}


	/// <summary>
	/// Initializes and sorts the array
	/// Then gets the search values for
	/// Linear/Binary search
	/// We then get/set our sub array 
	/// Details for sliding window search
	/// </summary>
	void InitData()
	{
		MergeSortInit();
		/// Generator and device
		std::random_device rd;
		std::mt19937 gen( rd() );

		/// Set the distribution for search values
		std::uniform_int_distribution<std::size_t> sizeDist( 0, this->szArray );
		/// Set the distribution for sliding window sub array
		std::uniform_int_distribution<std::size_t> sADist( 0, this->szArray - 21 );
		/// Set the distribution for the sub array length
		std::uniform_int_distribution<std::size_t> sizeSumDist( 11, 20 );


		// Set the sub array start index
		sSumStart = sADist( gen );
		// Set the binary search value
		sValues[ 0 ] = this->array[ sizeDist( gen ) ];
		// Set the linear search value
		sValues[ 1 ] = this->array[ sizeDist( gen ) ];

		// Get random sub array length
		sSumLen = sizeSumDist( gen );

		// Calculate sub array sum, yes there is functions for this
		// But this is simple for our learning and works
		for ( std::size_t i = sSumLen; i < sSumLen + arrayIndex; ++i )
		{
			sSumValue += this->array[ i ];
		}
	}
};
