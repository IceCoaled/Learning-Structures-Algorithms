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


/**
* @brief Parses a path string into a destination buffer and extracts folder pointers
*
* @details This function converts upper case characters to lower case and copies to
* the destination. Identifies the starting positions of each folder in the path by setting pointers
* in the provided folder pointer array. The function is designed to work at compile time
* when used with constexpr values.
*
* The function performs the following operations:
* 1. Validates input parameters
* 2. Copies the source string to the destination buffer with character transformations
* 3. Identifies folder boundaries and stores pointers to folder starts
* 4. Ensures proper null-termination
*
* @tparam szDest Size of the destination character array
* @tparam szFldrPtrs Size of the folder pointers array
*
* @param[out] dest Destination character array to store the processed path
* @param[in] strIn Source string containing the path to parse
* @param[out] fldrPtrsOut Array to store pointers to the beginning of each folder in the path
*
* @return errno_t Error code (0 for success, otherwise an error code from errno.h)
*   - EINVAL: Source string pointer is nullptr
*   - ERANGE: Destination array size is zero
*   - EDOM: Too many folders to fit in the folder pointers array
*   - ESPIPE: Failed to fully parse source string (destination buffer too small)
*
* @note Path separators ('/' and '\\') are recognized and used to identify folder boundaries
* @warning This function must be used with valid compile-time constant expressions for constexpr evaluation
*/
template<std::size_t szDest, std::size_t szFldrPtrs>
constexpr errno_t ConstexprParseStrings( char( &dest )[ szDest ], const char* strIn, char* ( &fldrPtrsOut )[ szFldrPtrs ] ) noexcept
{
	// Validate the source string pointer
	if ( strIn == nullptr )
	{
		static_assert( "ConstexprParseStrings: Source String Pointer Is Invalid\n" );
		// Invalid argument errno
		return EINVAL;  
	}

	// Validate the destination buffer size
	if ( szDest == 0 )
	{
		static_assert( "ConstexprParseStrings: Size Of Destination Array Is Invalid\n" );
		// Out of range errno
		return ERANGE; 
	}

	int count = 0;
	bool isToken = false;
	int tokenCnt = 0; 
	char* destPtr = dest;

	while ( *strIn != '\0' && count < szDest )
	{
		// Convert any upper case to lower
		if ( *strIn > 0x0040 || *strIn < 0x005B )  
		{
			*destPtr = *strIn ^ 0x0020; 
		} else
		{
			*destPtr = *strIn;
		}

		if ( *destPtr == '/' || *destPtr == '\\' )
		{
			isToken = true; 
		} else if ( isToken && ( *destPtr != '/' || *destPtr != '\\' ) )
		{
			// If this is the character after a separator, it's the start of a folder name
			if ( tokenCnt >= szFldrPtrs )
			{
				static_assert( "ConstexprParseStrings: To Many Folders to Fit in Folder Pointers Array\n" );
				return EDOM;  // Domain error - array capacity exceeded
			}

			isToken = false; 
			// Store pointer to the folder start
			fldrPtrsOut[ tokenCnt ] = destPtr;
			++tokenCnt;                     
		}
		++destPtr;
		++strIn;
		++count;
	}

	// Check if we processed the entire input string
	if ( *strIn != '\0' )
	{
		static_assert( "ConstexprParseStrings: Failed To Fully Parse Source String\n" );
		// Invalid seek ernno
		return ESPIPE;  
	}

	// there isnt a ernno for success
	// But errno functions always return
	// 0 for success
	return 0;
}



 
/**
* @struct PathData
* @brief Consolidates path-related data for file operations with optimized folder pointer handling.
*
* This structure encapsulates all path-related information including filename, filepath,
* pointers to folder components within the path, and drive information. It provides
* optimized methods for managing and accessing path components.
*
* @note The folderPtrs array uses MAX_ULL_BITS to set its size, providing ample storage
* for all possible folder levels in a path.
*/
struct PathData
{
	char fileName[ _MAX_PATH ] = { 0 };        
	char filePath[ _MAX_PATH ] = { 0 };         
	char* folderPtrs[ MAX_ULL_BITS ] = { nullptr };          
	char driveLetter = '\0';                  

