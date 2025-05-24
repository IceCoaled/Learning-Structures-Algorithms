#include <print>
#include <vector>
#include <generator>
#include <algorithm>
#include <ranges>
#include <span>
#include <unordered_map>


//Definition for singly-linked list.
struct ListNode
{
    int val;
    ListNode* next;
    ListNode(): val( 0 ), next( nullptr ) {}
    explicit ListNode( int x ): val( x ), next( nullptr ) {}
    ListNode( int x, ListNode* next ): val( x ), next( next ) {}
};

class WildcardMatching
{
public:
     
    /**
    * @brief Matches a string against a pattern with wildcard characters - LeetCode
    * problem 44. Wildcard Matching
    *
    * This function determines if a given string matches a pattern that may contain
    * wildcard characters. The pattern supports:
    * - '*': Matches any sequence of characters (zero or more)
    * - '?': Matches any single character
    * - Any other character: Matches only that specific character
    *
    * @param s The input string to be matched
    * @param p The pattern string containing possible wildcard characters
    *
    * @note this ended up being very effective as i focused more, 
    * on the pattern string rather than the actual string. i think this is
    * why you see so many people using nested loops and all kinda of other stuff.
    * @return true if the string matches the pattern, false otherwise
    */
    constexpr bool isMatch( std::string& s, std::string& p ) const
    {
        // our decayed char arrays
        auto sCStr = s.c_str();
        auto pCStr = p.c_str();

        // Current working index 
        // where both string and pattern chars match
        // Or the patterns character is a question mark
        // Wildcard
        std::int_fast64_t matchIdx = ~0x0;
        // Current worrking star index
        std::int_fast64_t starIdx = ~0x0;

        // Loop through our string
        while ( sCStr != std::to_address( s.end() ) )
        {
            // If pattern array isnt done and we have a star
            if ( pCStr != std::to_address( p.end() ) && *pCStr == '*' )
            {
                // Set our star index and 
                // Current matching index
                starIdx = pCStr - &p[ 0 ];
                matchIdx = sCStr - &s[ 0 ];
                // Increment our pattern read pointer
                ++pCStr;
            }
            //  If pattern array isnt done 
            // And we have a question mark or the pattern and string character match
            else if ( pCStr != std::to_address( p.end() ) && *pCStr == '?' || *sCStr == *pCStr )
            {
                // Increment our string and pattern read pointers
                ++sCStr;
                ++pCStr;
            } 
            // If we have encountered a star previously
            else if ( starIdx != ~0x0 )
            {
                // Reset our pattern pointer just after
                // Current working star wildcard
                pCStr = ( &p[ 0 ] + ( starIdx + 1 ) );
                // Increment match index
                ++matchIdx;
                // Set our string read pointer to the
                // Current working matched character index
                sCStr = ( &s[ 0 ] + matchIdx );
            } else
            {
                // If we get here it means
                // Nothing matches or our read
                // pointers have hit the end of their arrays
                return false;
            }
        }
        // Increment our pattern read pointer 
        // Past any left over star wildcards
        while ( *pCStr == '*' )
        {
            ++pCStr;
        }

        // If we made it all the way through the pattern
        // String then we match
        return pCStr == std::to_address( p.end() );
    }



    /**
    * @brief This function multiplies integar values
    * represented in string form  - LeetCode
    * problem 43. Multiply Strings
    *
    * @param num1 string 1 containing string integer value.
    * @param num2 string 2 containing string integer value.
    *
    * @return std::string with resulting value.
    */
    std::string multiply( std::string& num1, std::string& num2 ) const
    {
        // Edge cases
        if ( num1 == "0" || num2 == "0" )
        {
            return "0";
        } else if ( num1.empty() || num2.empty() )
        {
            return "0";
        }

        // Resulting arithmetic vector 
        std::vector<std::uint8_t> result( num1.length() + num2.length(), 0 );

        // Multiply each digit and add to result
        for ( std::ptrdiff_t i = num1.length() - 1; i >= 0; --i )
        {
            for ( std::ptrdiff_t j = num2.length() - 1; j >= 0; --j )
            {
                // Convert ascii digits to actual digits
                // Get product
                std::uint8_t product = ( num1[ i ] ^ 0x0030 ) * ( num2[ j ] ^ 0x0030 );

                // Add to result
                std::uint8_t sum = product + result[ i + j + 1 ];

                result[ i + j ] += sum / 10; //< Carry
                result[ i + j + 1 ] = sum % 10;  //< Current position
            }
        }

        // Convert to string (skipping leading zeros)
        std::string resultStr;
        for ( auto digit : result )
        {
            // Skip leading zeros
            if ( resultStr.empty() && digit == 0 )
            {
                continue;
            }
            // convert back to ascii value
            resultStr.push_back( digit ^ 0x0030 );
        }

        return resultStr.empty() ? "0" : resultStr;
    }
};

