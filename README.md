# DDS Example

The repo provides some DDS example.

* simple_example: Simple example to publish and subscribe data.
* status_example: Use transient_local and liveliness to check the status of certain device.
* instance_example: Use instance to get data with certain key. You can refer to the test program `take_instance.c` in CycloneDDS.
* listener_example: Use listener in CycloneDDS to monitor event trigger.
* ros_example: Use DDS to pub/sub ROS topic.

The type of IDL in Cyclone DDS: https://github.com/eclipse-cyclonedds/cyclonedds/issues/350

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

* Instance example

```bash
./build/instance_example/instance_subscriber
./build/instance_example/instance_publisher
```

* Listener example

```bash
./build/listener_example/listener_subscriber
./build/listener_example/listener_publisher
```

* ROS example

```bash
./build/ros_example/ros_subscriber
./build/ros_example/ros_publisher
```

