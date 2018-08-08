# Simple-ocr

A simple OCR program, written in C++. It supports various kernels for the vector-vector distance. It uses the k-nearest neighbors algorithm and the nearest centroid classifier for recognizing the digits.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

The basic requirements for building the executable are:

* CMake 2.8+
* a c++ compiler
* make

#### Installation on Ubuntu

```
sudo apt-get install build-essentials cmake
```

### Installing

This program have been tested on Ubuntu 16.04 but should work under any systems that fulfills the aforementioned requirements.

#### Installation on Ubuntu

If you succesfully cloned the source files and you are currently in the project directory, you can generate the Makefile using the following command:

```
mkdir build
cd build/
cmake ..
```
Now you can build the binary by running:

```
make
```

## Usage instructions

The program can be run with the following command line:
```
./ocr [number of the kernel]
```
There are three kernels which are supported: 0 - linear kernel, 1 - polinomial kernel, 2 - Gauss kernel. So, if you want to run the problem with the Gauss kernel, you should call:
```
./ocr 2
```
The program outputs the result for each estimation of the learning algorithm. For a big speed-up, remove the printing commands.

## Authors

* **Biró Enikő** - [BiroEniko](https://github.com/biroeniko)
