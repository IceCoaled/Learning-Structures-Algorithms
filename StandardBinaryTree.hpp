#include "BinaryTreeBase.hpp"
#include <generator>
#include <queue>
#include <exception>



enum class TreeTraversal : byte
{
    breadth4Search = 0x0032,
    depth4Search = 0x0020,
    preOrderSearch = 0x0064,
    postOrderSearch = 0x00FF,
    inOrderSearch = 0x0000,
};

    /**
    * THIS IS CURRENTLY INCOMPLETE.
    * DO NOT USE
    */

    
/**
* @class StandardBinaryTree
* @brief Implements a binary search tree using FileData objects
*
* StandardBinaryTree creates a binary search tree from randomly generated
* FileData objects. The tree is sorted lexicographically by file names, with duplicate
* path keys removed. The class handles memory management for all FileData objects
* and provides visualization capabilities through PrintTree.
* 
* @notes Though technically this is a building a balanced tree, u dont have that in the 
* details because its not necessarily balanced depending on the names from the file paths.
* So if you built the binary tree on a more randomized selection of the paths, then it most 
* definetly wouldnt be balanced.
*/
class StandardBinaryTree
{
    /** Maximum size of the binary tree, inherited from UnqFileGen */
    static constexpr std::size_t SZ_MAX_TREE = UnqFileGen::SZ_MAX_ARRAY;

    /**
    * Vector containing all FileData pointers in sorted order
    * This ensures proper cleanup of memory on destruction.
    * This was added as when i did the linked list stuff i had 
    * A ton of pointers left dangling after a bit of learning.
    * This is much easier than restarting the PC to clear memory.
    */
    std::vector<FileData*> sortedFileData;

    // Pointer to the root node of the binary tree
    FileData* treeRoot = nullptr;

    // High resolution timer class to time the search functions
    HighResTimer timer;

public:
    /**
    * @brief Constructor that initializes, test's and print's the binary tree.
    *
    * Generates FileData objects, sorts them, removes duplicates,
    * builds a balanced binary tree, and prints the resulting structure.
    */
    StandardBinaryTree()
    {
        InitTree();
        TestSearches();
        PrintTree( this->treeRoot, std::strong_ordering::equal );
    }

    /**
    * @brief Destructor that properly cleans up all allocated FileData objects
    */
    ~StandardBinaryTree()
    {        
        // Recursivly calls delete on
        // Every child pointers
        delete this->treeRoot;
        // Just for safety
        this->treeRoot = nullptr;
        // Clear left over pointers
        this->sortedFileData.clear();
    }

