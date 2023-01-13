meson compile -C build
cp ./build/main.js ./build/main.wasm ../docs/wasm
cp ./build/main.data ../docs
echo "Wasm files exported to the webpage."
