#include "ClassBase.hpp"


template <typename T>
class SortingAlgorithms: protected AlgorithmsBase<T>
{
private:
	using AlgorithmsBase<T>::GetSzArrayValue;
	using AlgorithmsBase<T>::ArrayRef;
	using AlgorithmsBase<T>::TimerRef;
	using AlgorithmsBase<T>::TempBufferRef;
	using AlgorithmsBase<T>::InitArray;
	using AlgorithmsBase<T>::PrintArray;
	using AlgorithmsBase<T>::PrintAlgoName;
	using AlgorithmsBase<T>::ResetArray;
	using AlgorithmsBase<T>::XorSwap;

public:
	SortingAlgorithms() = default;
	~SortingAlgorithms() = default;

	void TestAllAlgorithms()
	{
		BubbleSort();
		SelectionSort();
		InsertionSort();
		QuickSortInit();
		MergeSortInit();
	}

private:

	/// <summary>
	/// Bubble Sort algorithm
	/// </summary>
	void BubbleSort()
	{
		/// Print Bubble Sort header, Initialize array. 
		/// Print original array
		PrintAlgoName("Bubble Sort");
		InitArray();
		PrintArray();

		/// Get our array references
		T szArray = GetSzArrayValue();
		auto& array = ArrayRef();

		/// Start Timer
		TimerRef().Start();

		/// Sort array
		for ( T i = 0; i < szArray - 1; ++i )
		{
			for ( T j = 0; j < szArray - i - 1; ++j )
			{
				if ( array[ j ] > array[ j + 1 ] )
				{
					XorSwap( array[ j ], array[ j + 1 ] );
				}
			}

		}

		/// End Timer
		TimerRef().Stop();
		double et = TimerRef().GetElapsed();

		/// Print time taken to sort
		std::cout << "<Bubble Sort Performance>\n";
		std::cout << "Total Time Taken : " << et << " us.\n";
		std::cout << "array Size Of : " << szArray << ".\n";
		std::cout << et / szArray << " us, per element.\n\n";

		/// Print sorted array
		PrintArray( false );
		std::cout << "\n\n";
	}


