#include "ClassBase.hpp"

/**
* @enum StrParseRes
* @brief Enumeration of string parsing result codes.
*
* This enum provides status codes for the file path parsing operation,
* indicating success or specific failure conditions that occurred during parsing.
*/
enum class StrParseRes
{
	Success,
	InvalidInput,
	NoDriveLetter,
	NoFileName,
	NoFilePath,
	InvalidFileNameOffset,
};



namespace UnqFileGen
{

	// Max size of both file path and file name arrays
	static constexpr const std::size_t SZ_MAX_ARRAY = 26;
	// Max file size; this would be interpreted in kb
	// As this is what you typically see in file properties
	static constexpr const std::size_t MAX_FILE_SIZE = 3000000;

	// Array of 26 unique filepaths with full paths and various drive letters
	static const std::array<std::string_view, SZ_MAX_ARRAY> filepaths = {
		"C:/Program Files/App/data.bin", "C:/Windows/System32/drivers/file.sys",
		"C:/Users/Admin/Documents/report.pdf", "C:/ProgramData/Application/logs/system.log",
		"D:/Projects/Website/index.html", "D:/Downloads/installer.exe",
		"D:/Media/Music/album/track01.mp3", "D:/Backup/2023/January/backup.zip",
		"E:/Games/RPG/saves/character.sav", "E:/Virtual Machines/Linux/disk.img",
		"E:/Photos/Vacation/img0001.jpg", "E:/Videos/Family/birthday.mp4",
		"F:/Work/Presentations/quarterly.pptx", "F:/Documents/Financial/taxes2023.xlsx",
		"F:/Source/Repository/project/main.cpp", "F:/Archives/Old Projects/legacy.tar",
		"G:/Temp/extract/contents.txt", "G:/Books/Technical/programming.epub",
		"G:/Research/Papers/2023/findings.docx", "G:/Database/Backups/db_dump.sql",
		"H:/External/Shared/company_logo.png", "H:/Transfer/Incoming/received_file.dat",
		"H:/Raw Data/Sensors/readings.csv", "H:/Scripts/Automation/daily_task.bat",
		"I:/Recovery/System Image/os_backup.img", "I:/Utilities/Portable Apps/text_editor.exe"
	};

	constexpr std::string_view GetRandomFilePath()
	{
		std::random_device rd;
		auto gen = std::mt19937( rd() );
		auto dist = std::uniform_int_distribution< std::size_t >( 0, SZ_MAX_ARRAY - 1 );
		return filepaths[ dist( gen ) ];
	}


	/**
	* @brief Simple roll right bit manipulation for,
	* our hash algorithim. ive also included roll
	* left just because.
	*
	* @details These are locked to ull for our purpose,
	* but youd be better off using a template with
	* std::numeric_limits stuff.
	*
	* @param value Value to have the roll done
	* @param roll How many bits you want to roll the value
	*
	* @return std::size_t of rolled value
	*/
	constexpr std::size_t RoR( std::size_t value, std::uint32_t roll )
	{
		return ( ( value >> roll ) | ( value << ( MAX_ULL_BITS - roll ) ) );
	}

	constexpr std::size_t RoL( std::size_t value, std::uint32_t roll )
	{
		return ( ( value << roll ) | ( value >> ( MAX_ULL_BITS - roll ) ) );
	}

	/**
	* @brief This is a hash algorithm i created
	* you can find more on it at
	* https://github.com/IceCoaled/UserMode-KernelMode-Asm-Functions/blob/main/CustomHash.asm
	*/
	constexpr std::size_t HashKey( const char* filePathIn )
	{
		std::size_t hValue = 0x030153912FF;
		std::size_t result = 0;

		while ( *filePathIn )
		{
			const char c = *filePathIn;
			{
				result ^= c;
				result *= hValue;
				hValue -= result;
				result = RoR( result, 0x0010 );
				result <<= 0x0006;
			}
			++filePathIn;
		}
		return result;
	}




	constexpr std::size_t GetFileSize( const std::size_t& nameKey, const std::size_t& pathKey )
	{
		return ( nameKey - pathKey ) % MAX_FILE_SIZE;
	}



