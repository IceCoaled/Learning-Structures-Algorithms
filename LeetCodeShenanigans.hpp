#include <print>
#include <vector>
#include <generator>
#include <algorithm>
#include <ranges>
#include <span>

class Solution 
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
};
