# What is this?

TBD

# Tests

## How to run tests in release mode

```
mkdir release
cd release
cmake -DCMAKE_BUILD_TYPE=Release .. && make && ./olymplib-tests
```

## How to run tests in debug mode (excludes stress testing because it takes way too long)

```
mkdir debug
cd debug
cmake -DCMAKE_BUILD_TYPE=Debug .. && make -j 5 && ./olymplib-tests --gtest_filter=-stress*
```