class JumpGame2
{
private:
    /**
    * @brief Calculates the minimum number of jumps required
    * to reach the end of an array - LeetCode
    * problem 45. Jump Game II
    *
    * This function implements a greedy algorithm to determine the minimum number of jumps
    * needed to reach the last element of the array from the first element. At each position,
    * it maintains two pointers:
    * - currentMax: The furthest position reachable with the current number of jumps
    * - nextMax: The furthest position reachable with one more jump
    *
    * The algorithm advances through the array, updating these pointers and incrementing
    * the jump counter whenever the current position reaches the boundary of the current jump.
    *
    * @param nums Reference to a vector containing jump distances at each position
    * @return int The minimum number of jumps needed to reach the end,
    * or bitwise NOT of 0 (~0x0) if it's impossible to reach the end
    */
    constexpr int CalcJumps( std::vector<int>& nums ) const
    {
        // Jump counter
        int jumps = 0;

        // Our vector pointers
        // Data pointer
        int* numsPtr = &nums[ 0 ];
        // Vector head address
        int* const numsHead = numsPtr;
        // Last elements address in vector
        int* const numsTail = std::to_address( nums.end() - 1 );

        // Max index we can reach with jumps
        // setting them to numsPtr is same as 0
        int* currentMax = numsPtr;
        // With jumps + 1
        int* nextMax = currentMax;

        while ( CompareAddress( numsPtr, numsTail ) == std::strong_ordering::less )
        {
            // Update the furthest position we can reach
            nextMax = MaxCalc( numsHead, numsTail, nextMax, numsPtr );
            if ( nextMax == std::numeric_limits<int*>::max() )
            {
                ++jumps;
                break;
            }

            // If we've reached the boundary of our current jump
            if ( CompareAddress( currentMax, numsPtr ) == std::strong_ordering::equal )
            {
                // We must make another jump
                ++jumps;
                currentMax = nextMax;

                // If we can already reach the end, exit early
                if ( CompareAddress( currentMax, numsTail ) != std::strong_ordering::less )
                {
                    break;
                }

                if ( CompareAddress( currentMax, numsPtr ) == std::strong_ordering::equal )
                {
                    return ~0x0;
                }

            }
            ++numsPtr;
        }

        return jumps;
    }


    /**
    * @brief Calculates the maximum reachable position based on the current position and jump value
    *
    * This helper function computes the furthest position that can be reached from the current
    * position and compares it with the previous maximum. It uses pointer arithmetic to
    * calculate positions within the array boundaries.
    *
    * @param vecBaseAddr Pointer to the first element of the vector
    * @param vecPeakAddr Pointer to the last element of the vector
    * @param nextMax Current maximum pointer position reachable with jumps+1
    * @param dataPtr Current position pointer in the array
    * @return int* The new maximum reachable position as a pointer,
    * or std::numeric_limits<int*>::max() if the end can be reached
    *
    * @note Returns the maximum of nextMax and the position reachable from dataPtr
    * @note If the calculated position exceeds or reaches the last element, returns the max limit
    * to indicate we can reach the end
    */
    constexpr int* MaxCalc( int* const vecBaseAddr,
                            int* const vecPeakAddr,
                            int* const nextMax,
                            int* const dataPtr ) const
    {
        // calculate next jump address
        // If its greater than or equal to the 
        // Address of the last element of nums
        // We can short circuit the answer
        int* nxtJmp = vecBaseAddr + ( ( dataPtr - vecBaseAddr ) + *dataPtr );
        if ( nxtJmp >= vecPeakAddr )
        {
            return std::numeric_limits<int*>::max();
        }

        // return the greater of the two
        return std::max( nextMax, nxtJmp );
    }


