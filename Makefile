build: src/main.cpp
	g++ -o "build/santa" "src/main.cpp"

run: build
	./build/santa

clean:
	rm -rf build/*