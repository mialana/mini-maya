if (USD_ROOT)

    if (NOT BOOST_ROOT)
        message(STATUS "BOOST_ROOT not set, defaulting to ${USD_ROOT}")
        set(BOOST_ROOT ${USD_ROOT})
    endif()

    if (NOT TBB_ROOT)
        message(STATUS "TBB_ROOT not set, defaulting to ${USD_ROOT}")
        set(TBB_ROOT ${USD_ROOT})
    endif()

else()
    message(FATAL_ERROR "USD_ROOT must be set to the root dir of the USD installation!")
endif()

# Boost & python.
if (ENABLE_PYTHON_SUPPORT)
    # Find python libraries.
    find_package(
        Python3
        COMPONENTS
            Interpreter Development
        REQUIRED
    )

    # Pick up boost version variables.
    find_package(Boost REQUIRED)

    # We can use Boost_VERSION_STRING in CMake 3.14+.
    set(boost_version_string "${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}.${Boost_SUBMINOR_VERSION}")

    # Boost provided cmake files in 1.70 and above causes inconsistent failures.
    if (${boost_version_string} VERSION_GREATER_EQUAL "1.70")
        option(Boost_NO_BOOST_CMAKE "Disable boost-provided cmake config" ON)
        if (Boost_NO_BOOST_CMAKE)
            message(STATUS "Disabling boost-provided cmake config")
        endif()
    endif()

    find_package(Boost
            REQUIRED
    )
endif()

# USD & TBB
include(${USD_ROOT}/pxrConfig.cmake)
find_package(TBB REQUIRED)

# QT
find_package(Qt6 REQUIRED COMPONENTS Core OpenGLWidgets)
