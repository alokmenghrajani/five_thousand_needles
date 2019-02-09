cmake_minimum_required (VERSION 2.8)
project (five_thousand_needles)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -Wextra -Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes -Wcast-qual -std=gnu11 -DPB_FIELD_16BIT")

if ($ENV{TARGET} MATCHES "nCipher")
  include(codesafe.cmake)
endif ()

include_directories(include)

# Main source files
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -Wno-unknown-pragmas")

set(main_SRC
    "main.c"
)

if ($ENV{TARGET} MATCHES "nCipher")
  set(extra_SRC
       "ncipher/rand.c")
  list(APPEND main_SRC ${extra_SRC})
else ()
  set(extra_SRC
       "dev/rand.c")
  list(APPEND main_SRC ${extra_SRC})
endif ()

add_executable(five_thousand_needles ${main_SRC})
