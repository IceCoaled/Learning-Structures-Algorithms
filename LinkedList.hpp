#include "ClassBase.hpp"
#include <expected>
#include <immintrin.h>
#include <intrin.h>
#include "IntrinsicCheck.hpp"

/**
* @brief This class implements merge sort and
* searching through the linked list. We can search
* for a name or entry number. This class handles
* performance measurements as well.
* 
* @tparam T Must be std::string or std::wstring
*/
template < typename T >
	requires StringType< T >
class LinkedListAlgorithms: public LinkListBase<T>
{ 
private:
	bool iSupportFlag;

	/// Using single initialization for this stuff
	// Since it is used multiple times
	std::random_device rd;
	std::mt19937 gen;
	std::vector<T> allNames;
	std::uniform_int_distribution<std::size_t> rdNameDist;

public:

	explicit LinkedListAlgorithms( const bool& wrap ): LinkListBase< T >( wrap )
	{ 
		Init();
		CreateAllEntries();
	}  
	~LinkedListAlgorithms() = default;

private:
	///-----------------Merge-Sort-------------------////

	/**
	* @brief Splits the linked list chunk into 2 parts
	* 
	* This uses 2 pointer method to get the middle
	* of the chuck. assigns mid + 1 to our return,
	* and detaches the flinks and binks to seperate 
	* the chunk
	* 
	* @param chunk the linked list chunk we want
	* to split
	* 
	* @return The base of the upper half of the
	* split chunk
	*/
	StringNode<T>* MergeSplit( StringNode<T>* chunk )
	{
		// Set current to the first node in the list
		StringNode<T>* pHigh = chunk->flink;
		StringNode<T>* pLow = chunk->flink;

		// Move the high pointer twice as fast
		// This naturally leaves the low pointer at the middle by the end
		while ( pHigh != chunk && pHigh != nullptr && pLow != nullptr )
		{
			if ( pHigh->flink->flink == nullptr || pLow->flink == nullptr )
			{
				break;
			} 
			pHigh = pHigh->flink->flink;
			pLow = pLow->flink;
		}

		StringNode<T>* midNode = pLow->flink;
		if ( midNode != nullptr )
		{
			pLow->flink = nullptr;
			midNode->blink = nullptr;
		}
		return midNode;
	}

	StringNode<T>* Merge( StringNode<T>* chunkOne, StringNode<T>* chunkTwo )
	{
		if ( chunkOne == nullptr )
		{
			return chunkTwo;
		} else if ( chunkTwo == nullptr )
		{
			return chunkOne;
		}

		int nameComp;
		if ( this->iSupportFlag )
		{
			nameComp = CompNamesV( chunkOne->name, chunkTwo->name );
		}
		else
		{
			nameComp = CompNames( chunkOne->name, chunkTwo->name );
		}
		
		if ( nameComp <= 0 )
		{
			chunkOne->flink = Merge( chunkOne->flink, chunkTwo );
			if ( chunkOne->flink != nullptr )
			{
				chunkOne->flink->blink = chunkOne;
			}
			chunkOne->blink = nullptr;
			return chunkOne;
		} else
		{
			chunkTwo->flink = Merge( chunkOne, chunkTwo->flink );
			if ( chunkTwo->flink != nullptr )
			{
				chunkTwo->flink->blink = chunkTwo;
			}
			chunkTwo->blink = nullptr;
			return chunkTwo;
		}
	}

	StringNode<T>* MergeSort( StringNode<T>* firstEntry )
	{
		if ( firstEntry == nullptr || firstEntry->flink == nullptr )
		{
			return firstEntry;
		}

		StringNode<T>* secondChunk = MergeSplit( firstEntry );

		firstEntry = MergeSort( firstEntry );
		secondChunk = MergeSort( secondChunk );

		return Merge( firstEntry, secondChunk );
	}

public:
	void MergeSortEntries()
	{
		if ( this->head->flink != nullptr || this->head->flink->flink != nullptr )
		{
			this->head->flink = MergeSort( this->head->flink );
		}

		if ( this->isListWrapped )
		{
			this->head->blink = GetLastEntry();
			this->head->blink->flink = this->head;
		}

		this->PrintAllEntries();
	}


	///-----------------Utils----------------------///

