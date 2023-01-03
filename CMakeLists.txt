cmake_minimum_required(VERSION 3.25.0)


include_directories(include/)

project(tools C)

project(filelang C)
project(tokenizer C)
project(parser C)
project(reader C)

add_library(tools src/filelang/tools.c)
add_library(reader src/filelang/reader/reader.c)

add_library(tokenizer src/filelang/tokenizer/tokenizer.c)
add_library(parser src/filelang/parser/parser.c)

add_executable(filelang src/filelang/main.c)
target_link_libraries(filelang tokenizer tools reader)