cmake_minimum_required( VERSION 3.1 )

project( grpc_example )

set(Poco_DIR "E:/ENV/lib/lib/x64/poco_1_12_6/cmake")
set(Poco_INCLUDE_DIR "E:/ENV/lib/lib/x64/poco_1_12_6/include")

find_package(gRPC CONFIG REQUIRED)
find_package(Poco REQUIRED Foundation)
find_package(Protobuf REQUIRED )

add_executable( grpc_demo ../main.cpp client.h server.h work_thread.h ./../proto/helloworld.grpc.pb.cc ./../proto/helloworld.pb.cc ./../proto/base.grpc.pb.cc ./../proto/base.pb.cc)

target_include_directories( grpc_demo PUBLIC ${Poco_INCLUDE_DIR})


target_link_libraries( grpc_demo PRIVATE gRPC::grpc++ gRPC::grpc++_reflection gRPC::gpr gRPC::grpc gRPC::grpc++ protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite Poco::Foundation)