    /**
    * @brief Compares two memory addresses using the spaceship operator
    *
    * This utility function provides a standardized way to compare two memory addresses
    * and returns the result as a std::strong_ordering. It abstracts the address comparison
    * operation to make the main algorithm cleaner and more readable.
    *
    * @param curReadAddr First pointer address to compare
    * @param compAddr Second pointer address to compare
    * @return std::strong_ordering The ordering relationship between the two addresses:
    * - std::strong_ordering::less if curReadAddr < compAddr
    * - std::strong_ordering::equal if curReadAddr == compAddr
    * - std::strong_ordering::greater if curReadAddr > compAddr
    *
    * @note Utilizes the C++20 spaceship operator (<=>)
    */
    constexpr std::strong_ordering CompareAddress( int* const curReadAddr,
                                                   int* const compAddr ) const
    {
        return curReadAddr <=> compAddr;
    }


public:

    /**
    * @brief Public entry point for calculating minimum jumps to reach the end of an array
    *
    * This function serves as the public interface for the jump calculation algorithm.
    * It handles edge cases and delegates the main calculation to the CalcJumps function.
    *
    * @param nums Reference to a vector containing jump distances at each position
    * @return int The minimum number of jumps required to reach the end of the array,
    * or 0 if the array is empty or has only one non-zero element,
    * or bitwise NOT of 0 (~0x0) if it's impossible to reach the end
    *
    * @note Handles special cases:
    * - Empty array: Returns 0
    * - Single element array with non-zero value: Returns 0
    * - Otherwise delegates to CalcJumps for the main algorithm
    */
    constexpr int jump( std::vector<int>& nums ) const
    {
        // Edge case checks
        if ( nums.empty() || nums.size() == 1 && nums[ 0 ] != 0 )
        {
            return 0;
        }
        // Main Function
        return CalcJumps( nums );
    }
};


//#define PERMS_2

class Permutations
{
public:
    /**
    * @brief Generates all permutations of a given vector of integers  - LeetCode
    * problem 46. Permutations
    *
    * This function creates all possible permutations of the input vector by utilizing
    * the GenPermutations generator function. Each permutation is added to a result vector.
    *
    * @param nums Reference to the vector of integers to permute
    * @return std::vector<std::vector<int>> A vector containing all possible permutations
    * of the input vector
    *
    * @note The order of permutations follows the lexicographical ordering produced by
    * std::next_permutation
    *
    * @see GenPermutations
    */
#ifdef PERMS_2
    std::vector<std::vector<int>> permuteUnique( std::vector<int>& nums ) const
    #else
    std::vector<std::vector<int>> permute( std::vector<int>& nums ) const
    #endif
    {
        // Edge case
        if ( nums.empty() )
        {
            return { {} };
        }

        // Initialize our output variable
        auto result = std::vector<std::vector<int>>();

        // Generate all permutations
        for ( auto&& perm : GenPermutations( std::span( nums.begin(), nums.end() ) ) )
        {
            result.push_back( { perm.begin(), perm.end() } );
        }

    #ifdef PERMS_2
                // Remove non unique permutations
        CleanPermutations( result );
    #endif // PERMS_2

        return result;
    }

private:
    /**
    * @brief Generator coroutine that yields all permutations of a vector as spans
    *
    * This function uses C++20 coroutines to efficiently generate all permutations
    * of the input vector without creating copies of the data. Instead, it yields
    * spans that view the permuted elements in-place.
    *
    * @param numsSpan span of nums the vector of integers to permute
    * @return std::generator<std::span<int>> A generator that yields spans representing
    *         each permutation of the input vector
    *
    * @note The order of permutations follows the lexicographical ordering produced by
    *       std::next_permutation
    * @note Requires C++20 or later for std::generator and std::span support
    *
    * @warning The caller must not modify the input vector while iterating through
    *          the generator results
    */
    std::generator<std::span<int>> GenPermutations( std::span<int> numsSpan ) const
    {
        // Sort first, otherwise we miss permutations
        std::ranges::sort( numsSpan );

        // Loop till there is no permutations left
        do
        {
            co_yield  std::span( numsSpan.begin(), numsSpan.end() );
        } while ( std::next_permutation( numsSpan.begin(), numsSpan.end() ) );
        co_return;
    }


#ifdef PERMS_2
    /**
    * @brief Removes duplicate permutations from our resulting permutations
    *
    * This function ensures that the provided vector contains only unique permutations
    * by performing a two-step cleaning process:
    * 1. First removes adjacent duplicate permutations
    * 2. Then sorts all permutations and removes any remaining duplicates
    *
    * @param permutations Reference to a vector of integer vectors representing permutations
    * This vector is modified in-place to contain only unique permutations
    *
    * @note The function modifies the original vector and may change the order of permutations
    * @note After this function completes, permutations will be sorted in lexicographical order
    * @note Time complexity is O(M log M) where M is the number of permutations
    * (dominated by the sorting operation)
    *
    */
    void CleanPermutations( std::vector<std::vector<int>>& permutations ) const
    {
        // Remove non unqiue 
        auto nonUQ = std::ranges::unique( permutations );
        permutations.erase( nonUQ.begin(), nonUQ.end() );

        // Sort and remove the remaining non unqiue chars
        std::ranges::sort( permutations );

        nonUQ = std::ranges::unique( permutations );
        permutations.erase( nonUQ.begin(), nonUQ.end() );
    }
#endif // PERMS_2
};


