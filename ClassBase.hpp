#ifndef CLASSBASE_HPP
#define CLASSBASE_HPP


#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <limits>
#include <numeric>
#include <type_traits>


template < typename T>
concept NumericConstraint = 
std::numeric_limits<T>::is_specialized && 
std::is_arithmetic_v<T>;



class HighResTimer
{
	using Timer = typename std::conditional<std::chrono::high_resolution_clock::is_steady,
		std::chrono::high_resolution_clock, std::chrono::steady_clock>;
	using Microseconds = std::chrono::duration<double, std::micro>;

private:
	Timer::type::time_point start;
	Timer::type::time_point stop;

public:
	HighResTimer() = default;
	~HighResTimer() = default;

	void Start()
	{
		start = Timer::type::now();
		stop = start;
	}

	void Stop()
	{
		stop = Timer::type::now();
	}

	double GetElapsed() const
	{
		return  Microseconds( stop - start ).count();
	}
};


												 
template <typename T>
	requires NumericConstraint<T>
class AlgorithmsBase
{
protected:

	/// <summary>
	/// Size of array
	/// </summary>
	T szArray = 0;

	/// <summary>
	/// array to be sorted
	/// </summary>
	std::vector<T> array;

	/// <summary>
	/// Temp buffer for sorting
	/// </summary>
	std::vector<T> tempBuffer;

	/// <summary>
	/// Sort timer
	/// </summary>
	HighResTimer timer = HighResTimer();


public:
	T GetSzArrayValue() const { return szArray; }
	std::vector<T>& ArrayRef() { return array; }
	std::vector<T>& TempBufferRef() { return tempBuffer; }
	HighResTimer& TimerRef() { return timer; }


	/// <summary>
	/// Initialize array with random size and elements
	/// </summary>
	void InitArray()
	{
		/// Create the generator
		std::random_device rd;
		std::mt19937 gen( rd() );

		/// Set the distribution for the generator
		std::uniform_int_distribution<T> sizeDist( 50, 200 );
		std::uniform_int_distribution<T> valueDist( 0, 100 );

		/// Clear array and set size to 0
		ResetArray();

		/// Generate the random size of array between 50 and 200
		szArray = sizeDist( gen );

		/// Generate random array elements between 0 and 100
		for ( T i = 0; i < szArray; i++ )
		{
			array.emplace_back( valueDist( gen ) );
		}
	}

	/// <summary>
	/// Resets array details
	/// </summary>
	void ResetArray()
	{
		tempBuffer.clear();
		array.clear();
		szArray = 0;
	}

	/// <summary>
	/// Xor Swap algorithm
	/// This is used as to not use a temporary variable
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	void XorSwap( T& a, T& b ) const
	{
		/// Short circuit
		if ( a == b ) return;

		/// Swap
		a ^= b;
		b ^= a;
		a ^= b;
	}


	/// <summary>
	/// Print array elements.
	/// If original is true, print original array,
	/// else print sorted array.
	/// Prints in traditional array format with 16 values per line.
	/// </summary>
	/// <param name="original"></param>
	void PrintArray( const bool& original = true ) const
	{
		const T elementsPerLine = 16;  /// Number of elements to print per line

		std::cout << "================================================================" << std::endl;
		if ( original )
		{
			std::cout << "Original array = ";
		} else
		{
			std::cout << "Sorted array = ";
		}

		std::cout << "{ " << std::endl;

		for ( T i = 0; i < szArray; i++ )
		{
			/// Print the current element
			std::cout << array[ i ];

			/// Decide what separator to use
			if ( i == szArray - 1 )
			{
				/// Last element gets no comma
				std::cout << "";
			} else
			{
				std::cout << ", ";

				/// Add a line break after every elementsPerLine elements (except at the end)
				if ( ( i + 1 ) % elementsPerLine == 0 && i < szArray - 1 )
				{
					std::cout << std::endl << " ";  // New line with a space for indentation
				}
			}
		}

		std::cout << " };" << std::endl;
		std::cout << "================================================================" << std::endl;
	}


	/// <summary>
	/// Prints the sort name
	/// </summary>
	/// <param name="sortName"></param>
	void PrintAlgoName( const std::string& sortName ) const
	{
		std::cout << "================================================================" << std::endl;
		std::cout << sortName << std::endl;
	}
};
#endif // !CLASSBASE_HXX

