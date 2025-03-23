#ifndef SORTINGTESTS_HPP
#define SORTINGTESTS_HPP

#include "ClassBase.hpp"

static void XorSwap( int& a, int& b )
{
    /// Short circuit
    if ( a == b ) return;

    /// Swap
    a ^= b;
    b ^= a;
    a ^= b;
}


/*
Test Case for Bubble Sort:
- Input array has repeated elements in reverse order
- This tests:
  1. Handling of duplicate values
  2. Worst case scenario (completely reversed)
  3. Multiple passes needed
*/

static void TestBubbleSort()
{
    std::vector<int> arr = { 5, 5, 4, 4, 3, 3, 2, 2, 1, 1 };
    auto n = ( ( int )arr.size() );

    // Print initial array
    std::cout << "Initial array: ";
    for ( const auto& num : arr )
    {
        std::cout << num << " ";
    }
    std::cout << "\n";

    // Your bubble sort implementation goes here
    // DO NOT copy from Sorting.hpp code!

    // Expected result: {1, 1, 2, 2, 3, 3, 4, 4, 5, 5}

    // Print sorted array
    std::cout << "Sorted array:  ";
    for ( const auto& num : arr )
    {
        std::cout << num << " ";
    }

    std::cout << "\n";

    // Verification
    for ( int i = 0; i < n - 1; i++ )
    {
        assert( arr[ i ] <= arr[ i + 1 ] && "Array is not sorted!" );
    }

    std::cout << "Bubble Sort Test Passed!\n";
}


/*
  Test Case for Selection Sort:
  - Input array has:
  1. Already sorted elements at the end
  2. Duplicates at the start
  3. Smallest element at the end
  This tests:
  1. Unnecessary swaps with duplicates
  2. Finding minimum in partially sorted array
  3. Moving a small element from end to front
*/

static void TestSelectionSort()
{
    std::vector<int> arr = { 4, 4, 4, 7, 3, 9, 1, 2, 2, 2 };
    auto n = ( ( int )arr.size() );

    // Print initial array
    std::cout << "Initial array: ";
    for ( const auto& num : arr )
    {
        std::cout << num << " ";
    }
    std::cout << "\n";

    // Your selection sort implementation goes here
    // DO NOT copy from original code!
   

    // Expected result: {1, 2, 2, 2, 3, 4, 4, 4, 7, 9}

    // Print sorted array
    std::cout << "Sorted array:  ";
    for ( const auto& num : arr )
    {
        std::cout << num << " ";
    }
    std::cout << "\n";

    // Verification
    for ( int i = 0; i < n - 1; i++ )
    {
        assert( arr[ i ] <= arr[ i + 1 ] && "Array is not sorted!" );
    }

    std::cout << "Selection Sort Test Passed!\n";
}


/*
  Test Case for Insertion Sort:
  - Input array has:
  1. Nearly sorted array but with few elements out of place
  2. Mixed duplicates
  3. Largest element near start
  This tests:
  1. Best case scenario (nearly sorted)
  2. Shifting elements right
  3. Handling duplicates in different positions
*/

static void TestInsertionSort()
{
    std::vector<int> arr = { 2, 9, 1, 3, 3, 4, 5, 5, 6, 7 };
    auto n = ( ( int ) arr.size() );

    // Print initial array
    std::cout << "Initial array: ";
    for ( const auto& num : arr )
    {
        std::cout << num << " ";
    }
    std::cout << "\n";

    // Your insertion sort implementation goes here
    // DO NOT copy from original code!
   


    // Expected result: {1, 2, 3, 3, 4, 5, 5, 6, 7, 9}

    // Print sorted array
    std::cout << "Sorted array:  ";
    for ( const auto& num : arr )
    {
        std::cout << num << " ";
    }
    std::cout << "\n";

    // Verification
    for ( int i = 0; i < n - 1; i++ )
    {
        assert( arr[ i ] <= arr[ i + 1 ] && "Array is not sorted!" );
    }

    std::cout << "Insertion Sort Test Passed!\n";
}


/*
  Test Case for Quick Sort:
  - Input array designed to test:
  1. Already sorted segments
  2. All equal elements in a section
  3. Single element segments
  4. Repeated values around potential pivot points
*/

static void TestQuickSort()
{
    std::vector<int> arr = { 5, 5, 5, 8, 1, 2, 3, 3, 7, 5 };             
    auto n = ( ( int )arr.size() );

    std::cout << "Initial array: ";
    for ( const auto& num : arr )
    {
        std::cout << num << " ";
    }
    std::cout << "\n";

    // Your QuickSort implementation goes here
    // You'll need both partition and recursive sort functions
    // DO NOT copy from original code!
   
    // Expected result: {1, 2, 3, 3, 5, 5, 5, 5, 7, 8}

    std::cout << "Sorted array:  ";
    for ( const auto& num : arr )
    {
        std::cout << num << " ";
    }
    std::cout << "\n";

    // Verification
    for ( int i = 0; i < n - 1; i++ )
    {
        assert( arr[ i ] <= arr[ i + 1 ] && "Array is not sorted!" );
    }

    std::cout << "Quick Sort Test Passed!\n";
}




/*
  Test Case for Merge Sort:
  - Input array designed to test:
  1. Already sorted subsequences
  2. Equal elements at merge boundaries
  3. Single elements at the end
  4. Even/odd length subarrays
*/

static void TestMergeSort()
{
    std::vector<int> arr = { 6, 2, 2, 3, 1, 5, 8, 5, 3, 1 };
    auto n = ( ( int )arr.size() );

    std::cout << "Initial array: ";
    for ( const auto& num : arr )
    {
        std::cout << num << " ";
    }
    std::cout << "\n";

    // Your MergeSort implementation goes here
    // You'll need both merge and recursive sort functions
    // DO NOT copy from original code!

    // Expected result: {1, 1, 2, 2, 3, 3, 5, 5, 6, 8}

    std::cout << "Sorted array:  ";
    for ( const auto& num : arr )
    {
        std::cout << num << " ";
    }
    std::cout << "\n";

    // Verification
    for ( int i = 0; i < n - 1; i++ )
    {
        assert( arr[ i ] <= arr[ i + 1 ] && "Array is not sorted!" );
    }

    std::cout << "Merge Sort Test Passed!\n";
}


#endif // !SORTINGTESTS_HPP

