get_filename_component(BUILD_PARENT_DIR ${CMAKE_BINARY_DIR} PATH)
set(CMAKE_INSTALL_PREFIX "${BUILD_PARENT_DIR}" CACHE PATH "Install path prefix, prepended onto install directories." FORCE)

set (DISPLAYIMAGE_SOURCE
    DisplayImage.cc
)

find_package( OpenCV REQUIRED )
add_executable( DisplayImage ${DISPLAYIMAGE_SOURCE} )
target_link_libraries( DisplayImage ${OpenCV_LIBS} )

install (TARGETS DisplayImage DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
