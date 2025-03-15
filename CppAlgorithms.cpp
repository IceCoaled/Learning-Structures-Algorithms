#include <iostream>
#include <vector>
#include <string>
#include <Windows.h>
#include <random>
#include <chrono>
#include <thread>
using namespace std::chrono_literals;


class HighResTimer
{
	using Clock = typename std::conditional<std::chrono::high_resolution_clock::is_steady,
		std::chrono::high_resolution_clock, std::chrono::steady_clock>::type;
	using Microseconds = std::chrono::duration<double, std::micro>;

private:
	Clock::time_point start;
	Clock::time_point stop;

public:
	::HighResTimer() = default;

	void Start()
	{
		start = Clock::now();
		stop = start;
	}

	void Stop()
	{
		stop = Clock::now();
	}

	double GetElapsed() const
	{
		return  Microseconds( stop - start ).count();
	}
};



class SortingAlgorithms
{
private:
	
	/// <summary>
	/// Size of array
	/// </summary>
	int szArray = 0;

	/// <summary>
	/// Array to be sorted
	/// </summary>
	std::vector<int> array;

public:

	::SortingAlgorithms(){}
	~SortingAlgorithms() {}

	void TestAllAlgorithms()
	{

		auto algo = std::thread( [this]()
		{
			this->BubbleSort();
		} );

		algo.join();

		algo = std::thread( [this]()
		{
			this->SelectionSort();
		} );

		algo.join();

		algo = std::thread( [this]()
		{
			this->QuickSortInit();
		} );

		algo.join();
	}

private:

	/// <summary>
	/// Bubble Sort algorithm
	/// </summary>
	void BubbleSort()
	{
		/// Print Bubble Sort header, Initialize array. 
		/// Print original array
		PrintSortName( "Bubble Sort" );
		InitArray();
		PrintArray();

		/// Star timer
		auto hRT = HighResTimer();
		hRT.Start();

		// Sort array
		for ( int i = 0; i < szArray - 1; ++i )
		{
			for ( int j = 0; j < szArray - i - 1; ++j )
			{
				if ( array[ j ] > array[ j + 1 ] )
				{
					XorSwap( array[ j ], array[ j + 1 ] );
				}
			}

		}

		/// End timer
		hRT.Stop();
		double et = hRT.GetElapsed();

		/// Print time taken to sort
		std::cout << "Total Time Taken : " << et << " us.\n";
		std::cout << "Array Size Of : " << this->szArray << ".\n";
		std::cout << et / this->szArray << " us, per element.\n\n";

		/// Print sorted array, reset array
		PrintArray( false );
		ResetArray();
		std::cout << "\n\n";
	}

	/// <summary>
	/// Selection sort algorithm
	/// </summary>
	void SelectionSort()
	{
		/// Print Selection Sort header, Initialize array. 
		/// Print original array
		PrintSortName( "Selection Sort" );
		InitArray();
		PrintArray();

		/// Star timer
		auto hRT = HighResTimer();
		hRT.Start();

		/// Sort array
		int minValueIndex = 0;
		for ( int i = 0; i < szArray - 1; ++i )
		{
			/// Reset minValueIndex
			minValueIndex = i;

			/// Iterate through unsorted portion
			for ( int j = i + 1; j < szArray; ++j )
			{
				if ( array[ j ] <= array[ minValueIndex ] )
				{
					minValueIndex = j;
				}
			}
			if ( minValueIndex != i )
			{
				/// Make swap
				XorSwap( array[ i ], array[ minValueIndex ] );
			}
		}

		/// End timer
		hRT.Stop();
		double et = hRT.GetElapsed();

		/// Print time taken to sort
		std::cout << "Total Time Taken : " << et << " us.\n";
		std::cout << "Array Size Of : " << this->szArray << ".\n";
		std::cout << et / this->szArray << " us, per element.\n\n";

		/// Print sorted array, reset array
		PrintArray( false );
		ResetArray();
		std::cout << "\n\n";
	}


	///-------------Quick-Sort-Start-----------------///

	
	/// <summary>
	/// Actual sorting function inside quick sort
	/// </summary>
	/// <param name="pLow"></param>
	/// <param name="pHigh"></param>
	/// <returns></returns>
	int QuickPartition( int pLow, int pHigh )
	{
		int i = pLow - 1;

		for ( int j = pLow; j < pHigh; ++j )
		{
			if ( array[ j ] <= array[ pHigh ] )
			{
				i += 1;
				XorSwap( array[ j ], array[ i ] );
			}
		}

		XorSwap( array[ i + 1 ], array[ pHigh ] );

		return i + 1;
	}

