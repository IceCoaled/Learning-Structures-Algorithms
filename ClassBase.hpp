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
#include <cstring>
#include <print>
#include <algorithm>
#include <array>
#include <ranges>

#ifdef _WIN32
#include <Windows.h>
#else
#include <clocale>
#include <sys/ioctl.h>
#include <unistd.h>
#endif

// 0th index bit count for unsigned long long
constexpr std::size_t MAX_ULL_BITS = 64;


/**
* @brief Timer class used for timing our algorithms
*
* Provides high-resolution timing capabilities using either
* std::chrono::high_resolution_clock or std::chrono::steady_clock,
* depending on which one is steady.
*/
class HighResTimer
{
	/// @brief Type alias for the clock, choosing high_resolution_clock if it's steady, otherwise steady_clock
	using Timer = typename std::conditional<std::chrono::high_resolution_clock::is_steady,
		std::chrono::high_resolution_clock, std::chrono::steady_clock>;

	/// @brief Type alias for microsecond duration representation
	using Microseconds = std::chrono::duration<double, std::micro>;

private:
	Timer::type::time_point start; ///< Timestamp when timing started
	Timer::type::time_point stop;  ///< Timestamp when timing stopped

public:
	/**
	* @brief Default constructor
	*
	* Creates a timer instance without starting it
	*/
	HighResTimer() = default;

	/**
	* @brief Default destructor
	*/
	~HighResTimer() = default;

	/**
	* @brief Start the timer
	*
	* Records the current time as the starting point
	* and initializes the stop time to the same value
	*/
	void Start()
	{
		start = Timer::type::now();
		stop = start;
	}

	/**
	* @brief Stop the timer
	*
	* Records the current time as the stopping point
	*/
	void Stop()
	{
		stop = Timer::type::now();
	}

	/**
	* @brief Get the elapsed time
	*
	* Calculates the time difference between start and stop points
	*
	* @return The elapsed time in microseconds
	*/
	double GetElapsed() const
	{
		return Microseconds( stop - start ).count();
	}

	/**
	* @brief prints start and stop times to console.
	*/
	void PrintTimes() const
	{
		std::println( "Start Time: {}, Stop Time: {}.",start.time_since_epoch().count(), stop.time_since_epoch().count() );
	}
};


/*
* @brief Constraint for our algorithm base class
* @tparam T this is required to be a integer or floating point
* variable, or custom one that implements
* std::numeric_limits::specialized characteristics properly
*/
template < typename T>
concept NumericConstraint =
std::numeric_limits<T>::is_specialized &&
std::is_arithmetic_v<T>();


/**
* @brief Base class for searching and sorting algorithmns
* It includes all the nessary functions needed for initializing
* a vector of numerics with uniform randomness. This also includes
* an xor based swap function, and the function needed to print the
* array of data.
* 
* @tparam T Numeric type that meets the NumericConstraint requirement
*/
template <typename T>
	requires NumericConstraint<T>
class AlgorithmsBase
{
protected:
	std::size_t szArray = 0; //< Size of array 
	std::vector<T> array; //< Our array
	std::vector<T> tempBuffer; //< Temp buffer for quick sort
	HighResTimer timer = HighResTimer(); //< timer for timing algorithms


public:


	/**
	* @brief Initialize array with random size and elements
	*/
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

	/**
	* @brief Resets array details
	*/
	void ResetArray()
	{
		tempBuffer.clear();
		array.clear();
		szArray = 0;
	}

	/**
	* @brief Xor Swap algorithm
	*
	* This is used as to not use a temporary variable
	*
	* @param a First value to swap
	* @param b Second value to swap
	*/
	void XorSwap( T& a, T& b ) const
	{
		/// Short circuit
		if ( a == b ) return;

		/// Swap
		a ^= b;
		b ^= a;
		a ^= b;
	}


	/**
	* @brief Print array elements
	*
	* If original is true, print original array,
	* else print sorted array.
	* Prints in traditional array format with 16 values per line.
	*
	* @param original If true, prints original array; if false, prints sorted array
	*/
	void PrintArray( const bool& original = true ) const
	{
		const T elementsPerLine = 16;  /// Number of elements to print per line

		std::println( "================================================================" );
		if ( original )
		{
			std::print( "Original array = " );
		} else
		{
			std::print( "Sorted array = " );
		}

		std::print( "{ \n" );

		for ( T i = 0; i < szArray; i++ )
		{
			/// Print the current element
			std::print("{}", array[ i ]);

			/// Decide what separator to use
			if ( i == szArray - 1 )
			{
				/// Last element gets no comma
				std::print( "" );
			} else
			{
				std::print( ", " );

				/// Add a line break after every elementsPerLine elements (except at the end)
				if ( ( i + 1 ) % elementsPerLine == 0 && i < szArray - 1 )
				{
					std::println( "\r " );  // New line with a space for indentation
				}
			}
		}

		std::println( " };" );
		std::println( "================================================================" );
	}


	/**
	* @brief Prints the sort name
	*
	* @param sortName Name of the sorting algorithm to display
	*/
	void PrintAlgoName( const std::string& sortName ) const
	{
		std::println( "================================================================" );
		std::println( "{}", sortName );
	}
};


/*
* @brief Constraint for our linked list base class
* 
* @tparam T this is required to be std::string or std::wstring
*/
template< typename T >
concept StringType = std::is_same_v< T, std::string > || 
std::is_same_v< T, std::wstring >;



