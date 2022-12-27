
https://github.com/fabiangreffrath/crispy-doom

### Preparations
Download sourceode and install prerequisites
```
$ git clone https://github.com/ebaklund/dmc.git
$ sudo apt install build-essential automake git
$ # sudo apt build-dep crispy-doom # Not needed and also fails
$ # One more package that I do not remember.

### Build dmc/dmr
drm is an embedded part of dmc. Build it first.
```
$ cd dmc/dmr
$ cargo build
```

### Build dmc
```
$ cd dmc
$ git clone https://github.com/ebaklund/dmc.git
$ sudo apt install build-essential automake git
$ # sudo apt build-dep crispy-doom # Not needed and also fails
$ # One more package that I do not remember.
$ cd dmc
$ autoreconf -fiv
$ ./configure
$ make
```

### WAD
```
$ cd dmc
$ wget http://cdn.debian.net/debian/pool/non-free/d/doom-wad-shareware/doom-wad-shareware_1.9.fixed.orig.tar.gz
$ gunzip doom-wad-shareware_1.9.fixed.orig.tar.gz
$ tar -xf doom-wad-shareware_1.9.fixed.orig.tar
```

### Run
```
$ cd dmc
$ ./src/crispy-doom -iwad doom-wad-shareware-1.9.fixed/doom1.wad
```
