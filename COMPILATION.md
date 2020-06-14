It does *NOT* compile on
Microsoft Windows 8.1 - with Visual Studio 2019 - MSVC Version 19.00.24245 for x86
possibly because of a compiler bug

See
https://developercommunity.visualstudio.com/content/problem/831128/c-invalid-template-argument-for-template-parameter.html
https://developercommunity.visualstudio.com/content/problem/293643/recursive-template-instantiation-throws-c3200.html

though it might be fixed
https://developercommunity.visualstudio.com/content/problem/926958/invalid-template-argument-for-template-parameter-e-1.html

I don't have the time to update and check

Confirmed to compile on
Linux 5.6.11-arch1-1 x86_64 (Archlinux)
With clang:
     clang version 10.0.0
     Target: x86_64-pc-linux-gnu
     Thread model: posix
With g++:
     g++ (Arch Linux 9.3.0-1) 9.3.0

Linux 5.6.15-arch1-1 x86_64 (Archlinux)
With clang:
    clang version 10.0.0
    Target: x86_64-pc-linux-gnu
    Thread model: posix
With g++:
    g++ (GCC) 10.1.0