	/**
	* @brief This function uses vectorization to get lower case
	* version of strings, and compares them for alphabetical order.
	*
	* @param nameOne The first name to compare.
	* @param nameTwo The second name to compare.
	* @return int
	* - Negative number if nameOne comes before nameTwo.
	* - Positive number if nameTwo comes before nameOne.
	* - 0 if both names are the same.
	*
	* @exception std::exception Throws exception in case wchar_t is using UTF-32.
	*/
	int CompNamesV( const T& nameOne, const T& nameTwo ) const
	{
		// Our 256bit vector structs for intrinsic function
		// Zero the memory out just in case
		__m256i nOne = { 0 };
		__m256i nTwo = { 0 };
		__stosb( nOne.m256i_u8, 0x0000, sizeof( nOne.m256i_u8 ) );
		__stosb( nTwo.m256i_u8, 0x0000, sizeof( nTwo.m256i_u8 ) );

		
		if constexpr ( std::is_same_v< T, std::string > )
		{
			__movsb( nOne.m256i_u8, ( ( unsigned char* ) nameOne.c_str() ), nameOne.length() );
			__movsb( nTwo.m256i_u8, ( ( unsigned char* ) nameTwo.c_str() ), nameTwo.length() );
			
			// Load our vector struct into avx register 
			nOne = _mm256_loadu_si256( &nOne );
			
			// Get our mask of what letters are capitals and lower case
			auto upperMask = _mm256_and_si256(
				_mm256_cmpgt_epi8( nOne, _mm256_set1_epi8( 0x0040 ) ),  
				_mm256_cmpgt_epi8( _mm256_set1_epi8( 0x005B ), nOne ) );
			
			/// Convert any capitals to lower case with mask
			nOne = _mm256_or_si256( nOne, _mm256_and_si256( upperMask, _mm256_set1_epi8( 0x0020 ) ) );

			// Do the same for string 2 now
			nTwo = _mm256_loadu_si256( &nTwo );
			upperMask = _mm256_and_si256( _mm256_cmpgt_epi8( nTwo, _mm256_set1_epi8( 0x0040 ) ),
			_mm256_cmpgt_epi8( _mm256_set1_epi8( 0x005B ), nTwo ) );
			nTwo = _mm256_or_si256( nTwo, _mm256_and_si256( upperMask, _mm256_set1_epi8( 0x0020 ) ) );

			// Compare if both are equal( same name )
			upperMask = _mm256_cmpeq_epi8( nOne, nTwo );

			// If not all significant bits are set
			auto eqMask = _mm256_movemask_epi8( upperMask );
			if ( eqMask != UINT_MAX )
			{
				// We get the less significant bit set or the first letter thats different
				auto firstPos = _tzcnt_u32( ~eqMask );
				return nOne.m256i_i8[ firstPos ] < nTwo.m256i_i8[ firstPos ] ? -1 : 1;
			} else
			{
				// If they are the same name we return 0
				return 0;
			}

		} else
		{
			/*
			* This is the same process as above except its adapted to use UTF-16
			* Only difference is we use the i16 variant of the vector instructions
			* We also have a check in place to make sure the wchar_t is using UTF-16
			*/
			if ( sizeof( wchar_t ) != 2UL )
			{
				throw std::runtime_error( "wchar_t is not currently supporting UTF-16." );
			}

			__movsw( nOne.m256i_u16, ( ( unsigned short* ) nameOne.c_str() ), nameOne.length() );
			__movsw( nTwo.m256i_u16, ( ( unsigned short* ) nameTwo.c_str() ), nameTwo.length() );
			
			 
			nOne = _mm256_loadu_si256( &nOne );
			
			auto upperMask = _mm256_and_si256(
				_mm256_cmpgt_epi16( nOne, _mm256_set1_epi16( 0x0040 ) ),
				_mm256_cmpgt_epi16( _mm256_set1_epi16( 0x005B ), nOne ) );
			
			nOne = _mm256_or_si256( nOne, _mm256_and_si256( upperMask, _mm256_set1_epi16( 0x0020 ) ) );

			nTwo = _mm256_loadu_si256( &nTwo );
			upperMask = _mm256_and_si256( _mm256_cmpgt_epi16( nTwo, _mm256_set1_epi16( 0x0040 ) ),
										_mm256_cmpgt_epi16( _mm256_set1_epi16( 0x005B ), nTwo ) );
			nTwo = _mm256_or_si256( nTwo, _mm256_and_si256( upperMask, _mm256_set1_epi16( 0x0020 ) ) );

			upperMask = _mm256_cmpeq_epi16( nOne, nTwo );

			auto eqMask = _mm256_movemask_epi8( upperMask );
			if ( eqMask != UINT_MAX )
			{
				// Its being shifted right to compensate for  _mm256_movemask_epi16 not existing
				auto firstPos = _tzcnt_u32( ~eqMask ) >> 1;
				return nOne.m256i_i16[ firstPos ] < nTwo.m256i_i16[ firstPos ] ? -1 : 1;
			} else
			{
				return 0;
			}
		}
	}

