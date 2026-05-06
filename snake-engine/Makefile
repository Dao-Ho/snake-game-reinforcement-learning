.PHONY: configure build run clean test

configure:
	cmake -B build

build:
	cmake --build build

run: build
	./build/snake_game

clean:
	rm -rf build

test: build
	cd build && ctest --output-on-failure
    