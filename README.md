# OpenGL-Learning

Leaning OpenGL

## Install & Build

```console
$ git clone https://github.com/BowiEgo/OpenGL-Learning
$ cd OpenGL-Learning
$ git submodule update --init --recursive
$ ./tools/vcpkg/bootstrap-vcpkg.sh
$ ./tools/vcpkg/vcpkg install
```

```console
$ mkdir build
$ cmake -S . -B build
$ cmake --build build
```

or just use the shell sript contains above commands.

```console
$ git clone https://github.com/BowiEgo/OpenGL-Learning
$ cd OpenGL-Learning
$ chmod +x install.sh
$ ./install.sh
```

## Run

```console
$ cd build
$ ./OpenGL_Learning
```
