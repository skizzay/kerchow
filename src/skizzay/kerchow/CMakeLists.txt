set(KERCHOW_TARGET skizzay_kerchow)
add_library(${KERCHOW_TARGET} picker.cpp)

# Try to use something that generates fairly decent entropy
IF (EXISTS /dev/urandom)
   target_compile_definitions(${KERCHOW_TARGET} PRIVATE KERCHOW_USE_DEV_URANDOM)
elseif (EXISTS /dev/random)
   target_compile_definitions(${KERCHOW_TARGET} PRIVATE KERCHOW_USE_DEV_RANDOM)
endif()
