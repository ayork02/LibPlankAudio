if (SNDFILE_LIBRARIES AND SNDFILE_INCLUDE_DIRS)
  # in cache already
  set(SNDFILE_FOUND TRUE)
else (SNDFILE_LIBRARIES AND SNDFILE_INCLUDE_DIRS)

  if (SNDFILE_FOUND)
    set(SNDFILE_INCLUDE_DIRS
      ${SNDFILE_INCLUDE_DIRS}
    )
  else (SNDFILE_FOUND)
    find_path(SNDFILE_INCLUDE_DIR
      NAMES
        sndfile.h
      PATHS
        /usr/include
        /usr/local/include
        /opt/local/include
        /sw/include
    )
   
    find_library(SNDFILE_LIBRARY
      NAMES
        sndfile
      PATHS
        /usr/lib
        /usr/local/lib
        /opt/local/lib
        /sw/lib
        ${SNDFILE_LIBRARY_DIR}
    )
   
    set(SNDFILE_INCLUDE_DIRS
      ${SNDFILE_INCLUDE_DIR}
    )
    set(SNDFILE_LIBRARIES
      ${SNDFILE_LIBRARY}
    )
   
    set(SNDFILE_LIBRARY_DIRS
      ${SNDFILE_LIBRARY_DIR}
    )
   
    if (SNDFILE_INCLUDE_DIRS AND SNDFILE_LIBRARIES)
       set(SNDFILE_FOUND TRUE)
    endif (SNDFILE_INCLUDE_DIRS AND SNDFILE_LIBRARIES)
   
    if (SNDFILE_FOUND)
      if (NOT SNDFILE_FIND_QUIETLY)
        message(STATUS "Found sndfile: ${SNDFILE_LIBRARIES}")
      endif (NOT SNDFILE_FIND_QUIETLY)
    else (SNDFILE_FOUND)
      if (SNDFILE_FIND_REQUIRED)
        message(FATAL_ERROR "Could not find sndfile")
      endif (SNDFILE_FIND_REQUIRED)
    endif (SNDFILE_FOUND)
  endif (SNDFILE_FOUND)

  # show the SNDFILE_INCLUDE_DIRS and SNDFILE_LIBRARIES variables only in the advanced view
  mark_as_advanced(SNDFILE_INCLUDE_DIRS SNDFILE_LIBRARIES)

endif (SNDFILE_LIBRARIES AND SNDFILE_INCLUDE_DIRS)
