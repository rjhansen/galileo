# Galileo

This is the result of three itches being hit with one scratch:

* A friend needed a simple example of GUI programming to work through with his kids,
* A teacher needed a simple tool to calculate sightlines, and
* I needed a reusable code skeleton for [Qt](https://www.qt.io/download-open-source) development.

If you need any of the three, you're in luck.

## Building

1. [Download the source code](https://github.com/rjhansen/galileo/releases).
2. Uncompress the source code somewhere.
3. Open a terminal window that has access to your developer tools. On macOS, `Terminal`
   will do fine. On Linux, use your terminal of choice. On Windows, open a Visual Studio
   terminal.
4. `cd` into the directory with the source code. Create a directory there called `build`
   and `cd` there.
6. Follow the installation-specific instructions below.

### Win32
You will need [Qt](https://www.qt.io/download-open-source) installed.

1. `cmake -S .. -B . -DCMAKE_PREFIX_PATH=C:\Qt\6.10.0\msvc2022_64`
2. Open the resulting Visual Studio solution file in Visual Studio.
3. Build, run, and debug all within the IDE.

### macOS
You will need Qt installed from [Homebrew](https://brew.sh), and [XCode](https://developer.apple.com/xcode/).

1. `cmake -S .. -B .`
2. Open the resulting XCode project in XCode.
3. Build, run, and debug all within the IDE.

### Linux and/or FreeBSD
You will need Qt installed from your favorite repository, as well as your choice of recent C++ compilers, and
Make.

1. `cmake -S .. -B . -DCMAKE_BUILD_TYPE=Release`
2. `make`
3. Build, run, and debug using your favorite tools.

## Reporting bugs
Please file bug reports in the [project tracker](https://github.com/rjhansen/galileo/issues).

## Copyright and Licensing
Everything is © 2020–2025, [Rob Hansen](mailto:rob@hansen.engineering?subject=Galileo) and is distributed under the ISC License.

Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.

**THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.**