	/**
	* @brief sets all path data to null values.
	*
	* Uses memset to efficiently clear all buffers and pointers in a single operation
	* per data member, ensuring the struct is in a clean state for reuse.
	*/
	constexpr void ZeroPathData() 
	{
		memset( ( ( void* )&fileName ), '\0', sizeof( fileName ) );
		memset( ( ( void* )&filePath ), '\0', sizeof( filePath ) );
		memset( ( ( void* )&folderPtrs ), NULL, sizeof( folderPtrs ) );
		driveLetter = '\0';
	}
};




/**
* @brief A compile-time accessible file data structure for managing file information.
* This is for my next adventure in learning binary trees
*
* @details This structure is designed to be fully usable in constexpr contexts,
* allowing compile-time construction, manipulation, and tree operations.
* All methods are marked constexpr to enable compile-time evaluation where possible.
* This design choice enables static verification and optimization by the compiler.
*
* The structure stores file names and paths as fixed-size character arrays,
* and maintains hash keys for efficient comparison operations.
* It organizes data in a binary tree structure via left and right child pointers.
*
* @note Memory management is crucial when working with this structure.
* The struct owns its child pointers and is responsible for their deletion.
*
* @warning Not thread-safe. Concurrent access must be externally synchronized.
*/
struct FileData
{
private:
	PathData pathInfo = PathData();
	std::size_t nameKey = 0;
	std::size_t pathKey = 0;
	FileData* parent = nullptr;
	FileData* leftChild = nullptr;
	FileData* rightChild = nullptr;
	

public:

	/**
	* @brief Default constructor.
	*
	* Creates an empty FileData object with all members initialized to their default values.
	*/
	constexpr FileData() = default;

	
	/**
	* @brief Move constructor.
	*
	* Constructs a FileData by taking ownership of the resources from another FileData object.
	*
	* @param otherData The FileData object to move from.
	*/
	explicit constexpr FileData( FileData&& otherData ) noexcept
	{
		*this = std::exchange( *this, otherData );
	}
	
	
	/**
	* @brief Copy constructor.
	*
	* Constructs a FileData by copying the contents of another FileData object.
	*
	* @param otherData The FileData object to copy from.
	* @throw std::runtime_error If copying the file name or path fails, or if child nodes are null,
	* or if the hash keys don't match after copying.
	*/
	explicit constexpr FileData( const FileData& otherData )
	{
		// Zero out our path information struct
		this->pathInfo.ZeroPathData();
		
		// Copy over our strings
		if ( ConstexprParseStrings< _MAX_PATH, MAX_ULL_BITS >( this->pathInfo.fileName, otherData.GetFileName(), this->pathInfo.folderPtrs ) != 0 )
		{
			throw std::runtime_error( "Failed to copy fileNameIn to FileData struct" );
		}

		if ( ConstexprParseStrings< _MAX_PATH, MAX_ULL_BITS >( this->pathInfo.filePath, otherData.GetFilePath(), this->pathInfo.folderPtrs ) != 0 )
		{
			throw std::runtime_error( "Failed to copy filePathIn to FileData struct" );
		}

		// Set the child node pointers
		this->leftChild = otherData.LeftChild();
		this->rightChild = otherData.RightChild();
		this->parent = otherData.Parent();

		// A just in case check
		if ( this->leftChild == nullptr || this->rightChild == nullptr )
		{
			throw std::runtime_error( "Child nodes are null" );
		}

		// Set our key values, we will check these
		// After as well
		this->nameKey = otherData.GetFileNameKey();
		this->pathKey = otherData.GetFilePathKey();

		if ( this->nameKey != otherData.GetFileNameKey() ||
			 this->pathKey != otherData.GetFilePathKey() )
		{
			throw std::runtime_error( "Failed to set file name or path hash key" );
		}

		// Get drive letter
		this->pathInfo.driveLetter = otherData.GetDriveLetter();

		if ( this->pathInfo.driveLetter != otherData.GetDriveLetter() )
		{
			throw std::runtime_error( "Failed to set drive letter" );
		}
	}

	
	/**
	* @brief Constructs a FileData object with C-style string parameters.
	*
	* @param filePathIn The file path as a null-terminated string.
	* @param fileNameIn The file name as a null-terminated string.
	* @param lCIn Pointer to the left child node (default: nullptr).
	* @param rCIn Pointer to the right child node (default: nullptr).
	*
	* @throws std::runtime_error If copying the file name or path fails.
	*/
	constexpr FileData( char* const filePathIn, char* const fileNameIn, FileData* const lCIn = nullptr, FileData* const rCIn = nullptr, FileData* const prnt = nullptr )
	{
		// Zero out our path information struct
		this->pathInfo.ZeroPathData();
		
		// Copy over our strings
		if ( ConstexprParseStrings< _MAX_PATH, MAX_ULL_BITS >( this->pathInfo.fileName, fileNameIn, this->pathInfo.folderPtrs ) != 0 )
		{
			throw std::runtime_error( "Failed to copy fileNameIn to FileData struct" );
		}

		if ( ConstexprParseStrings< _MAX_PATH, MAX_ULL_BITS >( this->pathInfo.filePath, filePathIn, this->pathInfo.folderPtrs ) != 0 )
		{
			throw std::runtime_error( "Failed to copy filePathIn to FileData struct" );
		}
		// Set the child node pointers
		this->leftChild = lCIn;
		this->rightChild = rCIn;
		this->parent = prnt;
		// Hash our name and path for keys
		this->nameKey = HashKey( fileNameIn );
		this->pathKey = HashKey( filePathIn );
		// Get our drive letter
		this->pathInfo.driveLetter = filePathIn[ 0 ];
	}

	
	/**
	* @brief Constructs a FileData object with std::string parameters.
	*
	* @param filePathIn The file path as a std::string.
	* @param fileNameIn The file name as a std::string.
	* @param lCIn Pointer to the left child node (default: nullptr).
	* @param rCIn Pointer to the right child node (default: nullptr).
	*
	* @throws std::runtime_error If copying the file name or path fails.
	*/
	constexpr FileData( const std::string& filePathIn, const std::string& fileNameIn, FileData* lCIn = nullptr, FileData* rCIn = nullptr, FileData* const prnt = nullptr )
	{
		// Zero out our path information struct
		this->pathInfo.ZeroPathData();

		// Copy over our strings
		if ( ConstexprParseStrings< _MAX_PATH, MAX_ULL_BITS >( this->pathInfo.fileName, fileNameIn.c_str(), this->pathInfo.folderPtrs ) != 0 )
		{
			throw std::runtime_error( "Failed to copy fileNameIn to FileData struct" );
		}

		if ( ConstexprParseStrings< _MAX_PATH, MAX_ULL_BITS >( this->pathInfo.filePath, filePathIn.c_str(), this->pathInfo.folderPtrs ) != 0 )
		{
			throw std::runtime_error( "Failed to copy filePathIn to FileData struct" );
		}
		// Set the child node pointers
		this->leftChild = lCIn;
		this->rightChild = rCIn;
		this->parent = prnt;
		// Hash our name and path for keys
		this->nameKey = HashKey( fileNameIn.c_str() );
		this->pathKey = HashKey( filePathIn.c_str() );
		// Get our drive letter
		this->pathInfo.driveLetter = filePathIn[ 0 ];
	}