	constexpr std::tuple<std::string_view, std::size_t, std::size_t, std::size_t> ParseFilePath( const std::string_view swFilePath )
	{
		if ( swFilePath.empty() )
		{
			return {};
		}

		std::string_view swFileName;

		if ( auto fNameOffset = swFilePath.find_last_of( '/' ); fNameOffset < swFilePath.length() )
		{
			swFileName = swFilePath.substr( fNameOffset + 1, ( swFilePath.find_last_of( '.' ) - fNameOffset ) - 1 );
			if ( swFileName.empty() )
			{
				return {};
			}
		} else
		{
			return {};
		}

		// Hash our name and path for keys
		auto nameKey = HashKey( std::string( swFileName ).c_str() );
		auto pathKey = HashKey( std::string( swFilePath ).c_str() );

		return std::tuple<std::string_view, std::size_t, std::size_t, std::size_t>( swFileName, UnqFileGen::GetFileSize( nameKey, pathKey ), nameKey, pathKey );
	}

};






/**
* @struct FileInfo
* @brief Contains detailed information about a file including its name, path, and metadata.
*
* This structure provides storage for file-related information such as the file name,
* path, folder structure, size, and drive letter. It automatically initializes
* this information during construction by parsing a file path.
*
* The structure stores string data as string_views for efficiency and provides
* methods to access and manipulate the stored information.
*/
struct FileInfo
{
	std::string_view fileName;
	std::string_view filePath;
	std::size_t fileSize = 0;
	std::size_t nameKey = 0;
	std::size_t pathKey = 0;

	
	
	/**
	* @brief Default constructor.
	*
	* Initializes a new FileInfo instance with default values for all members.
	* @note This constructor is conditionally noexcept based on its member types.
	*/
	constexpr FileInfo() noexcept = default;


	/**
	* @brief Constructor with file path.
	*
	* Initializes a new FileInfo instance by automatically parsing a generated file path
	* and extracting information such as file name, path, drive letter, and folders.
	* Also computes hash keys for efficient comparison operations.
	* @note This constructor is conditionally noexcept based on InitFileDetails.
	*/
	explicit constexpr FileInfo( const std::string_view& swFilePath )
	{
		std::ignore = InitFileDetails( swFilePath );
	}


	/**
	* @brief Copy constructor.
	*
	* Creates a new FileInfo instance by copying the contents of another instance.
	* This includes copying all string_views, folder pointers, hash keys, and other metadata.
	*
	* @param other The FileInfo instance to copy from.
	* @note This constructor is conditionally noexcept based on copying its member types.
	*/
	constexpr FileInfo( const FileInfo& other ) noexcept = default;


	/**
	* @brief Move assignment operator.
	*
	* Transfers ownership of resources from another FileInfo instance to this one,
	* leaving the source instance in a valid but unspecified state.
	*
	* @param other The FileInfo instance to move from.
	* @return Reference to this FileInfo after assignment.
	* @note This operator is noexcept since it only uses std::exchange which is noexcept.
	*/
	constexpr FileInfo& operator=( FileInfo&& other ) noexcept
	{
		if ( this != &other )
		{
			this->fileName = std::exchange( other.fileName, {} );
			this->filePath = std::exchange( other.filePath, {} );
			this->fileSize = std::exchange( other.fileSize, 0 );
			this->nameKey = std::exchange( other.nameKey, 0 );
			this->pathKey = std::exchange( other.pathKey, 0 );
		}
		return *this;
	}


	/**
	* @brief Copy assignment operator.
	*
	* Copies the contents of another FileInfo instance to this one.
	*
	* @param other The FileInfo instance to copy from.
	* @return Reference to this FileInfo after assignment.
	* @note This operator is conditionally noexcept based on copying its member types.
	*/
	constexpr FileInfo& operator=( const FileInfo& other )
		noexcept( std::is_nothrow_copy_assignable_v<std::string_view> ) = default;
	
