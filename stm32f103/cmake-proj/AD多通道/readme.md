build:
1) cmake -S . -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=cmake/arm-gcc-toolchain.cmake
2) cmake --build build or cd build & ninja or ninja -C build

flash:
0) st-flash needed
1) cmake --build build --target flash or cd build & ninja flash 
2) st-info --probe # show usb stm32 driver