    /**
    * @brief Searches for a file in the binary tree using the specified traversal method.
    *
    * @details This function searches the binary tree for a FileData node that matches
    * either the provided path key or name key. The search uses the specified tree traversal
    * algorithm (breadth-first, depth-first, in-order, pre-order, or post-order).
    * The search is performed by generating a sequence of nodes using the appropriate
    * traversal method and checking each node for a key match.
    *
    * @param tMethod The traversal method to use for searching the tree (from TreeTraversal enum).
    * @param pathKey The hash key of the path to search for. Default is 0 (no path search).
    * @param nameKey The hash key of the name to search for. Default is 0 (no name search).
    *
    * @return std::tuple<FileData*, std::uint16_t> Pointer to the found node, 
    * Or nullptr if no matching node is found. We also return the number of FileData
    * Objects parsed to get the final result.
    *
    * @note If both pathKey and nameKey are provided, the function will search using nameKey first.
    * @note At least one key (pathKey or nameKey) must be non-zero, otherwise the function returns nullptr.
    * @note The search performance is measured using an internal timer.
    *
    * @see TreeTraversal
    * @see BreadthFirstTraversal
    * @see InOrderTraversal
    * @see PreOrderTraversal
    * @see PostOrderTraversal
    */
    std::tuple<FileData*, std::uint16_t> FindFileData( const TreeTraversal& tMethod, const std::size_t& pathKey = 0, const std::size_t& nameKey = 0 )
    {
        // Edge case check
        if ( pathKey == 0 && nameKey == 0 )
        {
            return { static_cast< FileData* >( nullptr ), 0 };
        }
        
        // We consolidate the search key to just 1
        // Favouring the name key as this is how its sorted
        // For this
        std::size_t sKey = ( nameKey != 0 ) ? nameKey : pathKey;
        bool nKey = pathKey != 0;
        std::uint16_t numOfParsed = 0;

       
        // Define a search function that applies to any traversal result
        auto searchNodes = [&]( auto&& nodeGenerator )
            {
                this->timer.Start();
                for ( auto node : nodeGenerator )
                {
                    ++numOfParsed;
                    if ( nKey && node->CompareNameKeys( sKey ) )
                    {
                        return node;
                    } else if ( node->ComparePathKeys( sKey ) )
                    {
                        return node;
                    }
                }
                return static_cast< FileData* >( nullptr );
            };
        
        // Call the appropriate traversal method directly
        switch ( std::byte( tMethod ) )
        {
            case std::byte( TreeTraversal::breadth4Search ):
                return { searchNodes( BreadthFirstTraversal( this->treeRoot ) ), numOfParsed };
            case std::byte( TreeTraversal::depth4Search):
            case std::byte( TreeTraversal::inOrderSearch):
                return { searchNodes( InOrderTraversal( this->treeRoot ) ), numOfParsed };
            case std::byte( TreeTraversal::preOrderSearch):
                return { searchNodes( PreOrderTraversal( this->treeRoot ) ), numOfParsed };
            case std::byte( TreeTraversal::postOrderSearch):
                return { searchNodes( PostOrderTraversal( this->treeRoot ) ), numOfParsed };
            default:
                break;
        }

        return { static_cast< FileData* >( nullptr ), numOfParsed };
    }
   
private:
    /**
    * @brief Initializes the binary tree with sorted FileData objects
    *
    * @throws std::runtime_error If memory allocation fails for any FileData object
    */
    constexpr void InitTree()
    {
        // Create a vector to hold the unsorted FileData pointers
        std::vector<FileData*> unSorted;
        unSorted.reserve( SZ_MAX_TREE );

        // Create all FileData objects and add them to the vector
        for ( std::size_t i = 0; i < SZ_MAX_TREE; ++i )
        {
            
            auto entry = new FileData( UnqFileGen::filepaths[ i ] );
            if ( entry == nullptr )
            {
                // Throw an exception if memory allocation fails
                throw std::runtime_error( "Failed to Create FileData entry.\n" );
            } else
            {
                unSorted.emplace_back( entry );
            }
        }

        std::ranges::sort( unSorted, []( FileData* a, FileData* b )
                           {
                               return a->CompareNames( b ) ==
                                   std::strong_ordering::less;
                           } );

        // Transfer ownership of the sorted vector and clear the original
        this->sortedFileData = std::exchange( unSorted, {} );

        // Build the binary tree from the sorted data
        this->treeRoot = BuildSubTrees( 0, (( std::int32_t )this->sortedFileData.size() - 1 ) );
    }

    /**
    * @brief Recursively builds binary subtrees from the sorted array,
    * Gets middle array object. Then Recursively builds the left and 
    * right side of root. then returning the root to connect each 
    * Sub tree together.
    *
    * @param start Index of the first element in the current subtree
    * @param end Index of the last element in the current subtree
    * @return FileData* Root node of the constructed subtree
    */
    constexpr FileData* BuildSubTrees( std::int32_t start, std::int32_t end )
    {
        // Base case: invalid range
        if ( start > end )
        {
            return nullptr;
        }

        // Find the middle element to use as the subtree root
        const auto mid = std::midpoint<std::int32_t>( start, end );
        FileData* root = this->sortedFileData[ mid ];

        // Recursively build left subtree and establish bidirectional relationships
        if ( FileData* builtLeft = BuildSubTrees( start, mid - 1 ); builtLeft != nullptr )
        {
            // Set the parent of left child to current root
            builtLeft->AccessNode<&FileData::parent>( std::make_optional( root ) );

            // Set the left child of current root
            root->AccessNode<&FileData::leftChild>( std::make_optional( builtLeft ) );
        }

        // Recursively build right subtree and establish bidirectional relationships
        if ( FileData* builtRight = BuildSubTrees( mid + 1, end ); builtRight != nullptr )
        {
            // Set the parent of right child to current root
            builtRight->AccessNode<&FileData::parent>( std::make_optional( root ) );

            // Set the right child of current root
            root->AccessNode<&FileData::rightChild>( std::make_optional( builtRight ) );
        }

        return root;
    }

   
    
