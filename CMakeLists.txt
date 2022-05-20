cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(qblauncher)
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,-q -Wall -O3 -std=gnu99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-rtti -fno-exceptions")

include_directories(
)

link_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
)

add_definitions(-DRELEASE)

add_executable(qblauncher
  main.c
)

target_link_libraries(qblauncher
  taihen_stub
  SceLibKernel_stub_weak
  SceCtrl_stub_weak
  SceAppMgr_stub_weak
  SceLibc_stub_weak
  SceIofilemgr_stub_weak
  gcc
)

set_target_properties(qblauncher
  PROPERTIES LINK_FLAGS "-nostdlib"
)

vita_create_self(qblauncher.suprx qblauncher
  CONFIG ${CMAKE_SOURCE_DIR}/qblauncher.yml
)
