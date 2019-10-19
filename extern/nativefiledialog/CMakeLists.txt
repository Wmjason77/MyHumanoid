# SPDX-License-Identifier: GPL-3.0-or-later

project(nativefiledialog)

set(nfd_dir ${CMAKE_CURRENT_SOURCE_DIR}/nativefiledialog)

if(UNIX)
	find_package(PkgConfig REQUIRED)
	pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
	
	add_library(nativefiledialog STATIC
		${nfd_dir}/src/common.h
		${nfd_dir}/src/nfd_common.c
		${nfd_dir}/src/nfd_common.h
		${nfd_dir}/src/nfd_gtk.c
		${nfd_dir}/src/nfd_zenity.c
		${nfd_dir}/src/simple_exec.h
	)
	target_include_directories(nativefiledialog
	PRIVATE
		${GTK3_INCLUDE_DIRS}
	PUBLIC
		${nfd_dir}/include
	)
	target_link_libraries(nativefiledialog
	PRIVATE
		${GTK3_LIBRARIES}
	)
else()
	add_library(nativefiledialog STATIC
		${nfd_dir}/src/common.h
		${nfd_dir}/src/nfd_common.c
		${nfd_dir}/src/nfd_common.h
		${nfd_dir}/src/nfd_win.cpp
	)
	target_include_directories(nativefiledialog
	PUBLIC
		${nfd_dir}/include
	)
endif()