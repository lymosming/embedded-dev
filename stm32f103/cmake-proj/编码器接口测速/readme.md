build:
1) cmake -S .. -B . -G Ninja -DCMAKE_TOOLCHAIN_FILE=cmake/arm-gcc-toolchain.cmake
2) ninja

flash:
0) st-flash needed
1) ninja flash 
2) st-info --probe # show usb stm32 driver
