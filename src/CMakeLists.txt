set(SOURCE_FILES
    lca.cpp
    )

add_library( lca_static STATIC ${SOURCE_FILES})
add_library( lca_shared SHARED ${SOURCE_FILES})

add_executable( run run.cpp )

target_link_libraries( run ${PYTHON_LIBRARIES} )