
#include "Sorting.hpp"
#include "Searching.hpp"
#include "LinkedList.hpp"
#include "IntrinsicCheck.hpp"



int main()
{	
	/// Our sorting algorithms class	
	//auto sortAlgoS = std::make_unique< SortingAlgorithms< int > >();
	//sortAlgoS->TestAllAlgorithms();


	/// Our searching algorithms class
	//auto searchAlgoS = std::make_unique< SearchAlogrithms< int > >();
	//searchAlgoS->TestAllSearchAlgorithms();

	// Our linked list algorithmns class
	auto test = std::make_unique<LinkedListAlgorithms<std::string>>( true );
	auto llS = std::make_unique< LinkedListAlgorithms< std::string > >( true );
	llS->PrintAllEntries();
	auto szList = std::optional<std::size_t>( 0 );
	auto middleNode = std::expected<StringNode<std::string>*, std::string>( llS->GetListMiddle( szList ) );
	if ( middleNode.has_value() )
	{
		std::println( "Middle List Node Name: {}, Entry Number: {}, List Size: {}", middleNode.value()->name, middleNode.value()->entryNum, szList.value());
	} else
	{
		std::println( "ERROR: {}", middleNode.error() );
	}

	//llS->MergeSortEntries();
	
	system( "pause" );
    return 0;
}