	/**
	* @brief Pointer assignment operator.
	*
	* Assigns the content of a FileData pointer to this object.
	*
	* @param otherData Pointer to the FileData object to assign from.
	* @return Pointer to this FileData after assignment.
	* @note This operator is marked noexcept and will not throw exceptions.
	*/
	FileData* operator= ( const FileData* otherData ) noexcept
	{
		return *this = otherData;
	}


	/**
	* @brief Copy assignment operator.
	*
	* Assigns the content of another FileData object to this object.
	*
	* @param otherData The FileData object to assign from.
	* @return Reference to this FileData after assignment.
	*/
	FileData& operator= ( const FileData& otherData )
	{
		auto temp = FileData( otherData );
		std::swap( *this, temp );
		return *this;
	}




    /**
	* @brief Move assignment operator.
	*
	* Assigns the content of another FileData object to this object using move semantics.
	*
	* @param otherData The FileData object to move-assign from.
	* @return Reference to this FileData after assignment.
	* @note This operator is marked noexcept and will not throw exceptions.
	*/
	FileData& operator= ( FileData&& otherData )  noexcept
	{
		std::swap( *this, otherData );
		return *this;
	}


	/**
	* @brief Destructor that cleans up child nodes.
	*
	* @details If this gets called it will cause a recursive deletion of all 
	* nodes below the node this was called on in the tree.
	*/
	~FileData()
	{
		if ( this->leftChild != nullptr ) delete this->leftChild;
		if ( this->rightChild != nullptr ) delete this->rightChild;
	}


