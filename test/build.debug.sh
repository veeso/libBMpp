g++ -g -Wall main_bmp24.cpp ../src/bmp.cpp ../src/bmp24.cpp ../src/pixels/rgbpixel.cpp -I ../include/ -o bmp24_test -D BMP_DEBUG
g++ -g -Wall main_bmp8.cpp ../src/bmp.cpp ../src/bmp8.cpp ../src/pixels/bytepixel.cpp -I ../include/ -o bmp8_test -D BMP_DEBUG
#g++ -g -Wall main_monochrome.cpp ../src/bmp.cpp ../src/bmpmonochrome.cpp ../src/pixels/bwpixel.cpp -I ../include/ -o monochrome_test
