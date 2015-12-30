### WARNING

The library has not bee reviewed by others yet, therefore it should not be used for anything critical !
The library needs lot of work still !

### Crypto Engine library for C++ - V 0.0.1

This library simplifies the usage of NaCl for C++

### INSTALLATION

From the cryptoengine-lib folder execute the shell command:

`sh getdeps.sh`

This downloads the dependencies and compiles them.

Now enter the `build` folder created by the `getdeps.sh` script and execute:

`cmake -DCMAKE_BUILD_TYPE=Release -DGTEST_ENABLED=ON -DBORINGSSL_ENABLED=ON -DLIBRESSL_ENABLED=OFF ..`

`make`

`./tests/CryptoEngineTest`

`make install`

### How to use the library

Add it to your `CMakeLists.txt` via:

```
find_package(LibCryptoEngine REQUIRED)
if(LibCryptoEngine_FOUND)
  include_directories(${LibCryptoEngine_INCLUDE_DIR})
  target_include_directories(REPLACE_HERE_WITH_TARGET_NAME PUBLIC ${LibCryptoEngine_INCLUDE_DIR})
  target_link_libraries (REPLACE_HERE_WITH_TARGET_NAME ${LibCryptoEngine_LIBRARY})
endif()

```

You can find the Cmake Find Module inside the `CMake` folder, its name is: `FindLibCryptoEngine.cmake`


For information on how to use the library itself see ther wiki.


This is the big picture:

```
Message -> Encrypt -> EncryptedMessage -> ToBytes() => NETWORK <= FromBytes() -> EncryptedMessage -> Decrypt -> Message
```


### TODO

Fuzzing with American Fuzzy Loop


### LICENSE

Copyright (c) 2015 Sec51.com <info@sec51.com>

Permission to use, copy, modify, and distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.


