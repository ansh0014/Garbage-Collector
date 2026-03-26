# Garbage Collector

Educational mark-and-sweep garbage collector in C with a custom allocator.

This project is intended for learning runtime internals and memory management concepts.

## Features

- Custom heap allocator
- Mark-and-sweep collection pass
- CMake-based build
- Windows and Linux build support

## Project Layout

- include/common.h
- src/allocator/allocator.h
- src/allocator/allocator.c
- src/gc/gc.h
- src/gc/gc.c
- src/runtime/init.h
- src/runtime/init.c
- src/main.c

## Build and Run

### Windows (PowerShell or CMD)

	cd /d "D:\Garbage Collector\Garbage-Collector"
	cmake -S . -B build
	cmake --build build --config Debug
	.\build\Debug\garbage_collector.exe

### Linux

	cd /path/to/Garbage-Collector
	cmake -S . -B build
	cmake --build build
	./build/garbage_collector

## Expected Demo Output

Depending on the active main program, you should see either:

	Before GC: 42
	GC done

or stress-test output:

	PASS: GC stress test completed
	PASS: validated live objects and post-GC allocations (...)

If you see a FAIL line, it indicates a GC logic issue that still needs fixing.

## Docker

Build and run with Docker Compose:

	docker compose up --build

Run in background:

	docker compose up --build -d

Stop and remove containers:

	docker compose down

Open an interactive debug shell (debug profile):

	docker compose --profile debug run --rm gc-debug

Build image directly (without Compose):

	docker build -t garbage-collector .

Run image directly:

	docker run --rm garbage-collector

## Publish Docker Image

	docker tag garbage-collector <dockerhub-user>/garbage-collector:latest
	docker push <dockerhub-user>/garbage-collector:latest

Then others can run:

	docker run --rm <dockerhub-user>/garbage-collector:latest

## Notes

- This is a prototype collector for education, not production use.
- Conservative root scanning can retain objects longer than expected.
- Validate behavior with stress tests before extending features.

## License

See LICENSE.