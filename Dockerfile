FROM ubuntu:24.04 AS build

WORKDIR /work
RUN apt-get update && \
    apt-get install build-essential libclang-17-dev clang-17 llvm-17-dev cmake -y

COPY src /work/src/
COPY include /work/include/
COPY CMakeLists.txt /work/

# build
RUN mkdir build && \
    cd build && \
    cmake .. -DCMAKE_BUILD_TYPE=MinSizeRel -DLLVMVersion=17 && \
    cmake --build . && \
    cpack .

# runnable
FROM ubuntu:24.04 AS executable
COPY --from=build /work/build/golfC-1.2.0-Linux.deb /
RUN apt-get update && \
    apt install /golfC-1.2.0-Linux.deb clang-17 -y && \
    rm -rf /var/lib/apt/lists/*
ENTRYPOINT [ "minifier", "--", "-I", "/usr/lib/clang/17/include" ]