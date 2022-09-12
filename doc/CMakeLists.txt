project(doc)

# FIXME: this just does the equivalent of 'make' in the doc directory.
# Much more is possible (see Makefile.am).  The external script
# https://gitlab.oblong.com/platform/platform_site_deploy/blob/master/build-doxy.sh
# is currently the main customer.

find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(srcdir "${doc_SOURCE_DIR}")
    foreach(f apps common internal latex platform)
        configure_file(Doxyfile.${f}.in Doxyfile.${f}.gen @ONLY)
    endforeach()
    unset(srcdir)

    # Note: doc/Doxyfile.common.in:OUTPUT_DIRECTORY is set to 'build'
    add_custom_target(docdir
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/build
    )
    add_custom_target(platformdoc
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.platform.gen
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS docdir
    )
    add_custom_target(doc DEPENDS platformdoc)
else()
    message("Doxygen not found.  To build doc, install doxygen, graphviz, and imagemagick.")
endif(DOXYGEN_FOUND)
