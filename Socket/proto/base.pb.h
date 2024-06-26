// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: base.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_base_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_base_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3021000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3021012 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_base_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_base_2eproto {
  static const uint32_t offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_base_2eproto;
class BaseReply;
struct BaseReplyDefaultTypeInternal;
extern BaseReplyDefaultTypeInternal _BaseReply_default_instance_;
class BaseRequest;
struct BaseRequestDefaultTypeInternal;
extern BaseRequestDefaultTypeInternal _BaseRequest_default_instance_;
PROTOBUF_NAMESPACE_OPEN
template<> ::BaseReply* Arena::CreateMaybeMessage<::BaseReply>(Arena*);
template<> ::BaseRequest* Arena::CreateMaybeMessage<::BaseRequest>(Arena*);
PROTOBUF_NAMESPACE_CLOSE

// ===================================================================

class BaseRequest final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:BaseRequest) */ {
 public:
  inline BaseRequest() : BaseRequest(nullptr) {}
  ~BaseRequest() override;
  explicit PROTOBUF_CONSTEXPR BaseRequest(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  BaseRequest(const BaseRequest& from);
  BaseRequest(BaseRequest&& from) noexcept
    : BaseRequest() {
    *this = ::std::move(from);
  }

  inline BaseRequest& operator=(const BaseRequest& from) {
    CopyFrom(from);
    return *this;
  }
  inline BaseRequest& operator=(BaseRequest&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const BaseRequest& default_instance() {
    return *internal_default_instance();
  }
  static inline const BaseRequest* internal_default_instance() {
    return reinterpret_cast<const BaseRequest*>(
               &_BaseRequest_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(BaseRequest& a, BaseRequest& b) {
    a.Swap(&b);
  }
  inline void Swap(BaseRequest* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(BaseRequest* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  BaseRequest* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<BaseRequest>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const BaseRequest& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom( const BaseRequest& from) {
    BaseRequest::MergeImpl(*this, from);
  }
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _impl_._cached_size_.Get(); }

  private:
  void SharedCtor(::PROTOBUF_NAMESPACE_ID::Arena* arena, bool is_message_owned);
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(BaseRequest* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "BaseRequest";
  }
  protected:
  explicit BaseRequest(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kBufferFieldNumber = 3,
    kVersionFieldNumber = 1,
    kLengthFieldNumber = 2,
  };
  // bytes buffer = 3;
  void clear_buffer();
  const std::string& buffer() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_buffer(ArgT0&& arg0, ArgT... args);
  std::string* mutable_buffer();
  PROTOBUF_NODISCARD std::string* release_buffer();
  void set_allocated_buffer(std::string* buffer);
  private:
  const std::string& _internal_buffer() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_buffer(const std::string& value);
  std::string* _internal_mutable_buffer();
  public:

  // int32 version = 1;
  void clear_version();
  int32_t version() const;
  void set_version(int32_t value);
  private:
  int32_t _internal_version() const;
  void _internal_set_version(int32_t value);
  public:

  // int32 length = 2;
  void clear_length();
  int32_t length() const;
  void set_length(int32_t value);
  private:
  int32_t _internal_length() const;
  void _internal_set_length(int32_t value);
  public:

  // @@protoc_insertion_point(class_scope:BaseRequest)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr buffer_;
    int32_t version_;
    int32_t length_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_base_2eproto;
};
// -------------------------------------------------------------------

class BaseReply final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:BaseReply) */ {
 public:
  inline BaseReply() : BaseReply(nullptr) {}
  ~BaseReply() override;
  explicit PROTOBUF_CONSTEXPR BaseReply(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  BaseReply(const BaseReply& from);
  BaseReply(BaseReply&& from) noexcept
    : BaseReply() {
    *this = ::std::move(from);
  }

  inline BaseReply& operator=(const BaseReply& from) {
    CopyFrom(from);
    return *this;
  }
  inline BaseReply& operator=(BaseReply&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const BaseReply& default_instance() {
    return *internal_default_instance();
  }
  static inline const BaseReply* internal_default_instance() {
    return reinterpret_cast<const BaseReply*>(
               &_BaseReply_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(BaseReply& a, BaseReply& b) {
    a.Swap(&b);
  }
  inline void Swap(BaseReply* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(BaseReply* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  BaseReply* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<BaseReply>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const BaseReply& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom( const BaseReply& from) {
    BaseReply::MergeImpl(*this, from);
  }
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _impl_._cached_size_.Get(); }

  private:
  void SharedCtor(::PROTOBUF_NAMESPACE_ID::Arena* arena, bool is_message_owned);
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(BaseReply* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "BaseReply";
  }
  protected:
  explicit BaseReply(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kBufferFieldNumber = 3,
    kVersionFieldNumber = 1,
    kLengthFieldNumber = 2,
  };
  // bytes buffer = 3;
  void clear_buffer();
  const std::string& buffer() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_buffer(ArgT0&& arg0, ArgT... args);
  std::string* mutable_buffer();
  PROTOBUF_NODISCARD std::string* release_buffer();
  void set_allocated_buffer(std::string* buffer);
  private:
  const std::string& _internal_buffer() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_buffer(const std::string& value);
  std::string* _internal_mutable_buffer();
  public:

  // int32 version = 1;
  void clear_version();
  int32_t version() const;
  void set_version(int32_t value);
  private:
  int32_t _internal_version() const;
  void _internal_set_version(int32_t value);
  public:

  // int32 length = 2;
  void clear_length();
  int32_t length() const;
  void set_length(int32_t value);
  private:
  int32_t _internal_length() const;
  void _internal_set_length(int32_t value);
  public:

  // @@protoc_insertion_point(class_scope:BaseReply)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr buffer_;
    int32_t version_;
    int32_t length_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_base_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// BaseRequest

// int32 version = 1;
inline void BaseRequest::clear_version() {
  _impl_.version_ = 0;
}
inline int32_t BaseRequest::_internal_version() const {
  return _impl_.version_;
}
inline int32_t BaseRequest::version() const {
  // @@protoc_insertion_point(field_get:BaseRequest.version)
  return _internal_version();
}
inline void BaseRequest::_internal_set_version(int32_t value) {
  
  _impl_.version_ = value;
}
inline void BaseRequest::set_version(int32_t value) {
  _internal_set_version(value);
  // @@protoc_insertion_point(field_set:BaseRequest.version)
}

// int32 length = 2;
inline void BaseRequest::clear_length() {
  _impl_.length_ = 0;
}
inline int32_t BaseRequest::_internal_length() const {
  return _impl_.length_;
}
inline int32_t BaseRequest::length() const {
  // @@protoc_insertion_point(field_get:BaseRequest.length)
  return _internal_length();
}
inline void BaseRequest::_internal_set_length(int32_t value) {
  
  _impl_.length_ = value;
}
inline void BaseRequest::set_length(int32_t value) {
  _internal_set_length(value);
  // @@protoc_insertion_point(field_set:BaseRequest.length)
}

// bytes buffer = 3;
inline void BaseRequest::clear_buffer() {
  _impl_.buffer_.ClearToEmpty();
}
inline const std::string& BaseRequest::buffer() const {
  // @@protoc_insertion_point(field_get:BaseRequest.buffer)
  return _internal_buffer();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void BaseRequest::set_buffer(ArgT0&& arg0, ArgT... args) {
 
 _impl_.buffer_.SetBytes(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:BaseRequest.buffer)
}
inline std::string* BaseRequest::mutable_buffer() {
  std::string* _s = _internal_mutable_buffer();
  // @@protoc_insertion_point(field_mutable:BaseRequest.buffer)
  return _s;
}
inline const std::string& BaseRequest::_internal_buffer() const {
  return _impl_.buffer_.Get();
}
inline void BaseRequest::_internal_set_buffer(const std::string& value) {
  
  _impl_.buffer_.Set(value, GetArenaForAllocation());
}
inline std::string* BaseRequest::_internal_mutable_buffer() {
  
  return _impl_.buffer_.Mutable(GetArenaForAllocation());
}
inline std::string* BaseRequest::release_buffer() {
  // @@protoc_insertion_point(field_release:BaseRequest.buffer)
  return _impl_.buffer_.Release();
}
inline void BaseRequest::set_allocated_buffer(std::string* buffer) {
  if (buffer != nullptr) {
    
  } else {
    
  }
  _impl_.buffer_.SetAllocated(buffer, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (_impl_.buffer_.IsDefault()) {
    _impl_.buffer_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:BaseRequest.buffer)
}

// -------------------------------------------------------------------

// BaseReply

// int32 version = 1;
inline void BaseReply::clear_version() {
  _impl_.version_ = 0;
}
inline int32_t BaseReply::_internal_version() const {
  return _impl_.version_;
}
inline int32_t BaseReply::version() const {
  // @@protoc_insertion_point(field_get:BaseReply.version)
  return _internal_version();
}
inline void BaseReply::_internal_set_version(int32_t value) {
  
  _impl_.version_ = value;
}
inline void BaseReply::set_version(int32_t value) {
  _internal_set_version(value);
  // @@protoc_insertion_point(field_set:BaseReply.version)
}

// int32 length = 2;
inline void BaseReply::clear_length() {
  _impl_.length_ = 0;
}
inline int32_t BaseReply::_internal_length() const {
  return _impl_.length_;
}
inline int32_t BaseReply::length() const {
  // @@protoc_insertion_point(field_get:BaseReply.length)
  return _internal_length();
}
inline void BaseReply::_internal_set_length(int32_t value) {
  
  _impl_.length_ = value;
}
inline void BaseReply::set_length(int32_t value) {
  _internal_set_length(value);
  // @@protoc_insertion_point(field_set:BaseReply.length)
}

// bytes buffer = 3;
inline void BaseReply::clear_buffer() {
  _impl_.buffer_.ClearToEmpty();
}
inline const std::string& BaseReply::buffer() const {
  // @@protoc_insertion_point(field_get:BaseReply.buffer)
  return _internal_buffer();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void BaseReply::set_buffer(ArgT0&& arg0, ArgT... args) {
 
 _impl_.buffer_.SetBytes(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:BaseReply.buffer)
}
inline std::string* BaseReply::mutable_buffer() {
  std::string* _s = _internal_mutable_buffer();
  // @@protoc_insertion_point(field_mutable:BaseReply.buffer)
  return _s;
}
inline const std::string& BaseReply::_internal_buffer() const {
  return _impl_.buffer_.Get();
}
inline void BaseReply::_internal_set_buffer(const std::string& value) {
  
  _impl_.buffer_.Set(value, GetArenaForAllocation());
}
inline std::string* BaseReply::_internal_mutable_buffer() {
  
  return _impl_.buffer_.Mutable(GetArenaForAllocation());
}
inline std::string* BaseReply::release_buffer() {
  // @@protoc_insertion_point(field_release:BaseReply.buffer)
  return _impl_.buffer_.Release();
}
inline void BaseReply::set_allocated_buffer(std::string* buffer) {
  if (buffer != nullptr) {
    
  } else {
    
  }
  _impl_.buffer_.SetAllocated(buffer, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (_impl_.buffer_.IsDefault()) {
    _impl_.buffer_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:BaseReply.buffer)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)


// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_base_2eproto
