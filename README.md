# Time Measurement tool for c++ program

## Build

```shell
mkdir build
cd build
cmake ..
make # or other build tools on your platform,for example msbuild on windows.
```

## Integrate to your project

This is a simple tool with just one cpp file and one header file,you can use it anyway you like.If you use camke, you can add a sub-diredtory to you top level CMakeLists.txt.

Cmake example:

```cmake
add_subdirectory(time_measure) # compile this lib
...
link_libraries(time_measurement) # use this lib
```

## Usage

### EXPR_MEASURE

measure the execution time of a line of exprssion

```cpp
// This will measure the exetion time of std::cout
int main () {
    EXPR_MEASURE(std::cout << "Hello World" << std::endl;)
}
```

### BLOCK_MEASURE

measure a block, for example a function

```cpp
// this will measure the whole time this function executed in millisecond
void fun() {
    BLOCK_MEASURE
    // do some job
    return;
}
```

### MEASURE_START && MEASURE_STOP

measure execution time between theset two maco with the same ID

```cpp
// This will measure the time between fun to fun1
void fun() {
    MEASURE_START("m1")
    // do some job
}

void fun1 () {
    // do some job
    MEASURE_STOP("m1")
}

int main () {
    fun();
    fun1();
}
```
