idlc_generate(ListenerData_lib "ListenerData.idl")

add_executable(listener_publisher publisher.c)
target_link_libraries(listener_publisher PRIVATE ListenerData_lib CycloneDDS::ddsc)

add_executable(listener_subscriber subscriber.c)
target_link_libraries(listener_subscriber PRIVATE ListenerData_lib CycloneDDS::ddsc)
