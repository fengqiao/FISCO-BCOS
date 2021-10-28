
file(GLOB_RECURSE SRC_LIST "*.cpp")
file(GLOB_RECURSE HEADERS "*.h")
add_library(precompiled ${SRC_LIST} ${HEADERS})
target_link_libraries(precompiled PRIVATE blockverifier ethcore)
target_link_libraries(precompiled PUBLIC storage devcore JsonCpp)
target_link_libraries(precompiled PRIVATE Paillier GroupSig VRF)
add_definitions(-std=c++17)
add_subdirectory(extension)