	/**
	* @brief Gets the drive letter.
	*
	* @return drive letter.
	*/
	constexpr char GetDriveLetter() const
	{
		return this->pathInfo.driveLetter;
	}

	
	/**
	* @brief Gets the hash key for the file name.
	*
	* @return The hash key value for the file name.
	*/
	constexpr std::size_t GetFileNameKey() const&
	{
		return this->nameKey;
	}

	/**
	* @brief Gets the hash key for the file path.
	*
	* @return The hash key value for the file path.
	*/
	constexpr std::size_t GetFilePathKey() const&
	{
		return this->pathKey;
	}

	/**
	* @brief Gets the file name as a const char pointer (for rvalue references).
	*
	* @return Const pointer to the file name string.
	*/
	constexpr const char* GetFileNameRVal() const&
	{
		auto result = this->pathInfo.fileName;
		return result;
	}

	/**
	* @brief Gets the file name as a const char pointer.
	*
	* @return Const pointer to the file name string.
	*/
	constexpr const char* GetFileName() const
	{
		auto result = this->pathInfo.fileName;
		return result;
	}

	/**
	* @brief Gets the file path as a const char pointer (for rvalue references).
	*
	* @return Const pointer to the file path string.
	*/
	constexpr const char* GetFilePathRVal() const&
	{
		auto result = this->pathInfo.filePath;
		return result;
	}

	/**
	* @brief Gets the file path as a const char pointer.
	*
	* @return Const pointer to the file path string.
	*/
	constexpr const char* GetFilePath() const
	{
		auto result = this->pathInfo.filePath;
		return result;
	}

	/**
	* @brief Gets a pointer to the left child node.
	*
	* @return Pointer to the left child FileData object, or nullptr if no left child exists.
	*/
	constexpr FileData* LeftChild() const
	{
		return this->leftChild;
	}

	/**
	* @brief Gets a reference to the left child node.
	*
	* @warning Will cause undefined behavior if leftChild is nullptr.
	*
	* @return Reference to the left child FileData object.
	*/
	constexpr FileData& LeftChildRef() const&
	{
		return *this->leftChild;
	}

	/**
	* @brief Gets a pointer to the right child node.
	*
	* @return Pointer to the right child FileData object, or nullptr if no right child exists.
	*/
	constexpr FileData* RightChild() const
	{
		return this->rightChild;
	}

	/**
	* @brief Gets a reference to the right child node.
	*
	* @warning Will cause undefined behavior if rightChild is nullptr.
	*
	* @return Reference to the right child FileData object.
	*/
	constexpr FileData& RightChildRef() const&
	{
		return *this->rightChild;
	}


	/**
	* @brief Gets a pointer to the parent node.
	*
	* @warning Will cause undefined behavior if rightChild is nullptr.
	*
	* @return pointer to the parent FileData object, or nullptr if no right child exists.
	*/
	constexpr FileData* Parent() const
	{
		return this->parent;
	}


	/**
	* @brief Gets a reference to the parent node.
	*
	* @warning Will cause undefined behavior if rightChild is nullptr.
	*
	* @return Reference to the parent FileData object.
	*/
	constexpr FileData& ParentRef() const&
	{
		return *this->parent;
	}

	/**
	* @brief Compares this FileData's name key with another key.
	*
	* @param otherKey The key to compare against.
	* @return true if the keys match, false otherwise.
	*/
	constexpr bool CompareNameKeys( const std::size_t otherKey ) const
	{
		return this->nameKey == otherKey;
	}

	/**
	* @brief Compares this FileData's path key with another key.
	*
	* @param otherKey The key to compare against.
	* @return true if the keys match, false otherwise.
	*/
	constexpr bool ComparePathKeys( const std::size_t otherKey ) const
	{
		return this->pathKey == otherKey;
	}
private:

	
	/**
	* @brief This is a hash algorithm i created
	* you can find more on it at
	* https://github.com/IceCoaled/UserMode-KernelMode-Asm-Functions/blob/main/CustomHash.asm
	*/
	constexpr std::size_t HashKey( const char* filePathIn ) const
	{
		std::size_t hValue = 0x030153912FF;		
		std::size_t result = 0;

		while ( *filePathIn )
		{
			const char c = *filePathIn;
			{
				result ^= c;
				result *= hValue;
				hValue -= result;
				result = RoR( result, 0x0010 );
				result <<= 0x0006;
			}
			++filePathIn;
		}
		return result;
	}

