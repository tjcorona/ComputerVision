get_filename_component(BUILD_PARENT_DIR ${CMAKE_BINARY_DIR} PATH)
set(CMAKE_INSTALL_PREFIX "${BUILD_PARENT_DIR}" CACHE PATH "Install path prefix, prepended onto install directories." FORCE)

set( CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMake )

find_package( OpenCV REQUIRED )
find_package( PkgConfig )
pkg_check_modules( TESSERACT REQUIRED tesseract )
pkg_check_modules( LEPTONICA REQUIRED lept )

include_directories( SYSTEM ${Tesseract_INCLUDE_DIR} )

set (APPLICATIONS)

set (DISPLAYIMAGE_SOURCE
    DisplayImage.cc
)

list (APPEND APPLICATIONS DisplayImage)
add_executable( DisplayImage ${DISPLAYIMAGE_SOURCE} )
target_link_libraries( DisplayImage ${OpenCV_LIBS} )

set (FINDCONTOURS_SOURCE
    FindContours.cc
)

list (APPEND APPLICATIONS FindContours)
add_executable( FindContours ${FINDCONTOURS_SOURCE} )
target_link_libraries( FindContours ${OpenCV_LIBS} )

set (CAMERACAPTURE_SOURCE
    CameraCapture.cc
)

list (APPEND APPLICATIONS CameraCapture)
add_executable( CameraCapture ${CAMERACAPTURE_SOURCE} )
target_link_libraries( CameraCapture ${OpenCV_LIBS} )

set (VIDEOCAPTURE_SOURCE
    VideoCapture.cc
)

list (APPEND APPLICATIONS VideoCapture)
add_executable( VideoCapture ${VIDEOCAPTURE_SOURCE} )
target_link_libraries( VideoCapture ${OpenCV_LIBS} )

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


set (SIMPLETESSERACT_SOURCE
    SimpleTesseract.cc
)

list (APPEND APPLICATIONS SimpleTesseract)
add_executable( SimpleTesseract ${SIMPLETESSERACT_SOURCE} )
target_link_libraries( SimpleTesseract ${TESSERACT_LIBRARIES} ${LEPTONICA_LIBRARIES} )

install (TARGETS ${APPLICATIONS} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
