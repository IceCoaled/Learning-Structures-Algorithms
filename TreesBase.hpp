#include "ClassBase.hpp"

namespace UnqFileGen
{

	// Max size of both file path and file name arrays
	static constexpr const std::size_t SZ_MAX_ARRAY = 26;
	// Max file size; this would be interpreted in kb
	// As this is what you typically see in file properties
	static constexpr const std::size_t MAX_FILE_SIZE = 3000000;


	static std::random_device rd;
	static std::mt19937 gen;
	static std::uniform_int_distribution< std::size_t > rdFSizeDist;
	static std::uniform_int_distribution< std::size_t > rdPathDist;
	
	static void InitRandomGen()
	{
		gen = std::mt19937( rd() );
		rdFSizeDist	= std::uniform_int_distribution<std::size_t>( 100, MAX_FILE_SIZE );
		rdPathDist = std::uniform_int_distribution<std::size_t>( 0, SZ_MAX_ARRAY - 1 );
	}

	static std::size_t GetFileSize()
	{
		return rdFSizeDist( gen );
	}
	

	// Generate a unique filepath based on an index (0-25)
	static std::string_view GetFilePath()
	{
		// Array of 26 unique filepaths with full paths and various drive letters
		static std::array<std::string_view, 26> filepaths = {
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
		std::shuffle( filepaths.begin(), filepaths.end(), gen );
		return filepaths[ rdPathDist( gen ) ];
	}
};




/**
* @enum StrParseRes
* @brief Enumeration of string parsing result codes.
*
* This enum provides status codes for the file path parsing operation,
* indicating success or specific failure conditions that occurred during parsing.
*/
enum StrParseRes
{
	Success,
	InvalidInput,
	NoDriveLetter,
	NoFileName,
	NoFilePath,
	InvalidFileNameOffset,
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
	* @brief Constructor.
	*
	* Initializes a new FileInfo instance by automatically parsing a generated file path
	* and extracting information such as file name, path, drive letter, and folders.
	* Also computes hash keys for efficient comparison operations.
	*/
	constexpr FileInfo()
	{
		std::ignore = InitFileDetails();
	}
	
	
	/**
	* @brief Copy constructor.
	*
	* Creates a new FileInfo instance by copying the contents of another instance.
	* This includes copying all string_views, folder pointers, hash keys, and other metadata.
	*
	* @param other The FileInfo instance to copy from.
	*/
	constexpr FileInfo( const FileInfo& other ) = default;

	
	/**
	* @brief Move assignment operator.
	*
	* Transfers ownership of resources from another FileInfo instance to this one,
	* leaving the source instance in a valid but unspecified state.
	*
	* @param other The FileInfo instance to move from.
	* @return Reference to this FileInfo after assignment.
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
	* @note This operator is marked noexcept and will not throw exceptions.
	*/
	StrParseRes InitFileDetails() noexcept
	{
		this->filePath = UnqFileGen::GetFilePath();
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

		this->fileSize = UnqFileGen::GetFileSize();
			// Hash our name and path for keys
		this->nameKey = HashKey( this->GetFileName().c_str() );
		this->pathKey = HashKey( this->GetFilePath().c_str() );

		return StrParseRes::Success;
	}


	/**
	* @brief This is a hash algorithm i created
	* you can find more on it at
	* https://github.com/IceCoaled/UserMode-KernelMode-Asm-Functions/blob/main/CustomHash.asm
	*/
	constexpr std::size_t HashKey( const char* filePathIn ) const
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
	constexpr std::size_t RoR( std::size_t value, std::uint32_t roll ) const
	{
		return ( ( value >> roll ) | ( value << ( MAX_ULL_BITS - roll ) ) );
	}

