Get emsdk:
```
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
git pull
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh
```

Compile the C++ program to WebAssembly, and place the compiled documents at the right location in the "docs" folder:
```
cd core
./compile.sh
```