	/// <summary>
	/// Main recursion function
	/// </summary>
	/// <param name="pLow"></param>
	/// <param name="pHigh"></param>
	void QuickSort( int pLow, int pHigh )
	{
		if ( pLow < pHigh )
		{
			int pIndex = QuickPartition( pLow, pHigh );
			QuickSort( pLow, pIndex - 1 );
			QuickSort( pIndex + 1, pHigh );
		}
	}

	/// <summary>
	/// Main function to call for quick sort
	/// </summary>
	/// <param name="pLow"></param>
	/// <param name="pHigh"></param>
	void QuickSortInit()
	{
		/// Print Quick Sort header, Initialize array. 
		/// Print original array
		PrintSortName( "Quick Sort" );
		InitArray();
		PrintArray();

		/// Setup piviot high low
		const int pHigh = szArray - 1;
		const int pLow = 0;
		

		/// Star timer
		auto hRT = HighResTimer();
		hRT.Start();

		/// Sort array
		QuickSort( pLow, pHigh );

		/// End timer
		hRT.Stop();
		double et = hRT.GetElapsed();

		/// Print time taken to sort
		std::cout << "Total Time Taken : " << et << " us.\n";
		std::cout << "Array Size Of : " << this->szArray << ".\n";
		std::cout << et / this->szArray << " us, per element.\n\n";

		/// Print sorted array, reset array
		PrintArray( false );
		ResetArray();
		std::cout << "\n\n";
	}
	

	///----------------Util-Functions-----------------///

	/// <summary>
	/// Initialize array with random size and elements
	/// </summary>
	void InitArray()
	{
		/// Create the generator
		std::random_device rd;
		std::mt19937 gen( rd() );

		/// Set the distribution for the generator
		std::uniform_int_distribution<int> sizeDist( 50, 200 );
		std::uniform_int_distribution<int> valueDist( 0, 100 );

		/// Generate the random size of array between 50 and 200
		szArray = sizeDist( gen );

		/// Clear the array if it might have previous elements
		array.clear();

		/// Generate random array elements between 0 and 100
		for ( int i = 0; i < szArray; i++ )
		{
			array.emplace_back( valueDist( gen ) );
		}
	}

	/// <summary>
	/// Resets array details
	/// </summary>
	void ResetArray()
	{
		array.clear();
		szArray = 0;
	}

	/// <summary>
	/// Xor Swap algorithm
	/// This is used as to not use a temporary variable
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	void XorSwap( int& a, int& b ) const
	{
		/// Short Circuit
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
		const int elementsPerLine = 16;  // Number of elements to print per line

		std::cout << "================================================================" << std::endl;
		if ( original )
		{
			std::cout << "Original Array = ";
		} else
		{
			std::cout << "Sorted Array = ";
		}

		std::cout << "{ " << std::endl;  // Opening bracket for traditional array format

		for ( int i = 0; i < szArray; i++ )
		{
			// Print the current element
			std::cout << array[ i ];

			// Decide what separator to use
			if ( i == szArray - 1 )
			{
				// Last element gets no comma
				std::cout << "";
			} else
			{
				std::cout << ", ";

				// Add a line break after every elementsPerLine elements (except at the end)
				if ( ( i + 1 ) % elementsPerLine == 0 && i < szArray - 1 )
				{
					std::cout << std::endl << " ";  // New line with a space for indentation
				}
			}
		}

		std::cout << " };" << std::endl;  // Closing bracket for traditional array format
		std::cout << "================================================================" << std::endl;
	}


	/// <summary>
	/// Prints the sort name
	/// </summary>
	/// <param name="sortName"></param>
	void PrintSortName( const std::string& sortName ) const
	{
		std::cout << "================================================================" << std::endl;
		std::cout << sortName << std::endl;
	}

};






int main()
{	
	auto* sAlgoS = new SortingAlgorithms();

	sAlgoS->TestAllAlgorithms();

	delete sAlgoS;

	system( "pause" );
    return 0;
}
