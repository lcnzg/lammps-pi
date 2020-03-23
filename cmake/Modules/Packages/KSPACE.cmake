if(PKG_KSPACE)
  option(FFT_SINGLE "Use single precision FFTs instead of double precision FFTs" OFF)
  set(FFTW "FFTW3")
  if(FFT_SINGLE)
    set(FFTW "FFTW3F")
    add_definitions(-DFFT_SINGLE)
  endif()
  find_package(${FFTW} QUIET)
  if(${FFTW}_FOUND)
    set(FFT "FFTW3" CACHE STRING "FFT library for KSPACE package")
  else()
    set(FFT "KISS" CACHE STRING "FFT library for KSPACE package")
  endif()
  set(FFT_VALUES KISS FFTW3 MKL)
  set_property(CACHE FFT PROPERTY STRINGS ${FFT_VALUES})
  validate_option(FFT FFT_VALUES)
  string(TOUPPER ${FFT} FFT)

  if(FFT STREQUAL "FFTW3")
    find_package(${FFTW} REQUIRED)
    add_definitions(-DFFT_FFTW3)
    target_link_libraries(lammps PUBLIC ${FFTW}::${FFTW})
    if(FFTW3_OMP_LIBRARY OR FFTW3F_OMP_LIBRARY)
      option(FFT_FFTW_THREADS "Use threaded FFTW library" ON)
    else()
      option(FFT_FFTW_THREADS "Use threaded FFT library" OFF)
    endif()

    if(FFT_FFTW_THREADS)
      if(FFTW3_OMP_LIBRARY OR FFTW3F_OMP_LIBRARY)
        add_definitions(-DFFT_FFTW_THREADS)
	target_link_libraries(lammps PRIVATE ${FFTW}::${FFTW}_OMP)
      else()
        message(FATAL_ERROR "Need OpenMP enabled FFTW3 library for FFT_THREADS")
      endif()
    endif()
  elseif(FFT STREQUAL "MKL")
    find_package(MKL REQUIRED)
    add_definitions(-DFFT_MKL)
    option(FFT_MKL_THREADS "Use threaded MKL FFT" ON)
    if(FFT_MKL_THREADS)
      add_definitions(-DFFT_MKL_THREADS)
    endif()
    include_directories(${MKL_INCLUDE_DIRS})
    target_link_libraries(lammps PRIVATE ${MKL_LIBRARIES})
  else()
    # last option is KISSFFT
    add_definitions(-DFFT_KISS)
  endif()

  set(FFT_PACK "array" CACHE STRING "Optimization for FFT")
  set(FFT_PACK_VALUES array pointer memcpy)
  set_property(CACHE FFT_PACK PROPERTY STRINGS ${FFT_PACK_VALUES})
  validate_option(FFT_PACK FFT_PACK_VALUES)
  if(NOT FFT_PACK STREQUAL "array")
    string(TOUPPER ${FFT_PACK} FFT_PACK)
    add_definitions(-DFFT_PACK_${FFT_PACK})
  endif()
endif()