	/// <summary>
	/// Selection sort algorithm
	/// </summary>
	void SelectionSort()
	{
		/// Print Selection Sort header, Initialize array. 
		/// Print original array
		PrintAlgoName( "Selection Sort" );
		InitArray();
		PrintArray();

		/// Get our array references
		T szArray = GetSzArrayValue();
		auto& array = ArrayRef();

		/// Start Timer
		TimerRef().Start();

		/// Sort array
		T minValueIndex = 0;
		for ( T i = 0; i < szArray - 1; ++i )
		{
			/// Reset minValueIndex
			minValueIndex = i;

			/// Iterate through unsorted portion
			for ( T j = i + 1; j < szArray; ++j )
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

		/// End Timer
		TimerRef().Stop();
		double et = TimerRef().GetElapsed();

		/// Print time taken to sort
		std::cout << "<Selection Sort Performance>\n";
		std::cout << "Total Time Taken : " << et << " us.\n";
		std::cout << "array Size Of : " << szArray << ".\n";
		std::cout << et / szArray << " us, per element.\n\n";

		/// Print sorted array
		PrintArray( false );
		std::cout << "\n\n";
	}

	/// <summary>
	/// Insertion sort algorithm
	/// </summary>
	/// <param name="start"></param>
	/// <param name="end"></param>
	void InsertionSort()
	{
		/// Print Insertion Sort header, Initialize array. 
		/// Print original array
		PrintAlgoName( "Insertion Sort" );
		InitArray();
		PrintArray();

				/// Get our array references
		T szArray = GetSzArrayValue();
		auto& array = ArrayRef();


		/// Start Timer
		TimerRef().Start();

		for ( T i = 1; i <= szArray - 1; i++ )
		{
			T key = array[ i ];
			T j = i - 1;

			while ( j >= 0 && array[ j ] > key )
			{
				array[ j + 1 ] = array[ j ];
				j--;
			}

			array[ j + 1 ] = key;
		}

		/// End TimerRef()
		TimerRef().Stop();
		double et = TimerRef().GetElapsed();

		/// Print time taken to sort
		std::cout << "<Insertion Sort Performance>\n";
		std::cout << "Total Time Taken : " << et << " us.\n";
		std::cout << "array Size Of : " << szArray << ".\n";
		std::cout << et / szArray << " us, per element.\n\n";

		/// Print sorted array
		PrintArray( false );
		std::cout << "\n\n";
	}



	///-------------Quick-Sort-Start-----------------///


	/// <summary>
	/// Actual sorting function inside quick sort
	/// </summary>
	/// <param name="pLow"></param>
	/// <param name="pHigh"></param>
	/// <returns></returns>
	T QuickPartition( T pLow, T pHigh )
	{
		/// Get array reference
		auto& array = ArrayRef();
		T i = pLow - 1;

		for ( T j = pLow; j < pHigh; ++j )
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
	void QuickSort( T pLow, T pHigh )
	{
		if ( pLow < pHigh )
		{
			T pIndex = QuickPartition( pLow, pHigh );
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
		PrintAlgoName( "Quick Sort" );
		InitArray();
		PrintArray();

		/// Get our array references
		T szArray = GetSzArrayValue();
		auto& array = ArrayRef();

		/// Setup piviot high low
		const T pHigh = szArray - 1;
		const T pLow = 0;


		/// Start Timer
		TimerRef().Start();

		/// Sort array
		QuickSort( pLow, pHigh );

		/// End Timer
		TimerRef().Stop();
		double et = TimerRef().GetElapsed();

		/// Print time taken to sort
		std::cout << "<Quick Sort Performance>\n";
		std::cout << "Total Time Taken : " << et << " us.\n";
		std::cout << "array Size Of : " << szArray << ".\n";
		std::cout << et / szArray << " us, per element.\n\n";

		/// Print sorted array
		PrintArray( false );
		std::cout << "\n\n";
	}

	///---------------Merge-Sort-Start---------------///

	void MergeSortInit()
	{
		/// Print Merge Sort header, Initialize array. 
		/// Print original array
		PrintAlgoName( "Merge Sort" );
		InitArray();
		PrintArray();

		/// Get buffer reference and array size
		auto& tempBuffer = TempBufferRef();
		T szArray = GetSzArrayValue();
		
		/// Set tempBuffer size
		tempBuffer.resize( szArray );

		/// Start Timer
		TimerRef().Start();

		/// Call the traditional merge sort
		MergeSort( 0, szArray - 1 );

		/// End Timer
		TimerRef().Stop();
		double et = TimerRef().GetElapsed();

		/// Print time taken to sort
		std::cout << "<Merge Sort Performance>\n";
		std::cout << "Total Time Taken : " << et << " us.\n";
		std::cout << "array Size Of : " << szArray << ".\n";
		std::cout << et / szArray << " us, per element.\n\n";

		/// Print sorted array
		PrintArray( false );
		std::cout << "\n\n";
	}

	void MergeSort( T start, T end )
	{
		/// Base case: if the subarray has 0 or 1 element, it's already sorted
		if ( start >= end ) return;

		/// Calculate the middle point to divide the array into two halves
		const T mid = start + ( end - start ) / 2;

		/// Recursively sort the first half
		MergeSort( start, mid );

		/// Recursively sort the second half
		MergeSort( mid + 1, end );

		/// Merge the sorted halves
		Merge( start, mid, end );
	}

	void Merge( T start, T mid, T end )
	{
		auto& tempBuffer = TempBufferRef();
		auto& array = ArrayRef();
		
		/// Copy elements to the temp buffer
		for ( T i = start; i <= end; i++ )
		{
			tempBuffer[ i ] = array[ i ];
		}
		
		/// Initial index of first, second halves
		T i = start;     
		T j = mid + 1;  
		/// Initial index of merged array
		T k = start;      

		/// Merge the two halves back into the original array
		while ( i <= mid && j <= end )
		{
			if ( tempBuffer[ i ] <= tempBuffer[ j ] )
			{
				array[ k++ ] = tempBuffer[ i++ ];
			} else
			{
				array[ k++ ] = tempBuffer[ j++ ];
			}
		}

		/// Copy remaining elements of the first half, if any
		while ( i <= mid )
		{
			array[ k++ ] = tempBuffer[ i++ ];
		}
	}

};

