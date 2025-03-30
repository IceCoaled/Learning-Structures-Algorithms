

#include <vector>
#include <bitset>
#include <array>
#include <string>
#include <cstring>
#include <iostream>

// Define preprocessor check for compiler availability of intrinsics
#if defined(_MSC_VER)
    // Microsoft compiler
#include <intrin.h>
#define HAVE_CPUID_INTRINSICS
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
    // GCC, Clang, etc. on x86/x64
#include <cpuid.h>
#define HAVE_CPUID_INTRINSICS
#endif


/**
* @brief This class is for us to check cpuid bits 
* for AVX2 and BMI1 support for intrinsic 
* vector / bitminipulation functions.
* I have compiler checks in place but this is backup.
* 
* @details both amd and intel use the same registers 
* and bits for both these checks
*/
class CPUFeatures
{
public:
    /**
    * @brief Checks if AVX2 and BMI1 are supported
    * @return true if both are else false
    */
    bool CheckRequiredFeatures()
    {
        bool hasAVX2 = CPUFeatures::HasAVX2();
        bool hasBMI1 = CPUFeatures::HasBMI1();

        std::cout << "AVX2 supported: " << ( hasAVX2 ? "Yes" : "No" ) << std::endl;
        std::cout << "BMI1 supported: " << ( hasBMI1 ? "Yes" : "No" ) << std::endl;

        return hasAVX2 && hasBMI1;
    
    }
private:
    
    /**
    * @brief Checks if system supports AVX2.
    *
    * @return true if system supports AVX2 else false.
    */
    
    static bool HasAVX2()
    {
    #ifdef HAVE_CPUID_INTRINSICS
        std::array<int, 4> cpuInfo = { -1, -1, -1, -1 };

        // Check maximum supported function
        GetCPUID( cpuInfo.data(), 0 );
        int maxFunc = cpuInfo[ 0 ];

        if ( maxFunc >= 7 )
        {
            // EAX=7, ECX=0 gets us extended features including AVX2
            GetCPUID( cpuInfo.data(), 7, 0 );

            // AVX2 is bit 5 of EBX
            return ( cpuInfo[ 1 ] & ( 1 << 5 ) ) != 0;
        }
    #endif
        return false;
    }
    

    /**
    * @brief Checks if system supports BMI1.
    * 
    * @return true if system supports BMI1 else false.
    */
    
    static bool HasBMI1()
    {
    #ifdef HAVE_CPUID_INTRINSICS
        std::array<int, 4> cpuInfo = { -1, -1, -1, -1 };

        // Check maximum supported function
        GetCPUID( cpuInfo.data(), 0 );
        int maxFunc = cpuInfo[ 0 ];

        if ( maxFunc >= 7 )
        {
            // EAX=7, ECX=0 gets us extended features including BMI1
            GetCPUID( cpuInfo.data(), 7, 0 );

            // BMI1 is bit 3 of EBX
            return ( cpuInfo[ 1 ] & ( 1 << 3 ) ) != 0;
        }
    #endif
        return false;
    }
    
    
    /**
    * @brief basic cpuid wrapper for cross platform support
    */
    
    static void GetCPUID(int* info, int func, int subfunc = 0)
    {
    #if defined(_MSC_VER)
        __cpuidex( info, func, subfunc );
    #elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
        __cpuid_count( func, subfunc, info[ 0 ], info[ 1 ], info[ 2 ], info[ 3 ] );
    #else
            // Initialize to zeros
        std::memset( info, 0, sizeof( int ) * 4 );
    #endif
    } 
    
};
 



