cmake_minimum_required( VERSION 3.1 )

project( grpc_example )

find_package(gRPC CONFIG REQUIRED)

find_package( Protobuf REQUIRED )

add_executable( server server.cpp ./../proto/helloworld.grpc.pb.cc ./../proto/helloworld.pb.cc)

add_executable( client client.cpp ./../proto/helloworld.grpc.pb.cc ./../proto/helloworld.pb.cc)

add_executable( server_async server_async.cpp ./../proto/helloworld.grpc.pb.cc ./../proto/helloworld.pb.cc)

add_executable( client_async client_async.cpp ./../proto/helloworld.grpc.pb.cc ./../proto/helloworld.pb.cc)

add_executable( client_async2 client_async2.cpp ./../proto/helloworld.grpc.pb.cc ./../proto/helloworld.pb.cc)


target_link_libraries( server PRIVATE gRPC::grpc++ gRPC::grpc++_reflection gRPC::gpr gRPC::grpc gRPC::grpc++ protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite )

target_link_libraries( client PRIVATE gRPC::grpc++ gRPC::grpc++_reflection gRPC::gpr gRPC::grpc gRPC::grpc++ protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite )

target_link_libraries( server_async PRIVATE gRPC::grpc++ gRPC::grpc++_reflection gRPC::gpr gRPC::grpc gRPC::grpc++ protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite )

target_link_libraries( client_async PRIVATE gRPC::grpc++ gRPC::grpc++_reflection gRPC::gpr gRPC::grpc gRPC::grpc++ protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite )

target_link_libraries( client_async2 PRIVATE gRPC::grpc++ gRPC::grpc++_reflection gRPC::gpr gRPC::grpc gRPC::grpc++ protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite )
