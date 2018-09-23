
FILE = main.cpp
LIBS_HPP = bst.h files.hpp LagrangePolynomial.hpp matplotlibcpp.h seach.cpp
MATPLOTLIB = -I C:\Python37\include\ -I C:\Python37\Lib\site-packages\numpy\core\include -L C:\Python37 -lpython37

# g++ search.cpp bst.h files.hpp LagrangePolynomial.hpp matplotlibcpp.h -g3 -std=c++17 -pthread 
# -I C:\Python37\include\ -I C:\Python37\Lib\site-packages\numpy\core\include -L C:\Python37 -lpython37

# g3 para depurar
build: ${FILE}
	g++ ${FILE} ${LIBS_HPP} -g3 -std=c++17 -pthread ${MATPLOTLIB}
clear:
	del *.gch *.exe
