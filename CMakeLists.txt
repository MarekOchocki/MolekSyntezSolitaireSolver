cmake_minimum_required (VERSION 3.12)
project(molekSyntezSolitaireSolver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS 1)

IF(CMAKE_BUILD_TYPE MATCHES Debug)
	set(LIBRARYNAME "molekSyntezSolitaireSolver-d")
ELSE()
	set(LIBRARYNAME "molekSyntezSolitaireSolver")
ENDIF()

# target creation start
add_library(${LIBRARYNAME} SHARED
		include/molekSyntezSolitaireSolver/Board.cpp
		include/molekSyntezSolitaireSolver/Card.cpp
		include/molekSyntezSolitaireSolver/CardColumn.cpp
		include/molekSyntezSolitaireSolver/utils.cpp
		include/molekSyntezSolitaireSolver/GameWindow.cpp
		include/molekSyntezSolitaireSolver/Controller.cpp
)

target_include_directories(${LIBRARYNAME} PUBLIC include)
target_include_directories(${LIBRARYNAME} PUBLIC "dependencies/include")
target_link_directories(${LIBRARYNAME} PUBLIC "dependencies/lib")
set_target_properties(${LIBRARYNAME} PROPERTIES LINKER_LANGUAGE CXX)
# target creation end

# cpm libraries linking start (managed by cpm)
IF(CMAKE_BUILD_TYPE MATCHES Debug OR CMAKE_BUILD_TYPE MATCHES DEBUG)

ELSE()

ENDIF()
# cpm libraries linking end

# target installation start
install(DIRECTORY include DESTINATION .)
install(DIRECTORY "dependencies/bin" DESTINATION .)
install(DIRECTORY "res" DESTINATION bin/)
install(TARGETS ${LIBRARYNAME} RUNTIME DESTINATION bin/)
install(TARGETS ${LIBRARYNAME} ARCHIVE DESTINATION lib/)

add_executable(molekSyntezSolitaireSolver_main "main.cpp")
target_link_libraries(molekSyntezSolitaireSolver_main PRIVATE ${LIBRARYNAME})

install(TARGETS molekSyntezSolitaireSolver_main DESTINATION bin/)
# target installation end
