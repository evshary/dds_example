idlc_generate(StatusData_lib "StatusData.idl")

add_executable(status_publisher publisher.c)
target_link_libraries(status_publisher PRIVATE StatusData_lib CycloneDDS::ddsc)

add_executable(status_subscriber subscriber.c)
target_link_libraries(status_subscriber PRIVATE StatusData_lib CycloneDDS::ddsc)