class RotateImage
{
public:

    /**
    * @brief Rotates a square matrix 90 degrees clockwise in-place,
    * 48. Rotate Image.
    *
    * @param matrix Reference to a square matrix (vector of vectors of integers)
    * that will be modified in-place
    *
    * @see XorSwap() Private helper method used for efficient swapping without temporary variables
    */
    void rotate( std::vector<std::vector<int>>& matrix )
    {
        // Get size of matrix colums, and rows
        auto N = matrix.size();

        // Loop through matrix and do 90deg clockwise rotation
        for ( int i = 0; i < N >> 1; ++i )
        {
            for ( int j = i; j < N - i - 1; ++j )
            {
                // Swap values
                XorSwap( matrix[ i ][ j ], matrix[ N - 1 - j ][ i ] );
                XorSwap( matrix[ N - 1 - j ][ i ], matrix[ N - 1 - i ][ N - 1 - j ] );
                XorSwap( matrix[ N - 1 - i ][ N - 1 - j ], matrix[ j ][ N - 1 - i ] );
            }
        }
    }


private:
    /**
    * @brief Swaps two integers using XOR operations without a temporary variable
    *
    * @param a Reference to the first integer to be swapped
    * @param b Reference to the second integer to be swapped
    */
    __inline void XorSwap( int& a, int& b )
    {
        a ^= b;
        b ^= a;
        a ^= b;
    }
};



/*
* This one is a bit slower as i made a solution that uses 
* the sorted string for our hash key. As well as using a hashmap.
* There is several more performative was to solve this.
*/
class GroupAnagrams
{
public:
    /**
    * @brief Groups anagrams from a vector of strings, 49. Group Anagrams.
    *
    * @param strs Reference to a vector of strings to be grouped
    * @return std::vector<std::vector<std::string>> A vector of vectors where each inner vector
    *         contains a group of anagrams
    */
    constexpr std::vector<std::vector<std::string>> groupAnagrams( std::vector<std::string>& strs )
    {
        // Counter for tracking group indices
        std::ptrdiff_t cnt = 0;
        // Final result container
        std::vector<std::vector<std::string>> result;

        // Map to store hash value -> index in result vector
        std::unordered_map<std::string, std::ptrdiff_t> hash;

        // Process strings in reverse order (preserves relative order of anagrams)
        for ( auto& str : std::views::reverse( strs ) )
        {
            // Sort string 
            auto strVal = SortStr( str );

            // Check if we've seen this hash value before
            if ( auto it = hash.find( strVal ); it != hash.end() )
            {
                // String with same hash exists, add to that group
                result[ it->second ].push_back( str );
            } else
            {
                // Create new result vmap entry
                result.push_back( std::vector<std::string>( 1, str ) );
                hash.try_emplace( strVal, cnt );
                ++cnt;  //< Increment group counter
            }
        }
        return result;
    }

private:

    /**
    * @brief Creates a sorted version of the input string.
    *
    * @param str Reference to the input string to be sorted
    * @return std::string A new string containing all characters from the input
    *         sorted in lexicographical (ascending) order
    */
    constexpr std::string SortStr( std::string str ) const
    {
        std::sort( str.begin(), str.end() );
        return str;
    }

};


class Pow
{
public:

