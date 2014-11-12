set(SOURCE_FILES
    lca.cpp
    )

add_library( lca_static STATIC ${SOURCE_FILES})
add_library( lca_shared SHARED ${SOURCE_FILES})