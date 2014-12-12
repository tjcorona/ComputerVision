get_filename_component(BUILD_PARENT_DIR ${CMAKE_BINARY_DIR} PATH)
set(CMAKE_INSTALL_PREFIX "${BUILD_PARENT_DIR}" CACHE PATH "Install path prefix, prepended onto install directories." FORCE)

find_package( OpenCV REQUIRED )

set (APPLICATIONS)

set (DISPLAYIMAGE_SOURCE
    DisplayImage.cc
)

list (APPEND APPLICATIONS DisplayImage)
add_executable( DisplayImage ${DISPLAYIMAGE_SOURCE} )
target_link_libraries( DisplayImage ${OpenCV_LIBS} )

set (LINEARSVM_SOURCE
    LinearSVM.cc
)

list (APPEND APPLICATIONS LinearSVM)
add_executable( LinearSVM ${LINEARSVM_SOURCE} )
target_link_libraries( LinearSVM ${OpenCV_LIBS} )

set (NONLINEARSVM_SOURCE
    NonlinearSVM.cc
)

list (APPEND APPLICATIONS NonlinearSVM)
add_executable( NonlinearSVM ${NONLINEARSVM_SOURCE} )
target_link_libraries( NonlinearSVM ${OpenCV_LIBS} )

install (TARGETS ${APPLICATIONS} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
