 
Ushi is an Enity-Component-System implementation using C++20. The implementation 
is incomplete and will be updated.

## Features

&#9989; Compact data to improve cache locality <br>
&#9989; Compile-time configurations <br>
&#9989; Type safety and better compile-error messages (slightly) with C++20 <code>concept</code><br>
&#9989; Simple interface <br>

#### Under-development features

* Concurrency <br>
* Lazy evaluation <br>
* Custom allocators support through compile-time configurations <br>

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
│       │   ├── System          system module
│       │   └── World           highest-level abstraction of the implementation
│       └── Interface           public interface (includes a forward header)
└── Test                        contains mostly integration tests
```

## How Ushi works

## Issues

## Acknowledgements

## License