	/**
	* @brief Default destructor.
	*
	* Cleans up resources used by the FileInfo instance.
	* Since the class uses string_views that don't own their memory,
	* the destructor has no special cleanup operations.
	*/
	~FileInfo() = default;


	/**
	* @brief Gets the file name as a const char pointer (for rvalue references).
	*
	* @return Const pointer to the file name string.
	*/
	constexpr std::string GetFileName() const
	{
		return std::string( this->fileName );
	}

	/**
	* @brief Gets the file path as a const char pointer (for rvalue references).
	*
	* @return Const pointer to the file path string.
	*/
	constexpr std::string GetFilePath() const
	{
		return std::string( this->filePath );
	}


private:

	/**
	* @brief Initializes file details by parsing a file path.
	*
	* This method parses the file path to extract drive letter, file name,
	* folder structure, and other metadata. It also computes hash keys for
	* the file name and path for efficient comparisons.
	*
	* If parsing succeeds, it also sets the file size by retrieving it
	* from the UnqFileGen utility.
	*/
	StrParseRes InitFileDetails( const std::string_view& swFilePath )
	{
		this->filePath = swFilePath;
		if ( this->filePath.empty() )
		{
			return StrParseRes::NoFilePath;
		}

		if ( auto fNameOffset = this->filePath.find_last_of( '/' ); fNameOffset < this->filePath.length() )
		{
			this->fileName = this->filePath.substr( fNameOffset + 1, ( this->filePath.find_last_of( '.' ) - fNameOffset ) - 1 );
			if ( this->fileName.empty() )
			{
				return StrParseRes::NoFileName;
			}
		} else
		{
			return StrParseRes::InvalidFileNameOffset;
		}

		// Hash our name and path for keys
		this->nameKey = UnqFileGen::HashKey( this->GetFileName().c_str() );
		this->pathKey = UnqFileGen::HashKey( this->GetFilePath().c_str() );
		// Get file size
		this->fileSize = UnqFileGen::GetFileSize( nameKey, pathKey );
		return StrParseRes::Success;
	}
};








/**
* @struct FileData
* @brief A binary tree node containing file information and tree relationships.
*
* This structure represents a node in a binary tree data structure, where each node
* contains detailed file information and maintains relationships with parent and child nodes.
* It is designed for compile-time construction and manipulation, with all methods marked
* as constexpr where possible for compile-time evaluation.
*
* The structure manages memory for its child nodes and properly cleans up resources
* when destroyed. It provides comprehensive methods for accessing file information
* and traversing the tree structure.
*
* @note Memory management is crucial when working with this structure.
* The struct owns its child pointers and is responsible for their deletion.
*
* @warning Not thread-safe. Concurrent access must be externally synchronized.
*/
class FileData
{
private:
	FileInfo details;

public:
	// These need to stay public for our 
	// Template function
	FileData* parent = nullptr;
	FileData* leftChild = nullptr;
	FileData* rightChild = nullptr;


	/**
	* @brief Default constructor.
	*
	* Creates an empty FileData object with all members initialized to their default values.
	* @note This constructor is conditionally noexcept based on whether FileInfo's default
	* constructor can throw exceptions.
	*/
	constexpr FileData() noexcept( std::is_nothrow_default_constructible_v<FileInfo> ) = default;


	/**
	* @brief Constructor with file path.
	*
	* @param swFilePath input for the file data creation
	* Creates a FileData object with file information initialized from the provided path.
	* This instantiates the file info struct.
	* @note This constructor is conditionally noexcept based on whether FileInfo's constructor
	* from string_view can throw exceptions.
	*/
	explicit constexpr FileData( const std::string_view& swFilePath )
		noexcept( std::is_nothrow_constructible_v<FileInfo, const std::string_view&> )
		: details( FileInfo( swFilePath ) )
	{}

	/**
	* @brief Constructs a FileData object with C-style string parameters.
	*
	* @param lCIn Pointer to the left child node (default: nullptr).
	* @param rCIn Pointer to the right child node (default: nullptr).
	* @note This operator is marked noexcept and will not throw exceptions.
	*/
	constexpr FileData( FileData* const lCIn, FileData* const rCIn, FileData* const parentIn ) noexcept : 
		details( FileInfo() ), parent( parentIn ), leftChild( lCIn ), rightChild( rCIn ) {}


