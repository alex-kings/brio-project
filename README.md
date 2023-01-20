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


_20/01/2023_ **CORE**  
Make SURE that for non-last loops, there are always 2 3con pieces in the set of placeable pieces, and that these pieces ARE ALWAYS placed before starting generating the next loop.