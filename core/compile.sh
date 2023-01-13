meson compile -C build
cp ./build/main.js ./build/main.wasm ../docs/wasm
echo "Wasm files exported to the webpage."
