#include "ClassBase.hpp"
#include <generator>
#include <thread>
using namespace std::chrono_literals;


/**
* @brief This is a constraint for our
* binary search template, this keeps its
* limited to int type and string; for 
* searching for entry number or name
*/
template < typename T >
concept SearchType =
std::is_integral_v< T > ||
std::is_same_v< T, std::string > ||
std::is_same_v< T, std::wstring >;



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
	T searchName;
	bool listIsFlipped = false;

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


	/**
	* @brief This function runs all your functions on our linked list class,
	* It asks for user input wether to sort the list or when youd like to 
	* print the list.
	* 
	* Do not flip the list then try to use all the functions, quite a few 
	* have not yet been adapted to run properly on the flipped list.
	*/
	constexpr void RunClassFunctions()
	{
		std::println( "Press ( Y ) If You Would Like To Print the Linked List. \r\n Press ( N ) If You Would Like to proceed Without." );
		if ( GetUserInput() == 1 )
		{
			this->PrintAllEntries();
		}
		
		
		std::int32_t namedSort = 1000;
		std::println( "\n\nPress ( Y ) If You Would Like To Sort the List In Alphabetical Order. \r\n Press ( N ) If You Would Like to proceed Without." );
		namedSort = GetUserInput();
		if ( namedSort == 1 )
		{
			// Set the base classes current entry node
			// To nullptr as that is irrelivant once we
			// do anything to the list
			this->current = nullptr;

			// Sort the linked list
			MergeSortEntries();

			std::println( "\n\nPress ( Y ) If You Would Like To Print the Sorted Linked List. \r\n Press ( N ) If You Would Like to proceed Without." );
			if ( GetUserInput() == 1 )
			{
				this->PrintAllEntries();
			}
		}

		// Get random entry number to find
		const int sNum = GetRandomEntryNum();
		
		std::println( "========================================================" );
		if ( namedSort != 1 )
		{
			std::println( "\n\nUsing Binary Search To Find Random Entry Number: {}, \r\n And Linear Search To Find Random Name In List: {}.", sNum, searchName );
		} else
		{
			std::println( "\n\nUsing Binary Search To Find Random Name In List: {}, \r\n And Linear Search To Find Random Entry Number: {}.", searchName, sNum );
		}

		// Remove entry with name search 
		const T rName = RemoveEntry( searchName, namedSort == 1 );
		const auto rNumEt = this->timer.GetElapsed();
		
		// Remove entry with entry number search
		const auto rNum = RemoveEntry( sNum, namedSort != 1 );
		const auto rNameEt = this->timer.GetElapsed();

		std::println( "\n\n========================================================" );
		if ( !rName.empty() && CompNames( searchName, rName ) == 0 )
		{
			
			std::println( "\nSuccessfully Found: {}'s entry, and removed it.", searchName );
			if ( namedSort == 1 )
			{
				std::println( "Total time to Find And Delete via Name With Binary Search: {}.", rNameEt );
			} else
			{
				std::println( "Total time to Find And Delete via Name With Linear Search: {}.", rNameEt );
			}
		} else
		{
			std::println( "\nIf Youre Seeing This An Error Has Occured While Trying To Remove Entry." );
		}
		
		std::println( "\n\n========================================================" );

		if ( sNum == rNum )
		{
			
			std::println( "\nSuccessfully Found Entry Number: {}, and removed it.", sNum );
			if ( namedSort == 1 )
			{
				std::println( "Total time to Find And Delete via Entry Number With Binary Search: {}.", rNumEt );
			} else
			{
				std::println( "Total time to Find And Delete via Entry Number With Linear Search: {}.", rNumEt );
			}
		} else
		{
			std::println( "\nIf Youre Seeing This An Error Has Occured While Trying To Remove Entry." );
		}

		std::println( "\n\n========================================================" );
		std::println( "\nPress ( Y ) If You Would Like To Print the Linked List With Removed Entries. \r\n Press ( N ) If You Would Like to proceed Without." );
		if ( GetUserInput() == 1 )
		{
			this->PrintAllEntries();
		}

		FlipList();

		std::println( "\n\n========================================================" );
		std::println( "\nPress ( Y ) If You Would Like To Print the Flipped Linked List. \r\n Press ( N ) If You Would Like to proceed Without." );
		if ( GetUserInput() == 1 )
		{
			this->PrintAllEntries( );
		}

	}





