cmake_minimum_required(VERSION 3.2)
PROJECT (PebbleReact)

SET(PEBBLE_FLAVOUR basalt)
EXECUTE_PROCESS(COMMAND pebble sdk include-path ${PEBBLE_FLAVOUR} OUTPUT_VARIABLE PEBBLE_INCLUDE_DIR)
MESSAGE("${PEBBLE_INCLUDE_DIR}")
INCLUDE_DIRECTORIES("${PEBBLE_INCLUDE_DIR}")
INCLUDE_DIRECTORIES("build/include")
INCLUDE_DIRECTORIES("build/basalt")


FILE(GLOB SOURCE
  "src/c/**/*.h"
  "src/c/**/*.c"
)

FILE(GLOB HEADERS "${PEBBLE_INCLUDE_DIR}/*.h")

ADD_EXECUTABLE(PebbleReact ${SOURCE} ${HEADERS})