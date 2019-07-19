compr: compr.cpp
	g++ -std=c++98 $< -o $@ -lbfd -liberty -lz -ldl

run: compr
	./compr dumps/xz.core

clean:
	-rm compr