private:


	///------------------Flip-List-------------------///


	/**
	* @brief Reverses the order of nodes in the linked list
	*
	* This function flips the direction of the linked list by reversing all node connections.
	* It traverses the list once, swapping the forward and backward links of each node.
	* The function handles both wrapped (circular) and unwrapped list configurations.
	* After flipping, the original first node becomes the last and vice versa.
	* Sets the listIsFlipped flag to true to indicate the list has been flipped.
	*
	* @note Requires a properly initialized list with at least 2 nodes
	* @see StringNode
	*/
	void FlipList()
	{
		if ( this->head == nullptr || this->head->flink == nullptr ||
			 this->head->flink->flink == nullptr )
		{
			return;
		}

		// set our classes flipped list flag
		this->listIsFlipped = true;

		// Set initial entry pointers
		StringNode<T>* firstEntry = this->head->flink;
		StringNode<T>* cBlink = nullptr;
		StringNode<T>* current = this->head->flink;
		StringNode<T>* cFlink = nullptr;

		// Loop through each entry flipping the flink and blink
		while ( current != this->head && current != nullptr )
		{
			cFlink = current->flink;
			current->flink = cBlink;
			current->blink = cFlink;

			cBlink = current;
			current = cFlink;
		}

		
		// If our list is wrapped we need to reset the head
		// flink blink to wrap the list, else we only set the
		// heads flink to point to the new blink
		if ( this->isListWrapped )
		{
			this->head->blink = firstEntry;
			this->head->flink = cBlink;
			firstEntry->flink = this->head;
			if ( cBlink != nullptr )
			{
				cBlink->blink = this->head;
			}
			
		} else
		{
			this->head->flink = cBlink;
			firstEntry->flink = nullptr;
		}
	}



	///---------------Remove-Entry-------------------///

	/**
	* @brief Removes an entry from the list by name
	*
	* @param name The name to search for and remove
	* @param binarSearch true if you want binary search,
	* false, for linear search
	* @return int The entry number of the removed node, or -100 if not found
	*/
	T RemoveEntry( T name, const bool binarySearch )
	{
		StringNode<T>* node = nullptr;
		
		if ( binarySearch )
		{
			this->timer.Start();
			node = BinarySearch( name );
			this->timer.Stop();
		} else if ( !binarySearch )
		{
			this->timer.Start();
			node = LinearSearch( name );
			this->timer.Stop();
		}
		
		
		if ( node != nullptr )
		{
			T name = node->name;
			int entryNum = node->entryNum;
			node->blink->flink = node->flink;
			node->flink->blink = node->blink;
			node->blink = node->flink = nullptr;
			delete node;
			return name;
		} else
		{
			return T();
		}
	}
	
	/**
	* @brief Removes an entry from the list by entry number
	*
	* @param name The name to search for and remove
	* @param binarSearch true if you want binary search,
	* false, for linear search
	* @return int The entry number of the removed node, or -100 if not found
	*/
	std::int32_t RemoveEntry( int entryNum, const bool binarySearch )
	{		
		StringNode<T>* node = nullptr;

		if ( binarySearch )
		{
			this->timer.Start();
			node = BinarySearch( entryNum );
			this->timer.Stop();
		} else if ( !binarySearch )
		{
			this->timer.Start();
			node = LinearSearch( entryNum );
			this->timer.Stop();
		}


		if ( node != nullptr )
		{
			std::int32_t entryNum = node->entryNum;
			node->blink->flink = node->flink;
			node->flink->blink = node->blink;
			node->blink = node->flink = nullptr;
			delete node;
			return entryNum;
		} else
		{
			return -100;
		}
	}


	///---------------Linear-Search-------------------///


	/**
	* @brief Performs linear search on linked list
	* 
	* @tparam ST Search type (int for entry number or T for name),
	* the concept is at the top of this document
	* @param search The value to search for
	* @return StringNode<T>* Pointer to the found node, or nullptr if failure
	*/
	template< typename ST >
		requires SearchType< ST >
	constexpr StringNode<T>* LinearSearch( const ST& search )
	{
		if ( this->head == nullptr || this->head->flink == nullptr )
		{
			return nullptr;
		}
		
		StringNode<T>* pNode = this->head->flink;

		// Loop through each entry and compare our search value with the 
		// Entries proper value

		while ( ( pNode != nullptr && pNode->flink != nullptr ) || pNode != this->head )
		{
			if constexpr ( std::is_same_v< ST, int > )
			{
				if ( pNode != nullptr && pNode->entryNum == search )
				{
					return pNode;
				}
			} else if constexpr ( std::is_same_v< ST, T > )
			{
				if ( CompNames( search, pNode->name ) == 0 )
				{
					return pNode;
				}
			}
			
			if ( pNode->flink != nullptr )
			{
				pNode = pNode->flink;
			}
						
		}
	}


	
	///----------------Binary-Search------------------///
	
	/**
	* @brief Performs binary search on the linked list
	*
	* @tparam ST Search type (int for entry number or T for name),
	* the concept is at the top of this document
	* @param search The value to search for
	* @return StringNode<T>* Pointer to the found node, or nullptr if failure
	*/
	template< typename ST >
		requires SearchType< ST >
	constexpr StringNode<T>* BinarySearch( const ST& search )
	{
		if ( this->head == nullptr || this->head->flink == nullptr )
		{
			return nullptr;
		}
		
		StringNode<T>* pMid = nullptr;
		StringNode<T>* pLow = this->head->flink;
		StringNode<T>* pHigh = this->head->blink;

		// Loop our list
		while ( pLow != pHigh )
		{
			// Get Middle entry of our range
			pMid = GetListMiddle( pLow, pHigh );

			// If we are searching for entry number
			if constexpr ( std::is_same_v< ST, int > )
			{
				// Compare our search value for the middle
				// Nodes value, if ours is greater we shift
				// Our range up. If its less then shift down
				if ( pMid->entryNum == search )
				{
					pHigh = pLow = nullptr;
					return pMid;
				} else if ( search > pMid->entryNum )
				{
					pLow = pMid->flink;
					pMid = nullptr;
				} else
				{
					pHigh = pMid->blink;
					pMid = nullptr;
				}
			} 
			// If we are searching for entry name
			else if constexpr ( std::is_same_v< ST, T > )
			{
				// Get your string comparison result
				auto compRes = CompNames( pMid->name, search );
				
				// If they're the same we return
				// Else if the middle entries name falls lower
				// Then we shift our range up, else shift it down
				if ( compRes == 0 )
				{
					pHigh = pLow = nullptr;
					return pMid;
				} else if ( compRes < 0 )
				{
					pLow = pMid->flink;
					pMid = nullptr;
				} else
				{
					pHigh = pMid->blink;
					pMid = nullptr;
				}
			}

			if ( pLow->flink == pHigh )
			{
				break;
			}
		}
		
		pMid = pHigh = pLow = nullptr;
		return nullptr;
	}




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
	* split chunk or nullptr if failure
	*/
	constexpr StringNode<T>* MergeSplit( StringNode<T>* chunk )
	{
		if ( chunk == nullptr || chunk->flink == nullptr || 
			chunk->flink->flink == nullptr )
		{
			return chunk;
		}
		
		// Set current to the first node in the list
		StringNode<T>* pHigh = chunk->flink;
		StringNode<T>* pLow = chunk->flink;

		// Move the high pointer twice as fast
		// This naturally leaves the low pointer at the middle by the end
		while ( pHigh != nullptr && pHigh->flink != nullptr && 
				pHigh->flink->flink != nullptr &&
				pLow != nullptr && pLow->flink != nullptr )
		{			
			pHigh = pHigh->flink->flink;
			pLow = pLow->flink;

			if ( pHigh->flink == nullptr )
			{
				break;
			} else if ( pHigh->flink->flink == nullptr )
			{
				if ( pLow->flink != nullptr )
				{
					pLow = pLow->flink;
				}
				break;
			}
		}

		StringNode<T>* midNode = pLow->flink;
		if ( midNode != nullptr )
		{
			// Detach the left side of the list 
			// From the right; we always return 
			// The right side
			pLow->flink = nullptr;
			midNode->blink = nullptr;
		}
		return midNode;
	}


	/**
	* @brief Sorts a single chunk of a linked list by comparing nodes from both ends
	*
	* @param chunk The input chunk to be sorted
	* @return StringNode<T>* Pointer to the head of the sorted chunk or nullptr if failure
	*/
	constexpr StringNode<T>* ChunkSort( StringNode<T>* chunk )
	{
		// Return if empty or single node
		if ( chunk == nullptr || chunk->flink == nullptr )
		{
			return chunk;
		}

		StringNode<T> dummyHead = StringNode<T>(); //< Dummy node as anchor
		StringNode<T>* tail = &dummyHead; //< Tracks last node in result
		StringNode<T>* lastEntry = GetLastEntry( chunk ); //< Get end of chunk

		// Loop the chunk till the head and end meet
		while ( chunk != nullptr && lastEntry != nullptr && chunk != lastEntry )
		{
			auto compRes = CompNames( chunk->name, lastEntry->name );

			if ( compRes < 0 || ( compRes == 0 && chunk->entryNum < lastEntry->entryNum ) )
			{
				StringNode<T>* swap = chunk->flink;	 //< Save next node

				tail->flink = chunk; //< Add to result list
				chunk->blink = tail;
				chunk->flink = nullptr; //< Detach from source 
				tail = chunk;  //< Update tail

				chunk = swap; //< Move to next
			} else
			{
				StringNode<T>* swap = lastEntry->blink;

				tail->flink = lastEntry;
				lastEntry->blink = tail;  
				lastEntry->flink = nullptr;  
				tail = lastEntry;

				lastEntry = swap;
			}
		}

		
		// Handle remaining node if both pointers meet
		if ( chunk != nullptr && chunk == lastEntry )
		{
			tail->flink = chunk;
			chunk->blink = tail;
			chunk->flink = nullptr;
		} 
		// Handle remaining nodes from front
		else if ( chunk != nullptr )
		{
			tail->flink = chunk;
			chunk->blink = tail;
		} 
		// Handle remaining nodes from back
		else if ( lastEntry != nullptr )
		{
			tail->flink = lastEntry;
			lastEntry->blink = tail;  
		}

		// Detach the blink from our result
		if ( dummyHead.flink != nullptr )
		{
			dummyHead.flink->blink = nullptr;
		}

		return dummyHead.flink;
	}


	/**
	* @brief Merges two sorted chunks into a single sorted chunk
	*
	* @param chunkOne First sorted chunk
	* @param chunkTwo Second sorted chunk
	* @return StringNode<T>* Pointer to the head of the merged result or nullptr if failure
	*/
	constexpr StringNode<T>* ChunkMergeSort( StringNode<T>* chunkOne, StringNode<T>* chunkTwo )
	{
		// Handle empty chunks
		if ( chunkOne == nullptr )
		{
			return chunkTwo;
		} else if ( chunkTwo == nullptr )
		{
			return chunkOne;
		} 
		
		StringNode<T> dummyHead = StringNode<T>(); //< Dummy node as anchor
		StringNode<T>* tail = &dummyHead; //< Tracks last node in result

		// Loop through both chunks
		while ( chunkOne != nullptr && chunkTwo != nullptr )
		{
			auto compRes = CompNames( chunkOne->name, chunkTwo->name );

			if ( compRes < 0 || ( compRes == 0 && chunkOne->entryNum < chunkTwo->entryNum ) )
			{
				StringNode<T>* swap = chunkOne->flink; //< Save next node

				tail->flink = chunkOne;	 //< Add to result list
				chunkOne->blink = tail;	//< Update tail
				tail = chunkOne;

				chunkOne = swap;  //< Move to next
			} else
			{
				StringNode<T>* swap = chunkTwo->flink;

				tail->flink = chunkTwo;
				chunkTwo->blink = tail;
				tail = chunkTwo;

				chunkTwo = swap;
			}
			
		}
		
		// Attach remaining nodes
		if ( chunkOne != nullptr )
		{
			tail->flink = chunkOne;
			chunkOne->blink = tail;
		} else if ( chunkTwo != nullptr )
		{
			tail->flink = chunkTwo;
			chunkTwo->blink = tail;
		}
		// Detach the blink from our result
		dummyHead.flink->blink = nullptr;

		return dummyHead.flink;
	}


	/**
	* @brief Uses merge sort algorithm to sort a linked list,
	* we sort by name. Although you could make a faster function
	* to sort by entry number
	*
	* @param firstEntry The head of the chunk
	* @return StringNode<T>* Pointer to the head of the sorted list
	* or nullptr if failure
	*/
	constexpr StringNode<T>* MergeSort( StringNode<T>* firstEntry )
	{
		if ( firstEntry == nullptr || firstEntry->flink == nullptr ||
			firstEntry->flink->flink == nullptr )
		{
			return firstEntry;
		}

		StringNode<T>* secondChunk = MergeSplit( firstEntry );
		firstEntry = MergeSort( firstEntry );
		secondChunk = MergeSort( secondChunk );			
			
		return ChunkMergeSort( ChunkSort( firstEntry ), ChunkSort( secondChunk ) );
	}


	/**
	* @Brief This Is the abstract function from the base class.
	*  This is the main function we call to sort the list
	* 
	* @details This re-wraps the list if needed then 
	* prints the list so we can see the final result
	*/
	constexpr void MergeSortEntries()
	{
		std::println( "==============Merge Sort, Linked List================" );
		this->timer.Start();
		if ( this->head->flink != nullptr && this->head->flink->flink != nullptr )
		{
			StringNode<T>* firstEntry = this->head->flink;
			// Detach the head first
			this->head->blink->flink = nullptr;
			this->head->flink = nullptr;
			this->head->flink = MergeSort( firstEntry );
		}
		this->timer.Stop();
		auto et = this->timer.GetElapsed();


		if ( this->isListWrapped )
		{
			this->head->blink = GetLastEntry( this->head );
			this->head->blink->flink = this->head;
		}

		std::println( "==============<Performance>================" );
		std::println( "Number Of Entries: {}, \r\n Total Time: {}us,\n\r Time Per Entry: {}us", this->numOfEntries, et, et / this->numOfEntries );
		std::println( "===========================================" );
	}

	///-----------------Utils----------------------///
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
	*/
	constexpr int CompNames( const T& nameOne, const T& nameTwo ) const
	{
		const std::size_t szL1 = nameOne.length();
		const std::size_t szL2 = nameTwo.length();
		uint32_t nL1 = '*';
		uint32_t nL2 = '*';

		for ( std::size_t i = 0; ( szL1 >= szL2 ) ? i < szL1 : i < szL2; ++i )
		{
			// If the current letter is a upper case
			// We assign the lower case letter
			// Else we just assign the letter
			if ( i < szL1 && ( ( nameOne[ i ] < 0x005B ) && ( nameOne[ i ] > 0x0040 ) ) )
			{
				nL1 = nameOne[ i ] ^ 0x0020;
			} else if ( i < szL1 )
			{
				nL1 = nameOne[ i ];
			}

			if ( i < szL1 && ( ( nameTwo[ i ] < 0x005B ) && ( nameTwo[ i ] > 0x0040 ) ) )
			{
				nL2 = nameTwo[ i ] ^ 0x0020;
			} else if ( i < szL2 )
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
			} else
			{
				nL1 = '*';
				nL2 = '*';
			}
		}
		return 0;
	}


