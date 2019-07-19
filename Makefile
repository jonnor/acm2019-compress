compr: compr.cpp
	g++ $< -o $@ -lbfd -liberty -lz -ldl

run: compr
	./compr dumps/xz.core

clean:
	-rm compr
