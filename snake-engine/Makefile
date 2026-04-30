.PHONY: configure build run clean

configure:
	cmake -B build

build:
	cmake --build build

run: build
	./build/snake_game

clean:
	rm -rf build