    /**
    * @brief Calculates x raised to the power of n,
    * 50. Pow(x, n)
    * @param x The base value (reference to a double)
    * @param n The exponent value (integer)
    * @return constexpr double The result of x^n
    */
    constexpr double myPow( double x, int n )
    {
        // Handle edge cases
        if ( n == 0 )
        {
            return 1.0;
        }
        if ( x == 0 )
        {
            return 0.0;
        }

        // Handle INT_MIN by breaking it into two steps
        if ( n == INT_MIN )
        {
            x = 1.0 / x;
            return myPow( x * x, -( n >> 1 ) );
        }

        // Convert negative exponent to positive
        if ( n < 0 )
        {
            n = -n;
        }

        // Exponentiation by squaring
        double result = 1.0;
        while ( n > 0 )
        {
            // If n is odd, multiply result by x
            if ( n % 2 == 1 )
            {
                result *= x;
            }
            // Square the base and halve the exponent
            x *= x;
            n >>= 1;
        }
        return result;
    }
};




class MaximumSubarray
{
public:
    /**
    * @brief Finds the maximum sum of any contiguous subarray within the given array
    *
    * @note After a few searches trying to mix an issue i was having i came across
    * Kadane's Algorithm. so this is the revision to change my approach.
    *
    * @param nums Reference to a vector of integers to search for maximum subarray
    * @return constexpr int32_t The maximum sum found in any contiguous subarray
    */
    constexpr int32_t maxSubArray( std::vector<int32_t>& nums ) const
    {
        // Edge cases
        if ( nums.empty() )
        {
            return 0;
        }
        if ( nums.size() == 1 )
        {
            return nums[ 0 ];
        }

        // Our result
        int32_t result = nums[ 0 ];
        // Sub array sum
        int32_t sum = result;

        // Loop through each possible sub array and figure out the largest sum
        for ( size_t i = 1; i < nums.size(); ++i )
        {
            sum = std::max( sum + nums[ i ], nums[ i ] );
            result = std::max( result, sum );
        }
        return result;
    }

private:

};


#include <iostream>
#include <bitset>
#include <cstring> 
#include <print>
#include <vector>
#include <generator>
#include <algorithm>
#include <ranges>
#include <span>
#include <unordered_map>
#include <numeric>
#include <cassert>



class LengthOfLastWord
{
private:

public:
    /**
    * @brief Calculates the length of the last word in a string, 58. Length of Last Word.
    *
    *
    * @param s The input string to analyze
    * @return int32_t The length of the last word in the string
    *                 Returns 0 if no word is found or if the string is empty
    *
    */
    constexpr int32_t lengthOfLastWord( std::string s ) const
    {
        // Our counters
        size_t firstLetterIndex = s.length() - 1;
        size_t lastLetterIndex = firstLetterIndex;
        // flag to introduce short circuit counting
        // counting as in decrementing `firstLetterIndex`
        bool count = false;

        // Check if we underflowed to max uint64 (0xFFFFFFFFFFFFFFFF)
        // This checks for string length of 0
        if ( firstLetterIndex == ~0x0UL ) [[unlikely]]
        {
            return 0;
        } else
        {
            // Loop the string in reverse
            for ( const auto& c : std::views::reverse( s ) )
            {
                // Short circuit decrement / breakout
                // If flag is active and current char is a-z
                if ( count && ( ( c >= 'a' && c <= 'z' ) || ( c >= 'A' && c <= 'Z' ) ) )
                {
                    --firstLetterIndex;
                }
                // If the flag is active and current char isnt a-z breakout
                else if ( count && ( !( c >= 'a' && c <= 'z' ) || !( c >= 'A' && c <= 'Z' ) ) )
                {
                    break; //< done
                }
                // If flag isnt active but the current char is a-z we set our flag and
                // Decrement the counter
                else if ( !count && ( ( c >= 'a' && c <= 'z' ) || ( c >= 'A' && c <= 'Z' ) ) )
                {
                    count = true;
                    --firstLetterIndex;
                } else
                {
                    --firstLetterIndex;
                    --lastLetterIndex;
                }
            }
        }
        // Compiler was complaining about size_t to int32_t, solution
        // Read only the 32 bits we need to int32_t, this isnt recommended
        // Typically but i know its safe here and it shuts the compiler up
        return ( ( lastLetterIndex - firstLetterIndex ) & 0xFFFFFFFF );
    }

};