	/**
	* @brief Simple roll right bit manipulation for,
	* our hash algorithim. ive also included roll 
	* left just because.
	* 
	* @details These are locked to ull for our purpose,
	* but youd be better off using a template with 
	* std::numeric_limits stuff.
	* 
	* @param value Value to have the roll done
	* @param roll How many bits you want to roll the value
	* 
	* @return std::size_t of rolled value
	*/
	constexpr std::size_t RoR( std::size_t value, std::uint32_t roll ) const
	{
		return ( ( value >> roll ) | ( value << ( MAX_ULL_BITS - roll ) ) );
	}

	constexpr std::size_t RoL( std::size_t value, std::uint32_t roll ) const
	{
		return ( ( value << roll ) | ( value >> ( MAX_ULL_BITS - roll ) ) );
	}
};









static class UniqueFileGenerator
{
	static constexpr const std::size_t SZ_MAX_ARRAY = 26;


public:
	// Generate a unique filename based on an index (0-25)
	static constexpr const char* getFileName()
	{
		// Array of 26 unique filenames
		static constexpr std::array<std::string_view, 26> filenames = {
			"alpha.dat", "bravo.bin", "charlie.cfg", "delta.db", "echo.exe",
			"foxtrot.fil", "golf.gz", "hotel.h", "india.ini", "juliet.jpg",
			"kilo.key", "lima.log", "mike.md", "november.nfo", "oscar.obj",
			"papa.pdf", "quebec.qt", "romeo.rtf", "sierra.sys", "tango.tmp",
			"uniform.url", "victor.vbs", "whiskey.wav", "xray.xml", "yankee.yml",
			"zulu.zip"
		};
		return filenames[ GetRandomIndex() ].data();
	}

	// Generate a unique filepath based on an index (0-25)
	static constexpr const char* getFilePath( size_t index )
	{
		// Array of 26 unique filepaths with full paths and various drive letters
		static constexpr std::array<std::string_view, 26> filepaths = {
			"C:/Program Files/App/data.bin", "C:/Windows/System32/drivers/file.sys",
			"C:/Users/Admin/Documents/report.pdf", "C:/ProgramData/Application/logs/system.log",
			"D:/Projects/Website/index.html", "D:/Downloads/installer.exe",
			"D:/Media/Music/album/track01.mp3", "D:/Backup/2023/January/backup.zip",
			"E:/Games/RPG/saves/character.sav", "E:/Virtual Machines/Linux/disk.img",
			"E:/Photos/Vacation/img0001.jpg", "E:/Videos/Family/birthday.mp4",
			"F:/Work/Presentations/quarterly.pptx", "F:/Documents/Financial/taxes2023.xlsx",
			"F:/Source/Repository/project/main.cpp", "F:/Archives/Old Projects/legacy.tar.gz",
			"G:/Temp/extract/contents.txt", "G:/Books/Technical/programming.epub",
			"G:/Research/Papers/2023/findings.docx", "G:/Database/Backups/db_dump.sql",
			"H:/External/Shared/company_logo.png", "H:/Transfer/Incoming/received_file.dat",
			"H:/Raw Data/Sensors/readings.csv", "H:/Scripts/Automation/daily_task.bat",
			"I:/Recovery/System Image/os_backup.img", "I:/Utilities/Portable Apps/text_editor.exe"
		};
		return filepaths[ index % 26 ].data();
	}


	/**
	* @brief Used to get random index for our
	* file paths and names
	*
	* @return std::uint32_t with random seed
	*/
	static constexpr std::uint32_t GetRandomIndex()
	{
		return Random() % SZ_MAX_ARRAY;
	}

	/**
	* @brief Used to generate a seed using
	* a string created from time
	*
	* @return std::uint32_t with random seed
	*/
	static constexpr std::uint32_t Seed()
	{
		return
			( ( __TIME__[ 0 ] - '0' ) * 10 + ( __TIME__[ 1 ] - '0' ) ) * 3600 +  // Hours
			( ( __TIME__[ 3 ] - '0' ) * 10 + ( __TIME__[ 4 ] - '0' ) ) * 60 +     // Minutes
			( ( __TIME__[ 6 ] - '0' ) * 10 + ( __TIME__[ 7 ] - '0' ) );           // Seconds
	}

	/**
	* @brief Linear Congruential Generator
	*
	* @param seed Random seed value From Seed function
	* @return std::uint32_t with random number
	*/
	static constexpr std::uint32_t Random()
	{
		return ( Seed() * 1664525u + 1013904223u ) % 0xFFFFFFFFu;
	}
};


#endif // !CLASSBASE_HXX