    /**
    * @brief Prints a visual representation of the binary tree with clear left/right indicators
    *
    * Generates an ASCII representation of the tree structure using branch characters.
    * The tree is printed with the root at the top and branches extending downward.
    * Left branches are marked with "L├── " and right branches with "R└── " for clarity.
    *
    * @param node Current node to print (starts with root)
    * @param lftRitRot strong ordering to detect left, right, root nodes
    * @param prefix Boolean indicating if the current node is a left child
    * 
    *  @detail std::strong_ordering::less = left, std::strong_ordering::greater = right
    * std::strong_ordering::equal = root.
    */
    constexpr void PrintTree( FileData* node, const std::strong_ordering lftRitRot, const std::string& prefix = "") const
    {
        // Base case: empty node
        if ( node == nullptr ) return;

        // Print the current prefix (indentation and branch structure)
        std::print( "{}", prefix );

        // Print left/right indicator with branch character
        if ( lftRitRot == std::strong_ordering::less )
        {
            std::print( "L├── " );
        } else if ( lftRitRot == std::strong_ordering::greater )
        {
            std::print( "R└── ");
        }

        // Print the node's file name
        std::println( "{}", node->GetFileName() );

        // Calculate new prefixes for children
        // Using 5 spaces to account for the "L" and "R" markers
        std::string leftChildPrefix;
        std::string rightChildPrefix;
        if ( lftRitRot == std::strong_ordering::less )
        {
            rightChildPrefix = leftChildPrefix = prefix + "│    ";
        } else if ( lftRitRot == std::strong_ordering::greater )
        {
            rightChildPrefix = leftChildPrefix = prefix + "     ";
        }

        // Recursively print left subtree with updated prefix
        PrintTree( node->AccessNode<&FileData::leftChild>().value_or( nullptr ),
                   std::strong_ordering::less, leftChildPrefix );

         // Recursively print right subtree with updated prefix
        PrintTree( node->AccessNode<&FileData::rightChild>().value_or( nullptr ), std::strong_ordering::greater, rightChildPrefix );
    }


    ///--------------Traversal-Methods------------------///


    /**
    * @brief Implements pre-order traversal of the binary tree using C++20 coroutines
    *
    * @details Performs a pre-order traversal (root, left, right) of the binary tree starting
    * from the provided node. The traversal is implemented as a generator coroutine which yields
    * nodes in pre-order sequence.
    *
    * @param nInput Pointer to the starting node for traversal
    * @return std::generator<FileData*> A generator that yields FileData pointers in pre-order sequence
    *
    * @note Returns immediately if the input node is null
    * @note Uses C++20 coroutine features with co_yield for lazy evaluation
    * @note Safe with null child pointers through optional value handling
    *
    * @see InOrderTraversal
    * @see PostOrderTraversal
    * @see BreadthFirstTraversal
    */
    std::generator<FileData*> PreOrderTraversal( FileData* nInput ) const
    {
        if ( !nInput ) co_return;

        co_yield nInput;

        if ( auto left = nInput->AccessNode<&FileData::leftChild>(); left.has_value() )
        {
            for ( auto node : PreOrderTraversal( left.value_or( nullptr ) ) )
            {
                co_yield node;
            }
        }

        if ( auto right = nInput->AccessNode<&FileData::rightChild>(); right.has_value() )
        {
            for ( auto node : PreOrderTraversal( right.value_or( nullptr ) ) )
            {
                co_yield node;
            }
        }
    }