class RotateList
{

public:

    /**
    * @brief Rotates a linked list to the right by k positions, 61. Rotate List
    *
    *
    * @param head Pointer to the head node of the linked list to rotate
    * @param k Number of positions to rotate the list to the right
    *
    * @return ListNode* Pointer to the new head of the rotated list, or nullptr
    *         if the input list is empty
    *
    * @warning The current overflow calculation logic may produce unexpected results
    *          when k is significantly larger than the list size
    *
    */
    ListNode* rotateRight( ListNode* head, int k )
    {
        // Edge cases
        if ( head == nullptr )
        {
            return nullptr;
        } else if ( head->next == nullptr )
        {
            return head;
        }

        // Our traversal node
        ListNode* traverse = head;
        // Vector to hold list pointers
        std::vector<ListNode*> nodeAddrs = { nullptr };

        // Loop our Nodes and add them to vector
        while ( traverse != nullptr )
        {
            nodeAddrs.push_back( traverse );
            traverse = traverse->next;
        }

        // This is faster than modifying the loop above
        nodeAddrs.erase( nodeAddrs.begin() );

        // Calculate the overflow of rotations
        // If k > szList
        auto szList = nodeAddrs.size();
        k %= szList;

        // Edge case k == szList
        if ( k == 0 )
        {
            return head;
        }

        // Connect the tail to the head
        nodeAddrs.back()->next = head;

        // Disconnect the list where the new tail is
        nodeAddrs[ szList - k - 1 ]->next = nullptr;

        // return the new head
        return nodeAddrs[ szList - k ];
    }
};