	/**
	* @brief Move constructor.
	*
	* Constructs a FileData by taking ownership of the resources from another FileData object.
	*
	* @param otherData The FileData object to move from.
	* @note This constructor is conditionally noexcept based on whether FileInfo's move operations
	* can throw exceptions.
	*/
	constexpr FileData( FileData&& otherData ) noexcept( std::is_nothrow_move_constructible_v<FileInfo> )
	{
		this->details = std::move( otherData.details );
		this->leftChild = std::exchange( otherData.leftChild, nullptr );
		this->rightChild = std::exchange( otherData.rightChild, nullptr );
		this->parent = std::exchange( otherData.parent, nullptr );
	}


	/**
	* @brief Copy constructor.
	*
	* Constructs a FileData by copying the contents of another FileData object.
	*
	* @param otherData The FileData object to copy from.
	* @note This constructor is conditionally noexcept based on whether FileInfo's copy
	* constructor can throw exceptions.
	*/
	constexpr FileData( const FileData& otherData ) noexcept( std::is_nothrow_copy_constructible_v<FileInfo> )
		: details( FileInfo( otherData.details ) ), parent( otherData.parent ), leftChild( otherData.leftChild ), 
		rightChild( otherData.rightChild ){}


	/**
	* @brief Pointer assignment operator.
	*
	* Assigns the content of a FileData pointer to this object.
	*
	* @param otherData Pointer to the FileData object to assign from.
	* @return Pointer to this FileData after assignment.
	* @note This operator is conditionally noexcept based on whether the copy assignment
	* operation can throw exceptions when dereferencing the pointer.
	*/
	constexpr FileData& operator=( const FileData* otherData )
		noexcept( noexcept( *this = *std::declval<const FileData*>() ) )
	{
		if ( otherData != nullptr )
		{
			*this = *otherData;
		}
		return *this;
	}

	/**
	* @brief Copy assignment operator.
	*
	* Assigns the content of another FileData object to this object using copy-and-swap idiom.
	*
	* @param otherData The FileData object to assign from.
	* @return Reference to this FileData after assignment.
	* @note This operator is conditionally noexcept based on whether FileData's copy construction
	* and swap operations can throw exceptions.
	*/
	constexpr FileData& operator=( const FileData& otherData )
		noexcept( std::is_nothrow_copy_constructible_v<FileData> )
	{
		auto temp = FileData( otherData );
		std::swap( *this, temp );
		return *this;
	}

	/**
	* @brief Move assignment operator.
	*
	* Assigns the content of another FileData object to this object using move semantics and swap.
	*
	* @param otherData The FileData object to move-assign from.
	* @return Reference to this FileData after assignment.
	* @note This operator is conditionally noexcept based on whether the swap operation
	* between two FileData objects can throw exceptions.
	*/
	constexpr FileData& operator=( FileData&& otherData )
		noexcept( std::is_nothrow_move_constructible_v<FileData> )
	{
		std::swap( *this, otherData );
		return *this;
	}


	/**
	* @brief Destructor that cleans up child nodes.
	*
	* @details If this gets called it will cause a recursive deletion of all
	* nodes below the node this was called on in the tree.
	*/
	~FileData()
	{
		if ( this->leftChild != nullptr ) delete this->leftChild;
		if ( this->rightChild != nullptr ) delete this->rightChild;
		this->leftChild = nullptr;
		this->rightChild = nullptr;
		this->parent = nullptr;
	}


