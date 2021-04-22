# DDS Example

The repo provides some DDS example.

simple_example: Simple example to publish and subscribe data.
status_example: Use transient_local and liveliness to check the status of certain device.

## Build

```bash
cmake -Bbuild -H. && cmake --build build
```

## Run

* Simple example

```bash
./build/simple_example/simple_subscriber
./build/simple_example/simple_publisher
```

* Status example

```bash
./build/status_example/status_subscriber
./build/status_example/status_publisher
```
