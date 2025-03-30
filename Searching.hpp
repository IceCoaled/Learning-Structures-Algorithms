#include "ClassBase.hpp"
#include <array>



/**
* @brief Class implementing various searching algorithms
*
* Contains implementations of common searching algorithms including
* Linear Search, Binary Search, and Sliding Window Search.
* The class handles initialization of search data, execution of
* searches, and performance measurement.
*
* @tparam T Numeric type that meets the NumericConstraint requirement
* 
* @details For the sliding window search, we get a random number for an
* index into the array, We then use this and get a random 
* number for a sub array length and get the sum of the sub array.
*
* The min sub array length is minimum of 11. This is to 
* eliminate edge cases for the purpose of just learning the algorithm itself
*
* We are also just returning array indexes for the result; as to use for 
* checking if we got the correct result
*/
template <typename T>
	requires NumericConstraint<T>
class SearchAlogrithms: public AlgorithmsBase<T>
{
private:
	std::array<T, 2> sValues; //< search values for linear and binary
	T sSumValue; //< sum of sub array
	std::size_t sSumStart; //< starting index of sub array
	std::size_t sSumLen; //< length of sub array

public:

	/**
	* @brief Constructor that initializes search data
	*/
	SearchAlogrithms()
	{
		InitData();
	}

	/**
	* @brief Default destructor
	*/
	~SearchAlogrithms() = default;

	/**
	* @brief Executes all search algorithms and validates their results
	*/
	void TestAllSearchAlgorithms()
	{
		auto lSResult = LinearSearch();
		if ( lSResult.has_value() && this->array[ lSResult.value() ] == sValues[ 1 ] )
		{
			PrintResults( lSResult.value() );
		}
		auto sWResult = SlidingWindow();
		if ( sWResult.has_value() && std::get< 0 >( sWResult.value() ) == sSumStart && std::get< 1 >( sWResult.value() ) == sSumLen )
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

	/**
	* @brief Linear search algorithm
	*
	* @return std::nullopt if not found, else index of value to search for
	*/
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

	/**
	* @brief Binary search algorithm
	*
	* @return std::nullopt if not found, else index of value to search for
	*/
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
			} else if ( sValues[ 0 ] > this->array[ mid ] )
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


	/**
	* @brief Sliding window search algorithm
	*
	* In this context we are using it to find the sub array that contains
	* the sum of values that equals sSumValue
	*
	* @return std::nullopt if not found, or a tuple where the first value is the
	*         0th index of the subarray and the second is the length of the subarray
	*/
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
			return std::make_tuple( pLower, pUpper - pLower + 1 );
		} else
		{
			std::cout << "Failed To Find Sub Array In Data\n";
			return std::nullopt;
		}
	}

	///---------------Merge-Sort-Start---------------///

	/**
	* @brief Initializes and executes the Merge Sort algorithm
	*/
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

	/**
	* @brief Recursive Merge Sort implementation
	*
	* @param start Starting index of the array segment to sort
	* @param end Ending index of the array segment to sort
	*/
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

	/**
	* @brief Merges two sorted subarrays into a single sorted array
	*
	* @param start Starting index of first subarray
	* @param mid Ending index of first subarray
	* @param end Ending index of second subarray
	*/
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

	/**
	* @brief Prints performance results for search algorithms
	*
	* @param index The index where the value was found
	*/
	void PrintResults( const std::size_t& index )
	{
		std::println( "<Performance>" );
		std::println( "Total Time Taken : {} us.", this->timer.GetElapsed() );
		std::println( "Index of Value: {}", index );
	}

	/**
	* @brief Prints performance results for sliding window search
	*
	* @param slidingWindowResult A tuple containing the start index and length of found subarray
	*/
	void SWPrintResults( const std::tuple<std::size_t, std::size_t>& slidingWindowResult )
	{
		std::println( "<Performance>" );
		std::println( "Total Time Taken : {} us.", this->timer.GetElapsed() );
		std::println( "Index of sub array: {}", std::get<0>( slidingWindowResult ) );
		std::println( "Length of sub array: {}", std::get<1>( slidingWindowResult ) );
	}


	/**
	* @brief Initializes and sorts the array
	*
	* Then gets the search values for Linear/Binary search
	* We then get/set our sub array details for sliding window search
	*/
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
		for ( std::size_t i = sSumStart; i < sSumLen + sSumStart; ++i )
		{
			sSumValue += this->array[ i ];
		}
	}
};
