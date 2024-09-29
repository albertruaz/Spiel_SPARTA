cmake_minimum_required(VERSION 3.10)
project(tiny_hanabi_test)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 컴파일러로 clang++ 설정
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)




# 경로 설정 (include 및 라이브러리 경로)
include_directories(
    /home/swkang/Spiel_SPARTA/open_spiel
    /home/swkang/Spiel_SPARTA/open_spiel/open_spiel
)

link_directories(
    /home/swkang/Spiel_SPARTA/open_spiel/build
)

# 빌드 타겟 설정
add_executable(tiny_hanabi_test_build 
    # ./csrc/tiny_hanabi_test.cc
    ./csrc/extensiontiny.cc
    ./csrc/HanabiServer.cc
    ./csrc/BotUtils.cc
    ./csrc/SimpleBot.cc
    ./csrc/SearchBot.cc
    # ./csrc/JointSearchBot.cc
)

# open_spiel 라이브러리 연결
target_link_libraries(tiny_hanabi_test_build PRIVATE open_spiel)

# PYTHONPATH 설정 (실행 시 설정)
set(ENV{PYTHONPATH} "$ENV{PYTHONPATH}:/home/swkang/Spiel_SPARTA/Hanabi_SPARTA")
