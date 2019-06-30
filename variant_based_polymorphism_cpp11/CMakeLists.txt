cmake_minimum_required(VERSION 3.10)

project (variant_based_polymorphism)

find_package(Boost REQUIRED)

add_executable(vbp main.cpp)

target_compile_features(vbp
    PUBLIC
        cxx_std_11
)

target_link_libraries(vbp
    PRIVATE
        Boost::boost
)
