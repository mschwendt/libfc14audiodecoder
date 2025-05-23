x96_64 wihout debug output
```sh
CXXFLAGS="-march=native -mfpmath=sse -O3 -std=gnu++26" ./build.sh --prefix=/usr --libdir=/usr/lib64
make install
fcplay -r .
```

x86_64 with debug output:
```sh
CXXFLAGS="-march=native -mfpmath=sse -O3 -std=gnu++26 -DDEBUG=1 -DDEBUG2=1 -DDEBUG3=1" ./build.sh --prefix=/usr --libdir=/usr/lib64
make install
fcplay -r .
```

x86_64 with experimental extra debug dump output:
```sh
CXXFLAGS="-march=native -mfpmath=sse -O3 -std=gnu++26 -DDEBUG=1 -DDEBUG2=1 -DDEBUG3=1 -DDEBUG_DUMP=1" ./build.sh --prefix=/usr --libdir=/usr/lib64
make install
fcplay -r .
```
