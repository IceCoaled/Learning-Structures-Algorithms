#include "ClassBase.hpp"


template <typename T>
	requires NumericConstraint<T>
class SortingAlgorithms: public AlgorithmsBase<T>
{

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
		this->PrintAlgoName("Bubble Sort");
		this->InitArray();

		// Edge case check
		if ( this->szArray == 0 || this->szArray == 1 )
		{
			return;
		}

		this->PrintArray();

		/// Start Timer
		this->timer.Start();

		/// Sort array
		for ( T i = 0; i < this->szArray - 1; ++i )
		{
			for ( T j = 0; j < this->szArray - i - 1; ++j )
			{
				if ( this->array[ j ] > this->array[ j + 1 ] )
				{
					this->XorSwap( this->array[ j ], this->array[ j + 1 ] );
				}
			}

		}

		/// End Timer
		this->timer.Stop();
		double et = this->timer.GetElapsed();

		/// Print time taken to sort
		std::cout << "<Bubble Sort Performance>\n";
		std::cout << "Total Time Taken : " << et << " us.\n";
		std::cout << "array Size Of : " << this->szArray << ".\n";
		std::cout << et / this->szArray << " us, per element.\n\n";

		/// Print sorted array
		this->PrintArray( false );
		std::cout << "\n\n";
	}


	/// <summary>
	/// Selection sort algorithm
	/// </summary>
	void SelectionSort()
	{
		/// Print Selection Sort header, Initialize array. 
		/// Print original array
		this->PrintAlgoName( "Selection Sort" );
		this->InitArray();

		// Edge case check
		if ( this->szArray == 0 || this->szArray == 1 )
		{
			return;
		}

		this->PrintArray();

		/// Start Timer
		this->timer.Start();

		/// Sort array
		T minValueIndex = 0;
		for ( T i = 0; i < this->szArray - 1; ++i )
		{
			/// Reset minValueIndex
			minValueIndex = i;

			/// Iterate through unsorted portion
			for ( T j = i + 1; j < this->szArray; ++j )
			{
				if ( this->array[ j ] <= this->array[ minValueIndex ] )
				{
					minValueIndex = j;
				}
			}
			if ( minValueIndex != i )
			{
				/// Make swap
				this->XorSwap( this->array[ i ], this->array[ minValueIndex ] );
			}
		}

		/// End Timer
		this->timer.Stop();
		double et = this->timer.GetElapsed();

		/// Print time taken to sort
		std::cout << "<Selection Sort Performance>\n";
		std::cout << "Total Time Taken : " << et << " us.\n";
		std::cout << "array Size Of : " << this->szArray << ".\n";
		std::cout << et / this->szArray << " us, per element.\n\n";

		/// Print sorted array
		this->PrintArray( false );
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
		this->PrintAlgoName( "Insertion Sort" );
		this->InitArray();

		// Edge case check
		if ( this->szArray == 0 || this->szArray == 1 )
		{
			return;
		}

		this->PrintArray();

		/// Start Timer
		this->timer.Start();

		T j;
		T comparand;
		for ( T i = 1; i <= this->szArray - 1; ++i )
		{
			comparand = this->array[ i ];
			j = i - 1;
			while ( j >= 0 && this->array[ j ] > comparand )
			{
				this->array[ j + 1 ] = this->array[ j ];
				--j;
			}

			this->array[ j + 1 ] = comparand;
		}

		/// End this->timer
		this->timer.Stop();
		double et = this->timer.GetElapsed();

		/// Print time taken to sort
		std::cout << "<Insertion Sort Performance>\n";
		std::cout << "Total Time Taken : " << et << " us.\n";
		std::cout << "array Size Of : " << this->szArray << ".\n";
		std::cout << et / this->szArray << " us, per element.\n\n";

		/// Print sorted array
		this->PrintArray( false );
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
		T i = pLow - 1;

		for ( T j = pLow; j < pHigh; ++j )
		{
			if ( this->array[ j ] <= this->array[ pHigh ] )
			{
				this->XorSwap( this->array[ j ], this->array[ ++i ] );
			}
		}

		this->XorSwap( this->array[ ++i ], this->array[ pHigh ] );

		return i;
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
		this->PrintAlgoName( "Quick Sort" );
		this->InitArray();

		// Edge case check
		if ( this->szArray == 0 || this->szArray == 1 )
		{
			return;
		}

		this->PrintArray();

		/// Setup piviot high low
		const T pHigh = this->szArray - 1;
		const T pLow = 0;


		/// Start Timer
		this->timer.Start();

		/// Sort array
		QuickSort( pLow, pHigh );

		/// End Timer
		this->timer.Stop();
		double et = this->timer.GetElapsed();

		/// Print time taken to sort
		std::cout << "<Quick Sort Performance>\n";
		std::cout << "Total Time Taken : " << et << " us.\n";
		std::cout << "array Size Of : " << this->szArray << ".\n";
		std::cout << et / this->szArray << " us, per element.\n\n";

		/// Print sorted array
		this->PrintArray( false );
		std::cout << "\n\n";
	}

	///---------------Merge-Sort-Start---------------///

	void MergeSortInit()
	{
		/// Print Merge Sort header, Initialize array. 
		/// Print original array
		this->PrintAlgoName( "Merge Sort" );
		this->InitArray();

		// Edge case check
		if ( this->szArray == 0 || this->szArray == 1 )
		{
			return;
		}

		this->PrintArray();
		
		/// Set tempBuffer size
		this->tempBuffer.resize( this->szArray );

		/// Start Timer
		this->timer.Start();

		/// Call the traditional merge sort
		MergeSort( 0, this->szArray - 1 );

		/// End Timer
		this->timer.Stop();
		double et = this->timer.GetElapsed();

		/// Print time taken to sort
		std::cout << "<Merge Sort Performance>\n";
		std::cout << "Total Time Taken : " << et << " us.\n";
		std::cout << "array Size Of : " << this->szArray << ".\n";
		std::cout << et / this->szArray << " us, per element.\n\n";

		/// Print sorted array
		this->PrintArray( false );
		std::cout << "\n\n";
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

};