class ValidNumber
{

public:
    /**
    * @brief Validates whether a string represents a valid numeric literal using finite state machine parsing, 65. Valid Number
    *
    * This function implements a lexical analyzer similar to those used in production compilers
    * (GCC, Clang, MSVC) to validate numeric literals in real-time. It uses a finite state machine
    * approach to parse various numeric formats including integers, floating-point numbers,
    * scientific notation, hexadecimal literals, and type suffixes.
    * I was digging in and around here on llvm/clang stuff to get some ideas.
    * https://github.com/llvm/llvm-project/blob/main/clang/lib/Lex/LiteralSupport.cpp
    *
    * @param s The string to validate as a numeric literal
    *
    * @return true if the string represents a valid numeric literal, false otherwise
    *
    * @note This function is constexpr and can be evaluated at compile-time for constant expressions.
    *
    * **Supported Formats:**
    * - **Integers**: `42`, `-123`, `+456`
    * - **Floating-point**: `.5`, `3.14`, `-2.718`, `+1.0`
    * - **Scientific notation**: `1e10`, `2.5E-3`, `-1.23e+45`
    * - **Hexadecimal**: `0x1A2B`, `0X3C4D` (when prefixed with integer)
    * - **Type suffixes**:
    * - `f/F` (float): `3.14f`
    * - `l/L` (long): `123L`
    * - `u/U` (unsigned): `456U`
    * - `ll/LL` (long long): `789LL`
    * - `ul/UL`, `uz/UZ` (unsigned combinations): `123UL`
    *
    * **State Machine Transitions:**
    * ```
    * Start → [+/-] → Sign → [0-9] → Integer → [.] → Fraction
    *   ↓       ↓              ↓         ↓         ↓
    *  [.]     [.]            [eE]      [eE]      [eE]
    *   ↓       ↓              ↓         ↓         ↓
    * Percent Percent      Exponent  Exponent  Exponent
    *                          ↓         ↓         ↓
    *                      [+/-|0-9] [+/-|0-9] [+/-|0-9]
    *                          ↓         ↓         ↓
    *                       ExpInt    ExpInt    ExpInt
    *                          ↓         ↓         ↓
    *                      [suffix]  [suffix]  [suffix]
    *                          ↓         ↓         ↓
    *                      SuffixMaj SuffixMaj SuffixMaj
    * ```
    * @detail This function could easily be modified to get
    * proper numeric type check by returning the literal state
    * directly and comparing it to different states. this would
    * require a few changes, but easily implemented. See below
    * for an example of what it could be.
    * @example
    * ```cpp
    * Parser parser;
    * assert(parser.isNumber("123")); -> return // Integer
    * assert(parser.isNumber("-3.14")); ->      // Negative float
    * assert(parser.isNumber("2.5e-3")); ->     // Scientific notation
    * assert(parser.isNumber("0x1A2B")); ->     // Hexadecimal
    * assert(parser.isNumber("123.45f")); ->    // Float with suffix
    * assert(parser.isNumber("789LL")); ->      // Long long suffix
    * assert(!parser.isNumber("12.34.56")); ->  // Invalid: multiple dots
    * assert(!parser.isNumber("abc")); ->       // Invalid: non-numeric
    * ```
    *
    * @warning The function assumes well-formed suffix combinations. Invalid suffix sequences
    * like "123ff" or "456lul" will be rejected.
    *
    * @see LiteralState enum for detailed state definitions
    * @see IsSuffix() helper function for suffix validation
    *
    * @author IceCoaled
    * @date 2025-05-24
    * @version 1.0
    *
    * **Implementation Notes:**
    * - Uses `using enum LiteralState` for cleaner switch statements (C++20 feature)
    * - Tracks last three characters for complex suffix validation (e.g., "LL", "UL")
    * - Single-pass algorithm with O(n) time complexity and O(1) space complexity
    * - Inspired by production compiler lexical analyzers for robust numeric parsing
    */
    constexpr bool isNumber( const std::string& s ) const
    {
        using enum LiteralState;

        // Last chars used to compare edge states
        char lastThree = '\0';
        char lastTwo = '\0';
        char lastChar = '\0';

        // Our literal start point
        LiteralState format = Start;

        // loop throw literals to check if they are
        // Valid numeric's
        for ( const auto& c : s )
        {

            switch ( format )
            {
                case Start:
                    // Check for positive/ negative
                    // Percent
                    // Or just straight digits
                    if ( c == '+' || c == '-' )
                    {
                        format = Sign;
                    } else if ( c == '.' )
                    {
                        format = Percent;
                    } else if ( std::isdigit( c ) )
                    {
                        format = Integer;
                    } else
                    {
                        return false;
                    }
                    break;
                    // Check for digits or percent
                case  Sign:
                    if ( std::isdigit( c ) )
                    {
                        format = Integer;
                    } else if ( c == '.' )
                    {
                        format = Percent;
                    } else
                    {
                        return false;
                    }
                    break;
                    // Check for digits, percent, 
                    // Or `e/E` exponent symbol
                    // Or check if we are a entering
                    // Into a hex value
                case  Integer:
                    if ( std::isdigit( c ) )
                    {
                        format = Integer;
                    } else if ( c == '.' )
                    {
                        format = Percent;
                    } else if ( c == exponentSym || c == capExponentSym )
                    {
                        format = Exponent;
                    } else if ( ( c == xSuffix || c == capXSuffix ) && lastChar == '0' )
                    {
                        format = Hex;
                    } else
                    {
                        return false;
                    }
                    break;
                    // Check if we are still a hex 
                    // Once in the hex check we
                    // Are locked in it Either 
                    // Returns true or false
                case Hex:
                    if ( std::isxdigit( c ) )
                    {
                        format = Hex;
                    } else
                    {
                        return false;
                    }
                    break;
                    // Check for digit to build a fraction( percentage value )
                    // Or check for exponent symbol 
                case  Percent:
                    if ( std::isdigit( c ) )
                    {
                        format = Fraction;
                    } else if ( c == exponentSym || c == capExponentSym )
                    {
                        format = Exponent;
                    } else
                    {
                        return false;
                    }
                    break;
                    // Check for a suffix, or more digits 
                    // For our percent value, or exponent
                    // Symbol
                case  Fraction:
                    if ( IsSuffix( c ) )
                    {
                        format = SuffixMaj;
                    } else if ( std::isdigit( c ) )
                    {
                        format = Fraction;
                    } else if ( c == exponentSym || c == capExponentSym )
                    {
                        format = Exponent;
                    } else
                    {
                        return false;
                    }
                    break;
                    // Check to make sure the user didnt enter
                    // Some weird stuff like `+.e13`, but we 
                    // Do check for a exponent sign positive or
                    // Negative, or the start of our exponent digits
                case  Exponent:
                    if ( !std::isdigit( lastThree ) && lastTwo == '.' )
                    {
                        return false;
                    } else if ( c == '+' || c == '-' )
                    {
                        format = ExpSign;
                    } else if ( std::isdigit( c ) )
                    {
                        format = ExpInt;
                    } else
                    {
                        return false;
                    }
                    break;
                    // If there is a sign for the exponent
                    // It must now be a digit or its invalid
                case  ExpSign:
                    if ( std::isdigit( c ) )
                    {
                        format = ExpInt;
                    } else
                    {
                        return false;
                    }
                    break;
                    // Checking for more exponent digits
                    // Or our suffix
                case  ExpInt:
                    if ( std::isdigit( c ) )
                    {
                        format = ExpInt;
                    } else if ( IsSuffix( c ) )
                    {
                        format = SuffixMaj;
                    } else
                    {
                        return false;
                    }
                    break;
                    // Checking for specific suffixes
                    // See below for valid ones
                    // It covers all the way to cpp23 uz,z,UZ,Z 
                    // Suffix's
                case  SuffixMaj:
                    if ( lastChar == fSuffix || lastChar == capFSuffix )
                    {
                        return false;
                    } else if ( lastChar == lSuffix || lastChar == capLSuffix &&
                                c == lSuffix || c == capLSuffix || c == xSuffix ||
                                c == capXSuffix )
                    {
                        format = SuffixMin;
                    } else if ( lastChar == uSuffix || lastChar == capUSuffix &&
                                c == lSuffix || c == capLSuffix || c == zSuffix ||
                                c == capZSuffix )
                    {
                        format = SuffixMin;
                    } else
                    {
                        return false;
                    }
                    break;
                    // If we are the remaining character in the multi
                    // Character suffix's go to end
                case  SuffixMin:
                    if ( lastChar == lSuffix || lastChar == capLSuffix &&
                         c == xSuffix || c == capXSuffix )
                    {
                        format = End;
                    } else
                    {
                        return false;
                    }
                    break;
                case  End:
                    return false;
                default: return false;
            }

            // These are checks to add in our
            // Comparand last characters
            // This is done like this so
            // We actually get the proper 
            // Characters 
            if ( lastTwo != '\0' )
            {
                lastThree = lastTwo;
            }

            if ( lastChar != '\0' )
            {
                lastTwo = lastChar;
            }
            lastChar = c;
        }

        // This is here to cover a edge case of a leet code check
        // For the f/F suffix not being valid.
#define LEEETCODE
#ifndef LEETCODE
        if ( lastChar == fSuffix || lastChar == capFSuffix )
        {
            return false;
        }
#endif // !LEETCODE


            // Compare against where we know are valid ending states
        return format == End || format == Hex || format == Integer || format == Fraction ||
            format == ExpInt || format == SuffixMaj || format == SuffixMin ||
            format == Percent && std::isdigit( lastTwo );
    }

private:

