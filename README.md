 
Ushi is an Enity-Component-System implementation using C++20. The implementation 
is incomplete and will be updated regularly.

## Features

&#9989; Compact data to improve cache locality when writing or accessing <br>
&#9989; Simple interface <br>
&#9989; Compile-time configurations <br>

#### Under-development features

* Replacing some STL containers with more compact data structures, i.e. dense set, graph, dense map, etc <br>
* Separated archetype implementation (group) <br>
* Concurrency <br>
* Ranges and views for certain groups of data <br>
* Lazy evaluation and cache <br>
* Signals <br>
* Custom allocators support through compile-time configurations <br>
* Lua scripting (if necessary)

## Projects

## Build instructions

The build is CMake-based.

## Project layout

```
├── Benchmarks
│   └── Data
├── CMake
├── Examples
├── Include
│   └── Ushi
│       ├── Container
│       ├── Core
│       ├── Impl
│       │   ├── Component
│       │   ├── Concurrency
│       │   ├── Config
│       │   ├── Entity
│       │   ├── Group
│       │   ├── Signal
│       │   ├── System
│       │   └── World
│       ├── Interface
│       └── Memory
│           ├── Allocator
│           └── Cache
└── Test
```

## How Ushi works

## Issues

## Acknowledgements

## License

