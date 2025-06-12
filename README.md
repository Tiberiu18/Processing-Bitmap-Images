# Bitmap Image Processing

This project provides a simple command line utility for manipulating bitmap files. It supports operations such as editing an image, inserting other images, drawing primitive shapes and performing flood fill. The original code was consolidated into a set of small modules for easier maintenance.

## Building

The sources are located in `Bitmap Image Processing/BmpParser`. To build the tool run:

```bash
cd "Bitmap Image Processing/BmpParser"
make
```

The resulting executable `BmpParser` will be created in the same directory.

## Usage

The program expects commands on `stdin`. Example:

```
edit images/christmas.bmp
set draw_color 255 0 0
set line_width 3
draw line 10 20 100 20
save output.bmp
quit
```

Input files used for testing are located under `input/` and reference outputs in `ref/`.

## License

This project is licensed under the terms of the MIT license. See [LICENSE](LICENSE) for details.
