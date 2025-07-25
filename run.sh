set -e

#> Make sure we are in the correct working directory
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
cd "$SCRIPT_DIR" || exit 1

if [[ "$1" == "reset" ]]; then
    echo "Resetting build directory"
    rm -r build
    meson setup build
fi


meson compile -C build
./build/opengl
