 
Ushi is an Enity-Component-System implementation using C++20. The implementation 
is incomplete and will be updated.

## Features

&#9989; Compact data to improve cache locality when writing or accessing <br>
&#9989; Simple interface <br>
&#9989; Compile-time configurations <br>
&#9989; Type safety and better compiler error messages with C++20 <code>concept</code><br>

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
├── Benchmarks                  cache and performance benchmarks
│   └── Data                    benchmarking results
├── CMake                       cmake utilities
├── Examples                    basic usage examples
├── Include/Ushi                source code      
│       ├── Container           dense map, dense set, graph, etc
│       ├── Core                base layer
│       ├── Impl                core modules
│       │   ├── Component       component module
│       │   ├── Concurrency     concurrency handler
│       │   ├── Config          compile-time configurations
│       │   ├── Entity          entity module
│       │   ├── Group           group (archetype)   
│       │   ├── Signal          defer, update, etc (lazy evaluation related)
│       │   ├── System          system module
│       │   └── World           highest-level abstraction of the implementation
│       ├── Interface           public interface (includes a forward header)
│       └── Memory              memory-related modules
│           ├── Allocator       custom allocators
│           └── Cache           look-up cache
└── Test                        contains mostly integration tests
```

## How Ushi works

## Issues

## Acknowledgements

## License

