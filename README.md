### JugiCine HL framework 


JugiCine HL framework (working name) is a high level game development system for creating 2d sprite based games.
It is currently in development and not yet ready for production of complete games.


This application requires nCine engine and should be built as a ncine 'game'.

1. Download repository into the same directory where nCine is located.
2. Create directory 'JugiCineHL-data' in the same directory and copy directory 'data' from 'examples/testbed' into it.
3. Use ncline tool to build the game.
4. You may need to copy 'data' directory into the directory where the game has been built.

#### Web build
Before building version for web (emscripten) you must change a cmake file inside your ncine web build.

1. Open file '...nCine_web_build/project/cmake/project_main.cmake'

2. Find line (in emscripten section):

```cmake
target_link_options(${NCPROJECT_EXE_NAME} PRIVATE "SHELL:--preload-file ${NCPROJECT_DATA_DIR}/data@")
```
3. Replace it with line:

```cmake
target_link_options(${NCPROJECT_EXE_NAME} PRIVATE "SHELL:-s STACK_SIZE=128KB --preload-file ${NCPROJECT_DATA_DIR}@")
```



   