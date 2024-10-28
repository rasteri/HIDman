mkdir release

make clean
make TARGET=HIDman-axp_v1.1.3 BOARD_TYPE=BOARD_AXP
cp ./build/HIDman-axp_v1.1.3.bin ./release

make clean
make TARGET=HIDman-mini_serial_v1.1.3 BOARD_TYPE=BOARD_AXP OSC_TYPE=OSC_INTERNAL
cp ./build/HIDman-mini_serial_v1.1.3.bin ./release

make clean
make TARGET=HIDman-mini_v1.1.3 BOARD_TYPE=BOARD_MINI OSC_TYPE=OSC_INTERNAL
cp ./build/HIDman-mini_v1.1.3.bin ./release

make clean
make TARGET=HIDman-micro_v1.1.3 BOARD_TYPE=BOARD_MICRO OSC_TYPE=OSC_INTERNAL
cp ./build/HIDman-micro_v1.1.3.bin ./release

make clean
make TARGET=HIDman-micro_swapped_v1.1.3 BOARD_TYPE=BOARD_MICRO BOARD_OPTIONS=OPT_SWAP_KBD_MSC OSC_TYPE=OSC_INTERNAL
cp ./build/HIDman-micro_swapped_v1.1.3.bin ./release