	/**
	* @brief This function gets lower case version of strings,
	* and compares them for alphabetical order.
	*
	*
	* @param nameOne The first name to compare.
	* @param nameTwo The second name to compare.
	* @return int
	* - Negative number if nameOne comes before nameTwo.
	* - Positive number if nameTwo comes before nameOne.
	* - 0 if both names are the same.
	*
	* @exception std::exception Throws exception in case wchar_t isnt using UTF-16.
	*/
	int CompNames( const T& nameOne, const T& nameTwo )	const
	{
		if ( sizeof( wchar_t ) != 2UL )
		{
			throw std::runtime_error( "wchar_t is not currently supporting UTF-16." );
		}
		auto szNOne = nameOne.length();
		auto szNTwo = nameTwo.length();
		short nL1 = '*';
		short nL2 = '*';

		for ( std::size_t i = 0; ( szNOne >= szNTwo ) ? i < szNOne : i < szNTwo; ++i )
		{
			// If the current letter is a upper case
			// We assign the lower case letter
			// Else we just assign the letter
			if ( i < szNOne && nameOne[ i ] < 0x005B )
			{
				nL1 = nameOne[ i ] ^ 0x0020;
			} else if ( i < szNOne )
			{
				nL1 = nameOne[ i ];
			}

			if ( i < szNTwo && nameTwo[ i ] < 0x005B )
			{
				nL2 = nameTwo[ i ] ^ 0x0020;
			} else if ( i < szNTwo )
			{
				nL2 = nameTwo[ i ];
			}

			// Check for letter difference
			if ( nL1 < nL2 )
			{
				return -1;
			} else if ( nL2 < nL1 )
			{
				return 1;
			}
		}
		return 0;
	}


	StringNode<T>* GetLastEntry()
	{
		StringNode<T>* entry = this->head;

		while ( entry != nullptr )
		{
			entry = entry->flink;
		}
		return entry;
	}


	/**
	* @brief Gets the middle of our linked list
	*
	* Also sets szList param to length of list
	*
	* @param[out] szList Will be set to the length of the list
	* @return The middle StringNode of list, or a string with error if unsuccessful
	*/
	std::expected< StringNode<T>*, std::string> GetListMiddle( std::optional<std::size_t>& szList ) const
	{
		// Set current to the first node in the list
		StringNode<T>* pHigh = this->head->flink;
		StringNode<T>* pLow = this->head->flink;
		// Set count to zero
		std::size_t	count = 0;

		// Move the high pointer twice as fast
		// This naturally leaves the low pointer at the middle by the end
		while ( pHigh != this->head && pHigh != nullptr && pLow != nullptr )
		{
			// Increment the count by 2 since we are moving high pointer twice each iteration
			count += 2uz;
			pHigh = pHigh->flink->flink;
			pLow = pLow->flink;
		}
		
		// Set pHigh to nullptr for memory safety
		pHigh = nullptr;
		
		// If the user supplied the szList variable
		// Assign the value
		if ( szList.has_value() )							  
		{
			szList = { count };
		}
		
		if ( pLow != nullptr )
		{
			return std::expected< StringNode<T>*, std::string>{ pLow };
		} else
		{
			return std::expected< StringNode<T>*, std::string>{ std::unexpected{ "Failed to find Middle of list" } };
		}
		return pLow;
	}


private:
	/**
	* @brief Creates all entries for the linked list
	*
	* This uses random name generator to initialize
	* the list so we can later sort it
	*/
	void CreateAllEntries()
	{
		for ( int i = 0; i < allNames.size(); ++i )
		{
			this->AddEntry( GetRandomName() );
		}
	}

	/**
	* @brief Initializes all our names, our generator,
	* and uniform distribution for getting random names
	*/
	void Init()
	{
		gen = std::mt19937( rd() );
		if constexpr ( std::is_same_v< T, std::string> )
		{
			allNames =
			{
				"Emma", "Liam", "Olivia", "Noah", "Ava", "Elijah",
				"Sophia", "Lucas", "Isabel", "Mason", "Mia",
				"Logan", "Charles", "Ethan", "Amelia", "James",
				"Harper", "Alex", "Evelyn", "Benji", "Abigail",
				"William", "Emily", "Michael", "Eliza", "Daniel"
			};
		} else
		{
			allNames =
			{
				L"Emma", L"Liam", L"Olivia", L"Noah", L"Ava", L"Elijah",
				L"Sophia", L"Lucas", L"Isabel", L"Mason", L"Mia",
				L"Logan", L"Charles", L"Ethan", L"Amelia", L"James",
				L"Harper", L"Alex", L"Evelyn", L"Benji", L"Abigail",
				L"William", L"Emily", L"Michael", L"Eliza", L"Daniel"
			};
		}		
		rdNameDist = std::uniform_int_distribution<std::size_t>( 0, allNames.size() - 1 );
		iSupportFlag = CPUFeatures::CheckRequiredFeatures();
	}

	/**
	* @brief Gets random name from allNames vector
	*
	* @return std::string or std::wstring of name depending on template input
	*/
	T GetRandomName()
	{
		std::shuffle( allNames.begin(), allNames.end(), gen );
		return allNames.at( rdNameDist( gen ) );		
	}
};
