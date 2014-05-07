cppfile = sudoku#type with a .cpp extension

last: 
	g++ $(cppfile).cpp helper.cpp `pkg-config --cflags --libs gtk+-2.0`
