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
#include <memory>
#include <cassert>
#include <__msvc_string_view.hpp>

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



template< typename T >
concept StringType = std::is_same_v< T, std::string > || 
std::is_same_v< T, std::wstring >;


template< typename T >
	requires StringType< T >
struct StringNode
{
	T name;
	int entryNum;
	StringNode* flink;
	StringNode* blink;

	/// <summary>
	/// Default constructor
	/// </summary>
	StringNode(): entryNum( 0 ), flink( nullptr ), blink( nullptr ) {}

	/// <summary>
	/// New entry constructor
	/// </summary>
	/// <param name="entryNumber"></param>
	/// <param name="newFlink"></param>
	/// <param name="newBlink"></param>
	StringNode( int entryNumber,T newName ,StringNode* newBlink ): 
		entryNum( entryNumber ), name( newName ), flink( nullptr ), blink( newBlink ) {}
};




template< typename T >
	requires StringType< T >
class LinkListBase
{
protected:
	/// <summary>
	/// Head of our linked list
	/// </summary>
	StringNode<T>* head;

	/// <summary>
	/// Current entry of our list
	/// This helps use with adding new entries
	/// </summary>
	StringNode<T>* current;

	/// <summary>
	/// Number of entries in our list
	/// </summary>
	int numOfEntries;

	/// <summary>
	/// Flag to tell if list is wrapped
	/// Back to head
	/// </summary>
	bool isListWrapped;

public:

	LinkListBase(): head( nullptr ), current( nullptr ), numOfEntries( 0 ), isListWrapped( false )
	{
		InitHead();
	}
	~LinkListBase()
	{
		RemoveAllEntries();
	}


	/// <summary>
	/// Initializes the head of the linked list
	/// Sets current to head
	/// </summary>
	/// <returns></returns>
	bool InitHead()
	{
		// Allocate the head
		head = new StringNode<T>();
		if ( head == nullptr )
		{
			// Return false if allocation failed
			return false;
		}
		// Set current to the head
		current = head
		return true;
	}


	/// <summary>
	/// Adds new entry to linked list
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	bool AddEntry( T name )
	{
		// Allocate new entry
		current->flink = new StringNode<T>( ++numOfEntries, name, current );
		if ( current->flink == nullptr )
		{
			// Return false if allocation failed
			return false;
		}
		// Push current forward to new entry
		current = current->flink;
		// If the list is to be wrapped we reset the head
		if ( isListWrapped )
		{
			current->flink = head;
		}		
		return true;
	}

	/// <summary>
	/// Print the entry number and name 
	/// Of each entry in the linked list
	/// </summary>
	void PrintAllEntries()
	{
		// Set our loop entry to the first
		// Entry in the list
		StringNode<T>* entry = head->flink;

		// If the list is wrapped we can use the
		// Head as our stop signal
		// else we just check for nullptr
		if ( isListWrapped )
		{
			while ( entry != head )
			{
				// Print entry details
				std::cout << "==============\n";
				std::cout << "Entry number : " << entry->entryNum << "\n";
				std::cout << "Entry name : " << entry->name << "\n";
				std::cout << "==============\n";
				// Push our entry to the next one
				entry = entry->flink;
			}
		} else
		{
			while ( entry != nullptr )
			{
				// Print entry details
				std::cout << "==============\n";
				std::cout << "Entry number : " << entry->entryNum << "\n";
				std::cout << "Entry name : " << entry->name << "\n";
				std::cout << "==============\n";
				// Push our entry to the next one
				entry = entry->flink;
			}
		}
	}


	/// <summary>
	/// This deletes all entries in the linked list
	/// Including the head and resets count to zero
	/// </summary>
	void RemoveAllEntries()
	{
		// Set our loop entry to the second
		// Entry in the list
		StringNode<T>* entry = head->flink;

		// If the list is wrapped we can use the
		// Head as our stop signal
		// else we just check for nullptr
		if ( isListWrapped )
		{
			while ( entry != head )
			{
				// We delete the entries behind
				// Us as we move forward through list
				delete entry->blink;
				// Push our entry to the next one
				entry = entry->flink;
			}
		} else
		{
			while ( entry != nullptr )
			{
				// We delete the entries behind
				// Us as we move forward through list
				delete entry->blink;
				// Push our entry to the next one
				entry = entry->flink;
			}
		}		
		// Delete head to finish
		delete head;
		head = nullptr;
		current = nullptr;
		numOfEntries = 0;
	}

	/*
	* Abstract functions for derived classes
	* To implement as we can add sorting and
	* Searching algorithms to help these functions
	*/
	virtual void SortEntriesByName() = 0;
	virtual T RemoveEntry( T name ) = 0;
	virtual T RemoveEntry( int entryNum ) = 0;
	virtual void RemoveEntry( T name ) = 0;
	virtual void RemoveEntry( int entryNum ) = 0;
	virtual void FlipList() = 0;

protected:

	/// <summary>
	/// Print the entries details
	/// We do a null check as this pointer will be
	/// Passed in from outside this class
	/// </summary>
	/// <param name="entry"></param>
	/// <returns></returns>
	bool PrintEntryDetails( StringNode<T>* entry ) const
	{
		// Null check
		if ( entry == nullptr )
		{
			return false;
		}

		// Print entry details
		std::cout << "==============\n";
		std::cout << "Entry number : " << entry->entryNum << "\n";
		std::cout << "Entry name : " << entry->name << "\n";
		std::cout << "==============\n";

		return true;
	}
};



#endif // !CLASSBASE_HXX

