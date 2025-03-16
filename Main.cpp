#include "Sorting.hpp"
//#include "Searching.hxx"


int main()
{	
	auto* sAlgoS = new SortingAlgorithms<int>();

	sAlgoS->TestAllAlgorithms();

	delete sAlgoS;

	system( "pause" );
    return 0;
}