file(GLOB_RECURSE SOURCEFILES *.cpp *.h *.ui)
file(GLOB_RECURSE STATICFILES *.usd *.usd* *.txt, *.qrc)
list(FILTER STATICFILES EXCLUDE REGEX "CMakeLists.txt")

usd_executable(mini-maya-executable

    CPPFILES
        ${SOURCEFILES}
        
    LIBRARIES
        usd
        usdGeom
        usdLux
        usdImaging
        usdAppUtils
        gf
        glf
        sdf
        tf
    
    STATICFILES
        ${STATICFILES}
)