	constexpr std::size_t RoL( std::size_t value, std::uint32_t roll ) const
	{
		return ( ( value << roll ) | ( value >> ( MAX_ULL_BITS - roll ) ) );
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
	FileData* parent = nullptr;
	FileData* leftChild = nullptr;
	FileData* rightChild = nullptr;




	/**
	* @brief Default constructor.
	*
	* Creates an empty FileData object with all members initialized to their default values.
	*/
	constexpr FileData() = default;

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
	* @note This operator is marked noexcept and will not throw exceptions.
	*/
	constexpr FileData( FileData&& otherData ) noexcept
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
	* @note This operator is marked noexcept and will not throw exceptions.
	*/
	constexpr FileData( const FileData& otherData ) noexcept: details( FileInfo( otherData.details ) ), 
		parent( otherData.parent ), leftChild( otherData.leftChild ), rightChild( otherData.rightChild ){}


	/**
	* @brief Pointer assignment operator.
	*
	* Assigns the content of a FileData pointer to this object.
	*
	* @param otherData Pointer to the FileData object to assign from.
	* @return Pointer to this FileData after assignment.
	* @note This operator is marked noexcept and will not throw exceptions.
	*/
	constexpr FileData& operator= ( const FileData* otherData ) noexcept
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
	* Assigns the content of another FileData object to this object.
	*
	* @param otherData The FileData object to assign from.
	* @return Reference to this FileData after assignment.
	* @note This operator is marked noexcept and will not throw exceptions.
	*/
	constexpr FileData& operator= ( const FileData& otherData ) noexcept
	{
		auto temp = FileData( otherData );
		std::swap( *this, temp );
		return *this;
	}




	/**
	* @brief Move assignment operator.
	*
	* Assigns the content of another FileData object to this object using move semantics.
	*
	* @param otherData The FileData object to move-assign from.
	* @return Reference to this FileData after assignment.
	* @note This operator is marked noexcept and will not throw exceptions.
	*/
	constexpr FileData& operator= ( FileData&& otherData )  noexcept
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
	}


	constexpr auto CompareNames( FileData* otherData ) const
	{
		return this->GetFileName() <=> otherData->GetFileName();
	}

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


	template<auto nodePtr, typename Self>
		requires FileDataQual<Self> &&
			NodePointerQual<nodePtr, Self>
	constexpr std::optional<FileData*> AccessNode( this Self&& self,
				std::optional<FileData*> newNode = std::nullopt ) 
	{
		auto thisObj = std::forward<Self>( self );
			
		if ( newNode.has_value() )
		{			
			if constexpr ( std::is_pointer_v<std::decay_t<Self>> )
			{
				static_assert( !std::is_const_v<std::remove_pointer_t<std::decay_t<Self>>>,
							   "AccessNode: Deduced parameter can't be const" );
				
				if constexpr ( !std::is_const_v<std::remove_pointer_t<std::decay_t<Self>>> )
				{
					thisObj->*nodePtr = newNode.value_or( nullptr );
				}
				
			} else
			{
				static_assert( !std::is_const_v<std::remove_reference_t<Self>>,
							   "AccessNode: Deduced parameter can't be const" );

				if constexpr ( !std::is_const_v<std::remove_reference_t<Self>> )
				{
					thisObj.*nodePtr = newNode.value_or( nullptr );
				}
			} 
			return std::nullopt;
		} else
		{			
			if constexpr ( std::is_pointer_v<std::decay_t<Self>> )
			{
				return ( thisObj->*nodePtr != nullptr ) ? 
					std::make_optional( thisObj->*nodePtr ) : std::nullopt;
			} else
			{
				return ( thisObj.*nodePtr != nullptr ) ? 
					std::make_optional( thisObj.*nodePtr ) : std::nullopt;
			}
		}
	}



	template<auto nodePtr, typename Self>
		requires FileDataQual<Self>	&&
			NodePointerQual<nodePtr, Self>
	constexpr std::optional<FileData*> GetNodeRef( this Self&& self )
	{	
		auto thisObj = std::forward<Self>( self );

		if constexpr ( std::is_pointer_v<std::decay_t<Self>> )
		{
			return ( thisObj->*nodePtr != nullptr ) ? 
				std::make_optional( thisObj->*nodePtr ) : std::nullopt;
		} else
		{
			return ( thisObj.*nodePtr != nullptr ) ? 
				std::make_optional( thisObj.*nodePtr ) : std::nullopt;
		}
	}
};


template<typename Self>
concept FileDataQual = ( std::is_same_v<FileData, std::decay_t<Self>> ||
	std::is_same_v<FileData, std::decay_t<Self>> ) ||
	( std::is_pointer_v<std::decay_t<Self>> &&
	std::is_same_v<FileData, std::remove_pointer_t<std::decay_t<Self>>> ) ||
	requires( Self s )
{
	{ s.operator->() } -> std::convertible_to<FileData*>;
};

template<auto NodePtr, typename Self>
concept NodePointerQual = requires( Self s, decltype( NodePtr ) np )
{
	{ s->*np } -> std::convertible_to<FileData*>;
} ||
	requires( Self s, decltype( NodePtr ) np )
{
	{ s.*np } -> std::convertible_to<FileData*>;
};



