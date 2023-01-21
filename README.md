To get emsdk:
```
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
git pull
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh
```

Compile with meson and emscripten:
```
meson setup build --cross-file=wasm.ini
```
