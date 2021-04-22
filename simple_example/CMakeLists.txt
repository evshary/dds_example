idlc_generate(TestData_lib "TestData.idl")

add_executable(simple_publisher publisher.c)
target_link_libraries(simple_publisher PRIVATE TestData_lib CycloneDDS::ddsc)

add_executable(simple_subscriber subscriber.c)
target_link_libraries(simple_subscriber PRIVATE TestData_lib CycloneDDS::ddsc)