/**
* @brief Node structure for a doubly-linked list of string elements
* Contains a name of string type, an entry number, and pointers to the next
* and previous nodes in the linked list.
*
* @tparam T must be std::string or std::wstring
*/
template< typename T >
	requires StringType< T >
struct StringNode
{
	T name;              //< The string data stored in this node
	int entryNum = 0; //< Entry number/identifier for this node
	StringNode* flink = nullptr; //< Forward link to the next node
	StringNode* blink = nullptr; //< Backward link to the previous node

	/**
	* @brief Default constructor
	*
	* Creates an empty node with default values
	*/
	StringNode() = default;

	/**
	* @brief Parameterized constructor
	*
	* Creates a node with specified values
	*
	* @param entryNumber The identifier number for this node
	* @param newName The string data to store in this node
	* @param newBlink Pointer to the previous node in the list
	*/
	StringNode( int entryNumber, T newName, StringNode* newBlink ):
		entryNum( entryNumber ), name( newName ), blink( newBlink ) {}

	/**
	* @brief Destructor
	*
	* Cleans up the node by nullifying pointers and resetting the entry number
	* and clearing string
	*/
	~StringNode()
	{
		flink = blink = nullptr;
		entryNum = 0;
		name.clear();
	}
};



/**
* @brief Linked list base class. Has all 
* the basic functionality needed allocate,
* deallocate, and print entries.
* 
* @tparam T must be std::string or std::wstring
* 
* @details This class DOES NOT use the head for
* any data, this is just for learning purposes
*/
template< typename T >
	requires StringType< T >
class LinkListBase
{
protected:

	StringNode<T>* head; //< Head of linked list, for mine i dont actually assign data to head
	StringNode<T>* current; //< This is just to help with ease of creating new entries
	int numOfEntries; //< Current number of entries in list( head is not included )
	bool isListWrapped; //< Flag for telling wether the linked list is wrapped or not
	HighResTimer timer;	//< timer for timing algorithms

public:

	/**
	* @brief Constructor for class, this calls InitHead
	* To initialize the class variables
	* 
	* @param isWrapped flag to tell class wether 
	* its a wrapped list or not
	*/
	explicit LinkListBase(const bool& isWrapped )
	{
		InitHead( isWrapped );
	}

	/**
	* @brief Deconstructor, this calls RemoveAllEntries
	* to clean up memory
	*/
	~LinkListBase()
	{
		RemoveAllEntries();
	}


	/**
	* @brief Initializes the head of the linked list
	*
	* Sets current to head
	*
	* @return true if the head was created else false
	*/
	bool InitHead(const bool& isWrapped )
	{
		this->isListWrapped = isWrapped;
		this->head = nullptr;
		this->current = nullptr;
		this->numOfEntries = 0;
		
		// Allocate the head
		head = new StringNode<T>();
		if ( head == nullptr )
		{
			// Return false if allocation failed
			return false;
		}
		// Set current to the head
		current = head;
		return true;
	}


	/**
	* @brief Adds new entry to linked list
	*
	* @tparam name The name to add to the new entry
	* @return true if new entry was created else false
	*/
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
			head->blink = current;
		}		
		return true;
	}


	/**
	* @brief Print the entry number and name 
	* of each entry in the linked list
	*/
	void PrintAllEntries( const bool flipped = false )
	{
		// Set our loop entry to the first
		// Entry in the list
		StringNode<T>* entry = nullptr;
		if ( !flipped )
		{
			entry = head->flink;
		} else
		{
			entry = head->blink;
		}

		// If the list is wrapped we can use the
		// Head as our stop signal
		// else we just check for nullptr
		if ( isListWrapped )
		{
			while ( entry != head )
			{
				// Print entry details
				PrintEntryDetails( entry );
				// Push our entry to the next one
				if ( !flipped )
				{
					entry = entry->flink;
				} else
				{
					entry = entry->blink;
				}
			}
		} else
		{
			while ( entry != nullptr )
			{
				// Print entry details
				PrintEntryDetails( entry );
				// Push our entry to the next one
				if ( !flipped )
				{
					entry = entry->flink;
				} else
				{
					entry = entry->blink;
				}
			}
		}
		entry = nullptr;
	}


	/**
	* @brief Deletes all entries in the linked list
	*
	* deletes head and sets head / current to nullptr,
	* Sets count to zero
	*/
	void RemoveAllEntries()
	{
		// Set our loop entry to the second
		// Entry in the list
		StringNode<T>* entry = head->flink->flink;

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
	virtual void MergeSortEntries() = 0;
	virtual T RemoveEntry( T name, const bool binarySearch ) = 0;
	virtual int RemoveEntry( int entryNum, const bool binarySearch ) = 0;
	virtual void FlipList() = 0;


protected:
	/**
	* @brief Print the entries details
	*
	* We do a null check as this pointer will be
	* passed in from outside this class
	*
	* @param entry The entry to print
	* @return true if entry wasn't null, else false
	*/
	bool PrintEntryDetails( StringNode<T>* entry ) const
	{
		// Null check
		if ( entry == nullptr )
		{
			return false;
		}

		// Print entry details
		// We just auto cast to std::string 
		// as we are using std::wstring as well
		std::println( "==============" );
		std::println(
		"Entry number: {}, Entry Name: {}",
		entry->entryNum,std::string( entry->name.begin(), entry->name.end() ) );
		std::println( "==============" );
		return true;
	}
};


#endif // !CLASSBASE_HPP