private:

	/**
	* @brief This function gets the last entry
	* in the list or chunk. This function must be used
	* on chunks or list that dont wrap or your get trapped
	* in infinite loop
	* 
	* @param list The first entry in the chunk or list
	*/
	constexpr StringNode<T>* GetLastEntry( StringNode<T>* list )
	{
		StringNode<T>* entry = list;

		while ( entry->flink != nullptr )
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
	* @return The middle StringNode of list/chunk, or nullptr
	*/
	constexpr StringNode<T>* GetListMiddle( StringNode<T>* firstEntry, StringNode<T>* lastEntry ) const
	{
		if ( firstEntry == nullptr || firstEntry->flink == nullptr ||
			 firstEntry->flink->flink == nullptr )
		{
			return firstEntry;
		}

		// Set current to the first node in the list
		StringNode<T>* pHigh = firstEntry;
		StringNode<T>* pLow = firstEntry;

		// Move the high pointer twice as fast
		// This naturally leaves the low pointer at the middle by the end
		while ( pHigh != nullptr && pHigh->flink != nullptr && 
				pHigh->flink->flink != nullptr && pLow != nullptr &&
				pLow->flink != nullptr )
		{
			pHigh = pHigh->flink->flink;
			pLow = pLow->flink;

			if ( pHigh == lastEntry || pHigh->flink == lastEntry )
			{
				break;
			} else if ( pHigh->flink->flink == lastEntry )
			{
				if ( pLow->flink != nullptr )
				{
					pLow = pLow->flink;
				}
				break;
			}
		}
		
		// Set pHigh to nullptr for memory safety
		pHigh = nullptr;
		
		if ( pLow != nullptr )
		{
			return pLow ;
		} else
		{
			return nullptr;
		}
	}


	/**
	* @brief Creates all entries for the linked list
	*
	* This uses random name generator to initialize
	* the list so we can later sort it
	*/
	constexpr void CreateAllEntries()
	{
		const int rEntryNum = GetRandomEntryNum();
		for ( const T& name : GetRandomNameAll() )
		{
			this->AddEntry( name );
			if ( this->current->entryNum == rEntryNum )
			{
				this->searchName = this->current->name;
			}
		}
	}

	/**
	* @brief Initializes all our names, our generator,
	* and uniform distribution for getting random names
	*/
	constexpr void Init()
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
	}

	/**
	* @brief Gets random name from allNames vector
	*
	* @return std::string or std::wstring of name depending on template input
	*/
	constexpr T GetRandomName()
	{
		std::shuffle( allNames.begin(), allNames.end(), gen );
		return allNames.at( rdNameDist( gen ) );		
	}

	std::generator<T> GetRandomNameAll()
	{
		for( std::size_t i = 0; i < allNames.size(); ++i )
		{
			std::shuffle( allNames.begin(), allNames.end(), gen );
			co_yield allNames.at( rdNameDist( gen ) );
		}
	}

	/**
	* @brief Gets random list entry number
	* 
	* @return int of random entry number
	*/
	constexpr int GetRandomEntryNum()
	{
		return	static_cast< int >( rdNameDist( gen ) );
	}


	/**
	* @brief This function comparies a string that was used
	* to capture user input to look for Y, y / N, n	key 
	* presses
	* 
	* @param input The user input string to check
	* @return -200 if the string is empty,
	* -100 if the string length is over 1,
	* -1 if the user pressed N or n, 
	* 1 if the user pressed Y or y
	*/
	constexpr std::int16_t ReadUserInput(std::string& input )
	{
		constexpr std::uint32_t N_KEY = 0x004E;
		constexpr std::uint32_t n_KEY = 0x006E;
		constexpr std::uint32_t Y_KEY = 0x0059;
		constexpr std::uint32_t y_KEY = 0x0079;
		
		if ( input.empty() )
		{
			return -200;
		}

		if ( input.length() > 1 )
		{
			return -100;
		}

	
		if ( input[ 0 ] == N_KEY || input[ 0 ] == n_KEY )
		{
			return -1;
		} else if ( input[ 0 ] == Y_KEY || input[ 0 ] == y_KEY )
		{
			return 1;
		}
		
	}


	/**
	* @brief This function gets user input
	* via stdin pipeline. You must press enter
	* to capture the input.
	* 
	* @return -1 if N or n was pressed,
	* 1 if Y or y was pressed
	*/
	constexpr std::uint32_t GetUserInput()
	{
		std::string input = "";
		
		while ( true )
		{
			std::cin >> input;
			while ( input.empty() )
			{
				std::this_thread::sleep_for( 20ms );
			}

			const auto uIResult = ReadUserInput( input );
			
			if ( uIResult == 1 )
			{
				return 1;
			} else if ( uIResult == -1 )
			{
				return -1;
			} else 
			{
				std::print( "                                                                                                       \r" );
				std::print( "Must Press ( Y ) or ( N )" );
				std::this_thread::sleep_for( 5000ms );
				std::print( "                                                                                                       \r" );
				input.clear();
				std::fflush( stdin );
			}
		}
	}
};