	/**
	* @brief Compares file names lexicographically with another FileData through pointer
	*
	* This method performs a three-way comparison between this object's file name
	* and the file name of another FileData object accessed through a pointer.
	* The comparison uses the C++20 spaceship operator (<=>) to generate a
	* std::strong_ordering result.
	*
	* @param otherData Pointer to the FileData object to compare with
	* @return std::strong_ordering Result of the three-way comparison:
	* - std::strong_ordering::less if this file name is lexicographically less than other's
	* - std::strong_ordering::equal if file names are equal
	* - std::strong_ordering::greater if this file name is lexicographically greater than other's
	*
	* @note This function assumes otherData is not nullptr and GetFileName() returns a comparable string
	*/
	constexpr auto CompareNames( FileData* otherData ) const
	{
		return this->GetFileName() <=> otherData->GetFileName();
	}

	/**
	* @brief Compares file names lexicographically with another FileData through reference
	*
	* This method performs a three-way comparison between this object's file name
	* and the file name of another FileData object accessed through a reference.
	* The comparison uses the C++20 spaceship operator (<=>) to generate a
	* std::strong_ordering result.
	*
	* @param otherData Reference to the FileData object to compare with
	* @return std::strong_ordering Result of the three-way comparison:
	* - std::strong_ordering::less if this file name is lexicographically less than other's
	* - std::strong_ordering::equal if file names are equal
	* - std::strong_ordering::greater if this file name is lexicographically greater than other's
	*/
	constexpr auto CompareNames( FileData& otherData ) const
	{
		return this->GetFileName() <=> otherData.GetFileName();
	}

	/**
	* @brief Compares this FileData's name key with another key.
	*
	* @param otherKey The key to compare against.
	* @return std::strong_ordering with comparison details.
	*/
	constexpr bool CompareNameKeys( const std::size_t otherKey ) const
	{
		return this->details.nameKey == otherKey;
	}


	/**
	* @brief Compares this FileData's path key with another key.
	*
	* @param otherKey The key to compare against.
	* @return std::strong_ordering with comparison details.
	*/
	constexpr bool ComparePathKeys( const std::size_t otherKey ) const
	{
		return this->details.pathKey == otherKey;
	}

	
	/**
	* @brief Gets the hash key for the file name.
	*
	* @return The hash key value for the file name.
	*/
	constexpr std::size_t GetFileNameKey() const&
	{
		return this->details.nameKey;
	}

	/**
	* @brief Gets the hash key for the file path.
	*
	* @return The hash key value for the file path.
	*/
	constexpr std::size_t GetFilePathKey() const&
	{
		return this->details.pathKey;
	}

	/**
	* @brief Gets the file name.
	*
	* @return std::string.
	*/
	constexpr std::string GetFileName() const
	{
		return std::string( this->details.fileName );
	}


	/**
	* @brief Gets the file path.
	*
	* @return std::string.
	*/
	constexpr std::string GetFilePath() const
	{
		return std::string( this->details.filePath );
	}


