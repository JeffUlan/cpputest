#!/bin/bash
# Script run in the travis CI

if [ "x$BUILDTOOL" = "xautotools" ]; then 
	../configure || exit 1
	echo "CONFIGURATION DONE. Compiling now."
	make check_all || exit 1
	if [ $CXX = "gcc" ]; then 
		ruby ../scripts/deploy_to_github_pages.rb || exit 1
	fi; 
fi

if [ "x$BUILDTOOL" = "xcmake" ]; then 
	cmake .. -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE || exit 1
	make || exit 1
	ctest -V || exit 1
fi

if [ "x$BUILDTOOL" = "xcmake" ]; then 
	cmake .. -DGMOCK=ON || exit 1
	make || exit 1
	ctest -V || exit 1
fi

if [ "x$BUILDTOOL" = "xcmake" ]; then 
	cmake .. -DGMOCK=OFF -DREAL_GTEST=ON || exit 1
	make || exit 1
	ctest -V || exit 1
fi