    /**
    * @brief This function quickly checks if the input char
    * is a suffix char.
    *
    * @param inputC this is the character to be checked
    *
    * @detail See below for all suffix chars
    *
    * @return True if input char is equal to a suffix char
    */
    constexpr bool IsSuffix( const char& inputC ) const
    {
        switch ( inputC )
        {
            case fSuffix:
            case lSuffix:
            case uSuffix:
            case xSuffix:
            case zSuffix:
                return true;
            default:
                return false;
        }
    }


    /**
    * @brief This is our enum for our
    * literal format
    */
    enum class LiteralState: uint16_t
    {
        Start = 0,
        Sign,
        Integer,
        Hex,
        Percent,
        Fraction,
        Exponent,
        ExpSign,
        ExpInt,
        SuffixMaj,
        SuffixMin,
        End,
    };

    /**
    *@details
    * f = float
    * l = long
    * u = unsigned
    * z = ssize_t
    * uz = size_t
    * ll = long long
    * ul = unsigned long
    * ull unsigned long long
    * lx unsigned long
    * lxx unsigned long long
    */
    static constexpr auto exponentSym = 'e';
    static constexpr auto capExponentSym = 'E';
    static constexpr auto fSuffix = 'f';
    static constexpr auto capFSuffix = 'F';
    static constexpr auto lSuffix = 'l';
    static constexpr auto capLSuffix = 'L';
    static constexpr auto uSuffix = 'u';
    static constexpr auto capUSuffix = 'U';
    static constexpr auto xSuffix = 'x';
    static constexpr auto capXSuffix = 'X';
    static constexpr auto zSuffix = 'z';
    static constexpr auto capZSuffix = 'Z';

};
