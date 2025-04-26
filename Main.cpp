
//#include "Sorting.hpp"
//#include "Searching.hpp"
//#include "LinkedList.hpp"
#include "StdBinaryTree.hpp"



int main()
{	
	// Set console to use UTF-8
	// This is because certain symbols i was needing
	// the console wasnt allowing
#ifdef _WIN32
	// Windows-specific: set console code page to UTF-8
	SetConsoleOutputCP( CP_UTF8 );
#else
	// Unix/Linux/Mac
	// Save the old locale as it could be modified,
	// This way we can reset it once we are done
	auto oldLoc = std::setlocale( LC_ALL, nullptr );
	std::setlocale( LC_ALL, "en_US.UTF-8" );
#define LOC
#endif

	
	/// Our sorting algorithms class	
	//auto sortAlgoS = std::make_unique< SortingAlgorithms< int > >();
	//sortAlgoS->TestAllAlgorithms();


	/// Our searching algorithms class
	//auto searchAlgoS = std::make_unique< SearchAlogrithms< int > >();
	//searchAlgoS->TestAllSearchAlgorithms();

	/// Our linked list algorithmns class	
	//auto linkedListAlgos = std::make_unique< LinkedListAlgorithms< std::string > >( true );
	//linkedListAlgos->RunClassFunctions();

	// Our standard binary tree algorithms class
	//auto tester = std::make_unique<StandardBinaryTree>();


	system( "pause" );

	// Resetting the code page/ locale
#ifdef LOC
	std::setlocale( LC_ALL, oldLoc );
#endif
    return 0;
}	