    /**
    * @brief Implements in-order traversal of the binary tree using C++20 coroutines
    *
    * @details Performs an in-order traversal (left, root, right) of the binary tree starting
    * from the provided node. The traversal is implemented as a generator coroutine which yields
    * nodes in in-order sequence.
    *
    * @param nInput Pointer to the starting node for traversal
    * @return std::generator<FileData*> A generator that yields FileData pointers in in-order sequence
    *
    * @note In-order traversal of a binary search tree produces nodes in sorted order
    *
    * @see PreOrderTraversal
    * @see PostOrderTraversal
    * @see BreadthFirstTraversal
    */
    std::generator<FileData*> InOrderTraversal( FileData* nInput ) const
    {
        if ( !nInput ) co_return;

        if ( auto left = nInput->AccessNode<&FileData::leftChild>(); left.has_value() )
        {
            for ( auto node : InOrderTraversal( left.value_or( nullptr ) ) )
            {
                co_yield node;
            }

        }

        co_yield nInput;

        if ( auto right = nInput->AccessNode<&FileData::rightChild>(); right.has_value() )
        {
            for ( auto node : InOrderTraversal( right.value_or( nullptr ) ) )
            {
                co_yield node;
            }
        }
    }



    /**
    * @brief Implements post-order traversal of the binary tree using C++20 coroutines
    *
    * @details Performs a post-order traversal (left, right, root) of the binary tree starting
    * from the provided node. The traversal is implemented as a generator coroutine which yields
    * nodes in post-order sequence.
    *
    * @param nInput Pointer to the starting node for traversal
    * @return std::generator<FileData*> A generator that yields FileData pointers in post-order sequence
    *
    * @note Post-order traversal is useful for operations like deletion (visiting children before parent)
    *
    * @see PreOrderTraversal
    * @see InOrderTraversal
    * @see BreadthFirstTraversal
    */
    std::generator<FileData*> PostOrderTraversal( FileData* nInput ) const
    {
        if ( !nInput ) co_return;

        if ( auto left = nInput->AccessNode<&FileData::leftChild>(); left.has_value() )
        {
            for ( auto node : PostOrderTraversal( left.value_or( nullptr ) ) )
            {
                co_yield node;
            }

        }

        if ( auto right = nInput->AccessNode<&FileData::rightChild>(); right.has_value() )
        {
            for ( auto node : PostOrderTraversal( right.value_or( nullptr ) ) )
            {
                co_yield node;
            }
        }

        co_yield nInput;
    }

    

    /**
    * @brief Implements breadth-first traversal of the binary tree using C++20 coroutines
    *
    * @details Performs a breadth-first (level-order) traversal of the binary tree starting
    * from the provided node. The traversal is implemented as a generator coroutine which yields
    * nodes level by level from top to bottom. Uses an iterative approach with a queue rather
    * than recursion.
    *
    * @param nInput Pointer to the starting node for traversal
    * @return std::generator<FileData*> A generator that yields FileData pointers in breadth-first order
    *
    * @note Uses std::queue for level management rather than recursion
    *
    * @see PreOrderTraversal
    * @see InOrderTraversal
    * @see PostOrderTraversal
    */
    std::generator<FileData*> BreadthFirstTraversal( FileData* nInput ) const
    {
        if ( !nInput ) co_return;

        // Queue for BFS
        std::queue<FileData*> nodeQueue;
        nodeQueue.push( nInput );

        // Process all nodes in the queue
        while ( !nodeQueue.empty() )
        {
            // Get the next node in the queue
            FileData* current = nodeQueue.front();
            nodeQueue.pop();

            // Yield the current node
            co_yield current;

            // Add left child to queue if exists
            if ( auto left = current->AccessNode<&FileData::leftChild>(); left.has_value() )
            {
                nodeQueue.push( left.value() );
            }

            // Add right child to queue if exists
            if ( auto right = current->AccessNode<&FileData::rightChild>(); right.has_value() )
            {
                nodeQueue.push( right.value() );
            }
        }
    }

