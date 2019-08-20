
set(proj grt)

# Set dependency list
set(${proj}_DEPENDS
  Swig
  )

# Include dependent projects if any
ExternalProject_Include_Dependencies(${proj} PROJECT_VAR proj)

if(${SUPERBUILD_TOPLEVEL_PROJECT}_USE_SYSTEM_${proj})
  message(FATAL_ERROR "Enabling ${SUPERBUILD_TOPLEVEL_PROJECT}_USE_SYSTEM_${proj} is not supported !")
endif()

# Sanity checks
if(DEFINED grt_DIR AND NOT EXISTS ${grt_DIR})
  message(FATAL_ERROR "grt_DIR [${grt_DIR}] variable is defined but corresponds to nonexistent directory")
endif()

if(NOT DEFINED ${proj}_DIR AND NOT ${SUPERBUILD_TOPLEVEL_PROJECT}_USE_SYSTEM_${proj})

  ExternalProject_SetIfNotDefined(
    ${SUPERBUILD_TOPLEVEL_PROJECT}_${proj}_GIT_REPOSITORY
    "${EP_GIT_PROTOCOL}://github.com/VASST/grt.git"
    QUIET
    )

  ExternalProject_SetIfNotDefined(
    ${SUPERBUILD_TOPLEVEL_PROJECT}_${proj}_GIT_TAG
    "master"
    QUIET
    )

  set(EP_SOURCE_DIR ${CMAKE_BINARY_DIR}/${proj})
  set(EP_BINARY_DIR ${CMAKE_BINARY_DIR}/${proj}-build)
  set(EP_INSTALL_DIR ${CMAKE_BINARY_DIR}/${proj}-install)

  find_package(PythonLibs REQUIRED)

  ExternalProject_Add(${proj}
    ${${proj}_EP_ARGS}
    GIT_REPOSITORY "${${SUPERBUILD_TOPLEVEL_PROJECT}_${proj}_GIT_REPOSITORY}"
    GIT_TAG "${${SUPERBUILD_TOPLEVEL_PROJECT}_${proj}_GIT_TAG}"
    SOURCE_DIR ${EP_SOURCE_DIR}
    SOURCE_SUBDIR build
    BINARY_DIR ${EP_BINARY_DIR}
    INSTALL_DIR ${EP_INSTALL_DIR}
    CMAKE_CACHE_ARGS
      # Compiler settings
      -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
      -DCMAKE_C_FLAGS:STRING=${ep_common_c_flags}
      -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
      -DCMAKE_CXX_FLAGS:STRING=${ep_common_cxx_flags}
      -DCMAKE_CXX_STANDARD:STRING=${CMAKE_CXX_STANDARD}
      -DCMAKE_CXX_STANDARD_REQUIRED:BOOL=${CMAKE_CXX_STANDARD_REQUIRED}
      -DCMAKE_CXX_EXTENSIONS:BOOL=${CMAKE_CXX_EXTENSIONS}
      # Output directories
      -DCMAKE_RUNTIME_OUTPUT_DIRECTORY:PATH=${CMAKE_BINARY_DIR}/${Slicer_THIRDPARTY_BIN_DIR}
      -DCMAKE_LIBRARY_OUTPUT_DIRECTORY:PATH=${CMAKE_BINARY_DIR}/${Slicer_THIRDPARTY_LIB_DIR}
      -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY:PATH=${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
      # Install directories
      # XXX The following two variables should be updated to match the
      #     requirements of a real CMake based external project
      # XXX Then, this comment and the one above should be removed. Really.
      -Dgrt_INSTALL_RUNTIME_DIR:STRING=${Slicer_INSTALL_THIRDPARTY_LIB_DIR}
      -Dgrt_INSTALL_LIBRARY_DIR:STRING=${Slicer_INSTALL_THIRDPARTY_LIB_DIR}
      # Output directories for CLIs
      #-DSlicerExecutionModel_DEFAULT_CLI_RUNTIME_OUTPUT_DIRECTORY:PATH=${SlicerExecutionModel_DEFAULT_CLI_RUNTIME_OUTPUT_DIRECTORY}
      #-DSlicerExecutionModel_DEFAULT_CLI_RUNTIME_LIBRARY_DIRECTORY:PATH=${SlicerExecutionModel_DEFAULT_CLI_LIBRARY_OUTPUT_DIRECTORY}
      #-DSlicerExecutionModel_DEFAULT_CLI_RUNTIME_ARCHIVE_DIRECTORY:PATH=${SlicerExecutionModel_DEFAULT_CLI_ARCHIVE_OUTPUT_DIRECTORY}
      # Options
      -DBUILD_TESTING:BOOL=OFF
      -DBUILD_GTEST:BOOL=OFF
      -DBUILD_EXAMPLES:BOOL=OFF
      -DBUILD_TESTS:BOOL=OFF
      -DSWIG_EXECUTABLE:FILEPATH=${SWIG_EXECUTABLE}
      -DPYTHON_EXECUTABLE:FILEPATH=${PYTHON_EXECUTABLE}
      -DPYTHON_LIBRARY:FILEPATH=${PYTHON_LIBRARY}
    DEPENDS
      ${${proj}_DEPENDS}
    )
  set(${proj}_DIR ${EP_BINARY_DIR})

else()
  ExternalProject_Add_Empty(${proj} DEPENDS ${${proj}_DEPENDS})
endif()

mark_as_superbuild(${proj}_DIR:PATH)
