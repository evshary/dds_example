idlc_generate(InstanceData_lib "InstanceData.idl")

add_executable(instance_publisher publisher.c)
target_link_libraries(instance_publisher PRIVATE InstanceData_lib CycloneDDS::ddsc)

add_executable(instance_subscriber subscriber.c)
target_link_libraries(instance_subscriber PRIVATE InstanceData_lib CycloneDDS::ddsc)
