idlc_generate(Std_String_lib "std_msgs_string.idl")

add_executable(ros_publisher publisher.c)
target_link_libraries(ros_publisher PRIVATE Std_String_lib CycloneDDS::ddsc)

add_executable(ros_subscriber subscriber.c)
target_link_libraries(ros_subscriber PRIVATE Std_String_lib CycloneDDS::ddsc)
