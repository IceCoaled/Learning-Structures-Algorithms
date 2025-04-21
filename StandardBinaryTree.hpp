#include "BinaryTreeBase.hpp"
#include <generator>
#include <functional>

    /**
    * THIS IS CURRENTLY INCOMPLETE.
    * DO NOT USE
    */

    
/**
* @class StandardBinaryTree
* @brief Implements a balanced binary search tree using FileData objects
*
* StandardBinaryTree creates a balanced binary search tree from randomly generated
* FileData objects. The tree is sorted lexicographically by file names, with duplicate
* path keys removed. The class handles memory management for all FileData objects
* and provides visualization capabilities through PrintTree.
*/
class StandardBinaryTree
{
    /** Maximum size of the binary tree, inherited from UnqFileGen */
    static constexpr std::size_t SZ_MAX_TREE = UnqFileGen::SZ_MAX_ARRAY;

    /**
    * Vector containing all FileData pointers in sorted order
    * This ensures proper cleanup of memory on destruction
    */
    std::vector<FileData*> sortedFileData;

    /** Pointer to the root node of the binary tree */
    FileData* treeRoot = nullptr;

public:
    /**
    * @brief Constructor that initializes and prints the binary tree
    *
    * Generates FileData objects, sorts them, removes duplicates,
    * builds a balanced binary tree, and prints the resulting structure.
    */
    StandardBinaryTree()
    {
        InitTree();
        PrintTree( this->treeRoot );
    }

    /**
    * @brief Destructor that properly cleans up all allocated FileData objects
    *
    * Iterates through the sorted vector of FileData pointers and deletes each
    * non-null entry to prevent memory leaks.
    */
    ~StandardBinaryTree()
    {
        for ( auto entry : this->sortedFileData )
        {
            if ( entry != nullptr )
            {
                delete entry;
            }
        }
    }

private:
    /**
    * @brief Initializes the binary tree with sorted FileData objects
    *
    * This method:
    * 1. Generates random FileData objects
    * 2. Sorts them lexicographically by name
    * 3. Removes entries with duplicate path keys
    * 4. Builds a balanced binary tree from the sorted data
    *
    * @throws std::runtime_error If memory allocation fails for any FileData object
    */
    void InitTree()
    {
        // Initialize the random generator for FileData creation
        UnqFileGen::InitRandomGen();

        // Create a vector to hold the unsorted FileData pointers
        std::vector<FileData*> unSorted;
        unSorted.reserve( SZ_MAX_TREE );

        // Create all FileData objects and add them to the vector
        for ( std::size_t i = 0; i < SZ_MAX_TREE; ++i )
        {
            auto entry = new FileData();
            if ( entry == nullptr )
            {
                // Throw an exception if memory allocation fails
                throw std::runtime_error( "Failed to Create FileData entry.\n" );
            } else
            {
                unSorted.emplace_back( entry );
            }
        }

        // Sort the file data objects lexicographically by name
        std::ranges::sort( unSorted, []( FileData* a, FileData* b )
                           {
                               return a->CompareNames( b ) ==
                                   std::strong_ordering::less;
                           } );

                           // Remove duplicate entries based on path keys
        auto dups =
            std::ranges::unique( unSorted, []( FileData* a, FileData* b )
                                 {
                                     return a->ComparePathKeys( b->GetFilePathKey() );
                                 } );

            // Clean up duplicate FileData objects and remove them from vector
        if ( !dups.empty() && dups.end() != unSorted.end() )
        {
            // Delete each duplicate object starting from the end
            for ( auto it = unSorted.end() - 1; it >= dups.end(); --it )
            {
                delete* it;
            }

            // Remove the null pointers from the vector and optimize storage
            unSorted.erase( dups.end(), unSorted.end() );
            unSorted.shrink_to_fit();
        }

        // Transfer ownership of the sorted vector and clear the original
        this->sortedFileData = std::exchange( unSorted, {} );

        // Build the binary tree from the sorted data
        this->treeRoot = BuildSubTrees( 0, this->sortedFileData.size() - 1 );
    }

    /**
    * @brief Recursively builds balanced binary subtrees from the sorted array
    *
    * Creates a balanced binary tree by:
    * 1. Finding the middle element as the subtree root
    * 2. Recursively building left subtree from elements before the middle
    * 3. Recursively building right subtree from elements after the middle
    * 4. Setting parent-child relationships between nodes
    *
    * @param start Index of the first element in the current subtree
    * @param end Index of the last element in the current subtree
    * @return FileData* Root node of the constructed subtree
    */
    FileData* BuildSubTrees( std::size_t start, std::size_t end )
    {
        // Base case: invalid range
        if ( start > end )
        {
            return nullptr;
        }

        // Find the middle element to use as the subtree root
        const auto mid = std::midpoint<std::size_t>( start, end );
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
    * @brief Prints a visual representation of the binary tree
    *
    * Generates an ASCII representation of the tree structure using branch characters.
    * The tree is printed with the root at the top and branches extending downward.
    * Left branches are marked with ├── and right branches with └──.
    *
    * @param node Current node to print (starts with root)
    * @param prefix String prefix for the current line (used for indentation)
    * @param isLeft Boolean indicating if the current node is a left child
    */
    void PrintTree( FileData* node, std::string prefix = "", bool isLeft = false )
    {
        // Base case: empty node
        if ( node == nullptr ) return;

        // Print the current prefix (indentation and branch structure)
        std::cout << prefix;

        // Print appropriate branch character based on left/right position
        std::cout << ( isLeft ? "├── " : "└── " );

        // Print the node's file name
        std::cout << node->GetFileName() << std::endl;

        // Recursively print left subtree with updated prefix
        // Add vertical line for left branches only if this is a left child
        PrintTree( node->AccessNode<&FileData::leftChild>().value_or( nullptr ),
                   prefix + ( isLeft ? "│   " : "    " ),
                   true );

          // Recursively print right subtree with updated prefix
          // No vertical line needed after the last child
        PrintTree( node->AccessNode<&FileData::rightChild>().value_or( nullptr ),
                   prefix + ( isLeft ? "│   " : "    " ),
                   false );
    }
};


