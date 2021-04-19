idlc_generate(TestData_lib "TestData.idl")

add_executable(publisher publisher.c)
target_link_libraries(publisher PRIVATE TestData_lib CycloneDDS::ddsc)

add_executable(subscriber subscriber.c)
target_link_libraries(subscriber PRIVATE TestData_lib CycloneDDS::ddsc)