	/**
	* @brief Accesses or modifies a FileData node pointer member
	*
	* @tparam nodePtr Pointer-to-member of FileData that refers to another FileData*
	* @tparam Self Type of the caller, constrained by FileDataQual concept
	*
	* @param self The object whose member is being accessed (this-parameter in C++23)
	* @param newNode Optional new value to assign to the member pointer
	*
	* @return std::optional<FileData*> Current value of the node pointer if no new value was provided,
	*         otherwise returns std::nullopt
	*
	* @details This function provides a unified interface for getting or setting node pointers
	*          in a FileData object, regardless of how that object is accessed (by reference,
	*          pointer, or smart pointer). It uses concepts to constrain the types:
	*          - FileDataQual ensures Self can be treated as a FileData
	*          - NodePointerQual ensures nodePtr is a pointer-to-member of FileData type
	*
	* @note The function does NOT use perfect forwarding as it creates a temp object,
	*  this prevents modification of the deduced object. We are also protected against
	* const modifications via static asserts. I might swap this to std::expected. This
	* would be for alternative return that holds data if it was const or it was
	* already nullptr for example. Rather than the static asserts, this is because the 
	* function throw the asserts even when we are trying to only get the node not modify it.
	*
	* @throws No exceptions are thrown by this function
	*/
	template<auto nodePtr, typename Self>
		requires FileDataQual<Self>&&
			NodePointerQual<nodePtr, Self>
	constexpr std::optional<FileData*> AccessNode( this Self&& self,
					std::optional<FileData*> newNode = std::nullopt )
	{
		if ( newNode.has_value() )
		{
			if constexpr ( std::is_pointer_v<std::decay_t<Self>> )
			{
				// Prevent compilation if trying to modify through a const pointer
				static_assert( !std::is_const_v<std::remove_pointer_t<std::remove_reference_t<Self>>>,
							   "AccessNode: Deduced parameter can't be const" );

				if constexpr ( !std::is_const_v<std::remove_pointer_t<std::remove_reference_t<Self>>> )
				{
					// Set node pointer using pointer->*member syntax
					self->*nodePtr = newNode.value_or( nullptr );
				}

			} else
			{
				// Prevent compilation if trying to modify a const object
				static_assert( !std::is_const_v<std::remove_reference_t<Self>>,
							   "AccessNode: Deduced parameter can't be const" );

				if constexpr ( !std::is_const_v<std::remove_reference_t<Self>> )
				{
					// Set node pointer using object.*member syntax
					self.*nodePtr = newNode.value_or( nullptr );
				}
			}
			return std::nullopt;
		} else
		{
			if constexpr ( std::is_pointer_v<std::decay_t<Self>> )
			{
				// Get node pointer through pointer->*member access, wrapping in optional
				return ( self->*nodePtr != nullptr ) ?
					std::make_optional( self->*nodePtr ) : std::nullopt;
			} else
			{
				// Get node pointer through object.*member access, wrapping in optional
				return ( self.*nodePtr != nullptr ) ?
					std::make_optional( self.*nodePtr ) : std::nullopt;
			}
		}
	}

};


/**
* @concept FileDataQual
* @brief Concept that constrains types to FileData or FileData-compatible types
*
* @tparam Self The type being constrained
*
* @details This concept evaluates to true for:
*   1. FileData objects (by value or reference)
*   2. Raw pointers to FileData objects
*   3. Smart pointer types that implement operator->() returning a FileData*
*
* This allows template functions to accept FileData objects regardless of how
* they are passed (directly, through raw pointers, or through smart pointers).
* The concept uses std::decay_t to normalize reference types and remove cv-qualifiers.
*
* @note The implementation uses a disjunction of type traits and a requires expression
* for smart pointers, allowing for maximal flexibility. I also know this constrant is
* completely unnecessary as "deducing this" is self constraning. this is just practice
* for using template constrants.
*/
template<typename Self>
concept FileDataQual = std::is_same_v<FileData, Self> ||
std::is_same_v<FileData, std::decay_t<Self>> ||
( ( std::is_pointer_v<std::decay_t<Self>> || std::is_pointer_v<Self> ) && 
( std::is_same_v<FileData, std::remove_pointer_t<Self>> ||
std::is_same_v<FileData, std::remove_pointer_t<std::decay_t<Self>>> ) ) ||
requires( Self s )
{
	{ s.operator->() } -> std::convertible_to<FileData*>;
};

/**
* @concept NodePointerQual
* @brief Concept that constrains a member pointer to point to a FileData* member
*
* @tparam NodePtr The member pointer being constrained (auto parameter)
* @tparam Self The type on which the member pointer will be used
*
* @details This concept ensures that the provided member pointer (NodePtr) refers to
*          a FileData* member that can be accessed on the Self type. The concept
*          handles both:
*          1. Pointer access syntax (s->*np) for pointer-like Self types
*          2. Object access syntax (s.*np) for object Self types
*
* This allows template functions to safely use pointer-to-member access with
* FileData objects, ensuring type safety at compile time.
*
* @note The implementation uses two separate requires expressions to handle
*       the different access syntaxes, joined by a logical OR.
*/
template<auto NodePtr, typename Self>
concept NodePointerQual = requires( Self s, decltype( NodePtr ) np )
{
	{ s->*np } -> std::convertible_to<FileData*>;
} ||
	requires( Self s, decltype( NodePtr ) np )
{
	{ s.*np } -> std::convertible_to<FileData*>;
};



