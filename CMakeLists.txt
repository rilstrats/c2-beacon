cmake_minimum_required(VERSION 3.12)
project(c2-beacon LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(SRCS
	main.c
	c2api.c
	c2api.h
	c2json.c
	c2json.h
	c2request.c
	c2request.h
	c2types.h
)

add_executable(c2-beacon ${SRCS})

target_link_libraries(c2-beacon cjson curl)
