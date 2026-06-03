# SupportFFmpeg

SupportFFmpeg provides FFmpeg-based movie input/output support for KVS.

## Enable this support

Edit `kvs.conf`:

```make
KVS_SUPPORT_FFMPEG = 1
```

Or enable it when configuring with CMake:

```sh
cmake -S . -B build -DKVS_SUPPORT_FFMPEG=ON
```

## Requirements

SupportFFmpeg requires FFmpeg development headers and libraries.

The current bundled AvCpp-based implementation is intended for FFmpeg 5 or older. FFmpeg 6 or newer may require updating the copied AvCpp sources under `Source/SupportFFmpeg/AvCpp` to a newer AvCPP version and applying compatibility changes for newer FFmpeg APIs.

## Install dependencies

### Linux

```sh
sudo apt-get install ffmpeg libavcodec-dev libavformat-dev libavutil-dev libswscale-dev libswresample-dev
```

### macOS

```sh
brew install ffmpeg
```

For Intel Mac:

```sh
export KVS_FFMPEG_DIR=/usr/local/opt/ffmpeg
```

For Apple Silicon Mac:

```sh
export KVS_FFMPEG_DIR=/opt/homebrew/opt/ffmpeg
```

If you need a specific Homebrew formula, such as `ffmpeg@5`, point `KVS_FFMPEG_DIR` to that formula:

```sh
export KVS_FFMPEG_DIR=/opt/homebrew/opt/ffmpeg@5
```

### Windows

Windows support depends on the FFmpeg binary package and compiler ABI. Use a package that provides headers, import libraries, and DLLs compatible with your Visual C++ compiler.

Set:

| Variable | Example |
|:--|:--|
| `KVS_FFMPEG_DIR` | `C:\ffmpeg` |

The directory is expected to contain `include` and `lib` subdirectories. Make sure the FFmpeg DLLs are available in `PATH` or next to the executable.

## Build KVS with make

```sh
export KVS_DIR=$HOME/local/kvs
export KVS_FFMPEG_DIR=/opt/homebrew/opt/ffmpeg
make
make install
```

If automatic library detection is not sufficient, specify include/library/link flags explicitly:

```sh
export KVS_FFMPEG_INCLUDE_PATH=/path/to/ffmpeg/include
export KVS_FFMPEG_LIBRARY_PATH=/path/to/ffmpeg/lib
export KVS_FFMPEG_LINK_LIBRARY="-lavformat -lavcodec -lavutil -lswscale -lswresample"
```

## Build KVS with CMake

```sh
cmake -S . -B build \
  -DKVS_DIR=$HOME/local/kvs \
  -DKVS_SUPPORT_FFMPEG=ON \
  -DKVS_FFMPEG_DIR=/opt/homebrew/opt/ffmpeg
cmake --build build -j
cmake --install build
```

CMake can also use `pkg-config` when FFmpeg `.pc` files are available.

## Build examples

After installing KVS:

```sh
cd Example/SupportFFmpeg/MovieRenderer
kvsmake -G
kvsmake
```

## Troubleshooting

### Compile errors with FFmpeg 6 or newer

Update the AvCpp sources copied into `Source/SupportFFmpeg/AvCpp`, or use FFmpeg 5 or older.

### Libraries are not found

Set `KVS_FFMPEG_DIR` first. If that is not enough, set `KVS_FFMPEG_INCLUDE_PATH`, `KVS_FFMPEG_LIBRARY_PATH`, and `KVS_FFMPEG_LINK_LIBRARY`.