    /**
    * @brief Tests all search traversal methods with a random file path
    *
    * @details This function runs a test of all available tree traversal search methods
    * (breadth-first, depth-first, in-order, pre-order, and post-order) using a randomly
    * generated file path.
    * 
    * @note Performance timing is handled internally using the class timer
    * @note This function processes all traversal methods sequentially
    *
    * @see FindFileData
    * @see PrintSearch
    * @see PrintPerformance
    * @see TreeTraversal
    */
    void TestSearches()
    {          
        using enum TreeTraversal;
        for ( const auto tt : {breadth4Search, depth4Search, inOrderSearch, preOrderSearch, postOrderSearch } )
        {
            auto rPath = UnqFileGen::GetRandomFilePath();
            auto [name, fileSize, nameKey, pathKey] = UnqFileGen::ParseFilePath( rPath );
            PrintSearch( tt, rPath );
            auto [searchRes, objsParsed] = FindFileData( tt, pathKey, nameKey );
            this->timer.Stop();
            if ( searchRes != nullptr && searchRes->GetFilePath() == rPath )
            {
                PrintPerformance( objsParsed );
            } else
            {
                std::println( "Failed to Find File Data." );
            }
            
        }       
    }


    /**
    * @brief Prints information about the current search operation
    *
    * @details Outputs formatted information about the traversal method being used
    * and the target file path being searched for. This creates a visual separator
    * and header for each search operation's results in the console output.
    *
    * @param tMethod The traversal method being used (from TreeTraversal enum)
    * @param f2f The file path being searched for (as string_view)
    *
    * @note Output is formatted with separators for readability
    * @note The method converts the TreeTraversal enum value to its string representation
    *
    * @see TreeTraversal
    */
    void PrintSearch( const TreeTraversal& tMethod, std::string_view f2f ) const
    {
        std::println( "=========================================" );
        
        
        // Call the appropriate traversal method directly
        switch ( std::byte( tMethod ) )
        {
            case  std::byte( TreeTraversal::breadth4Search ):
                std::println( "Breadth-For-Search" );
                break;
            case std::byte( TreeTraversal::depth4Search ):
                std::println( "Depth-For-Search" );
                break;
            case std::byte( TreeTraversal::inOrderSearch ):
                std::println( "In-Order-Traversal-Search" );
                break;
            case std::byte( TreeTraversal::preOrderSearch ):
                std::println( "Pre-Order-Traversal-Search" );
                break;
            case std::byte( TreeTraversal::postOrderSearch ):
                std::println( "Post-Order-Traversal-Search" );
                break;
            default:
                break;
        }
        std::println( "File to Find : \r\n{}.", f2f );
    } 
    
    /**
    * @brief Prints performance metrics of the most recent search operation
    *
    * @details Outputs formatted information about the success of a search operation
    * along with its performance metrics( microseconds to complete ).
    * Total FileData objects parsed, and the time per object( total time / objects parsed).
    *
    * @note Timing information is retrieved from the class timer member
    * @note This should only be called after a successful search operation
    * @note Output is formatted with separators for readability
    *
    * @see TestSearches
    */
    void PrintPerformance( const std::uint16_t& objectsParsed ) const
    {
        std::println( "Successfully Found File Data." );
        std::println( "\n" );
        std::println( "<Performance>" );
        auto elapsed = this->timer.GetElapsed();
        std::println( "Search Complete in: {}μs .", elapsed );
        std::println( "Total Objects Parsed: {}, \r\n Time per FileData Parsed: {}μs.", objectsParsed, ( elapsed / objectsParsed ) );
        std::println( "=========================================" );
    }
};


