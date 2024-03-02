// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: services/profile_service/profile.proto
// Protobuf C++ Version: 4.25.3

#ifndef GOOGLE_PROTOBUF_INCLUDED_services_2fprofile_5fservice_2fprofile_2eproto_2epb_2eh
#define GOOGLE_PROTOBUF_INCLUDED_services_2fprofile_5fservice_2fprofile_2eproto_2epb_2eh

#include <limits>
#include <string>
#include <type_traits>
#include <utility>

#include "google/protobuf/port_def.inc"
#if PROTOBUF_VERSION < 4025000
#error "This file was generated by a newer version of protoc which is"
#error "incompatible with your Protocol Buffer headers. Please update"
#error "your headers."
#endif  // PROTOBUF_VERSION

#if 4025003 < PROTOBUF_MIN_PROTOC_VERSION
#error "This file was generated by an older version of protoc which is"
#error "incompatible with your Protocol Buffer headers. Please"
#error "regenerate this file with a newer version of protoc."
#endif  // PROTOBUF_MIN_PROTOC_VERSION
#include "google/protobuf/port_undef.inc"
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/arena.h"
#include "google/protobuf/arenastring.h"
#include "google/protobuf/generated_message_tctable_decl.h"
#include "google/protobuf/generated_message_util.h"
#include "google/protobuf/metadata_lite.h"
#include "google/protobuf/generated_message_reflection.h"
#include "google/protobuf/message.h"
#include "google/protobuf/repeated_field.h"  // IWYU pragma: export
#include "google/protobuf/extension_set.h"  // IWYU pragma: export
#include "google/protobuf/unknown_field_set.h"
// @@protoc_insertion_point(includes)

// Must be included last.
#include "google/protobuf/port_def.inc"

#define PROTOBUF_INTERNAL_EXPORT_services_2fprofile_5fservice_2fprofile_2eproto

namespace google {
namespace protobuf {
namespace internal {
class AnyMetadata;
}  // namespace internal
}  // namespace protobuf
}  // namespace google

// Internal implementation detail -- do not use these members.
struct TableStruct_services_2fprofile_5fservice_2fprofile_2eproto {
  static const ::uint32_t offsets[];
};
extern const ::google::protobuf::internal::DescriptorTable
    descriptor_table_services_2fprofile_5fservice_2fprofile_2eproto;
namespace profile {
class ProfileReply;
struct ProfileReplyDefaultTypeInternal;
extern ProfileReplyDefaultTypeInternal _ProfileReply_default_instance_;
class ProfileRequest;
struct ProfileRequestDefaultTypeInternal;
extern ProfileRequestDefaultTypeInternal _ProfileRequest_default_instance_;
}  // namespace profile
namespace google {
namespace protobuf {
}  // namespace protobuf
}  // namespace google

namespace profile {

// ===================================================================


// -------------------------------------------------------------------

class ProfileRequest final :
    public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:profile.ProfileRequest) */ {
 public:
  inline ProfileRequest() : ProfileRequest(nullptr) {}
  ~ProfileRequest() override;
  template<typename = void>
  explicit PROTOBUF_CONSTEXPR ProfileRequest(::google::protobuf::internal::ConstantInitialized);

  inline ProfileRequest(const ProfileRequest& from)
      : ProfileRequest(nullptr, from) {}
  ProfileRequest(ProfileRequest&& from) noexcept
    : ProfileRequest() {
    *this = ::std::move(from);
  }

  inline ProfileRequest& operator=(const ProfileRequest& from) {
    CopyFrom(from);
    return *this;
  }
  inline ProfileRequest& operator=(ProfileRequest&& from) noexcept {
    if (this == &from) return *this;
    if (GetArena() == from.GetArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const
      ABSL_ATTRIBUTE_LIFETIME_BOUND {
    return _internal_metadata_.unknown_fields<::google::protobuf::UnknownFieldSet>(::google::protobuf::UnknownFieldSet::default_instance);
  }
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields()
      ABSL_ATTRIBUTE_LIFETIME_BOUND {
    return _internal_metadata_.mutable_unknown_fields<::google::protobuf::UnknownFieldSet>();
  }

  static const ::google::protobuf::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::google::protobuf::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::google::protobuf::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const ProfileRequest& default_instance() {
    return *internal_default_instance();
  }
  static inline const ProfileRequest* internal_default_instance() {
    return reinterpret_cast<const ProfileRequest*>(
               &_ProfileRequest_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(ProfileRequest& a, ProfileRequest& b) {
    a.Swap(&b);
  }
  inline void Swap(ProfileRequest* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetArena() != nullptr &&
        GetArena() == other->GetArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetArena() == other->GetArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::google::protobuf::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(ProfileRequest* other) {
    if (other == this) return;
    ABSL_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  ProfileRequest* New(::google::protobuf::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<ProfileRequest>(arena);
  }
  using ::google::protobuf::Message::CopyFrom;
  void CopyFrom(const ProfileRequest& from);
  using ::google::protobuf::Message::MergeFrom;
  void MergeFrom( const ProfileRequest& from) {
    ProfileRequest::MergeImpl(*this, from);
  }
  private:
  static void MergeImpl(::google::protobuf::Message& to_msg, const ::google::protobuf::Message& from_msg);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  ::size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::google::protobuf::internal::ParseContext* ctx) final;
  ::uint8_t* _InternalSerialize(
      ::uint8_t* target, ::google::protobuf::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const { return _impl_._cached_size_.Get(); }

  private:
  ::google::protobuf::internal::CachedSize* AccessCachedSize() const final;
  void SharedCtor(::google::protobuf::Arena* arena);
  void SharedDtor();
  void InternalSwap(ProfileRequest* other);

  private:
  friend class ::google::protobuf::internal::AnyMetadata;
  static ::absl::string_view FullMessageName() {
    return "profile.ProfileRequest";
  }
  protected:
  explicit ProfileRequest(::google::protobuf::Arena* arena);
  ProfileRequest(::google::protobuf::Arena* arena, const ProfileRequest& from);
  public:

  static const ClassData _class_data_;
  const ::google::protobuf::Message::ClassData*GetClassData() const final;

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kIdFieldNumber = 1,
    kNicknameFieldNumber = 2,
    kPhotoFieldNumber = 3,
    kQuoteFieldNumber = 4,
    kBioFieldNumber = 5,
  };
  // string id = 1;
  void clear_id() ;
  const std::string& id() const;
  template <typename Arg_ = const std::string&, typename... Args_>
  void set_id(Arg_&& arg, Args_... args);
  std::string* mutable_id();
  PROTOBUF_NODISCARD std::string* release_id();
  void set_allocated_id(std::string* value);

  private:
  const std::string& _internal_id() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_id(
      const std::string& value);
  std::string* _internal_mutable_id();

  public:
  // string nickname = 2;
  void clear_nickname() ;
  const std::string& nickname() const;
  template <typename Arg_ = const std::string&, typename... Args_>
  void set_nickname(Arg_&& arg, Args_... args);
  std::string* mutable_nickname();
  PROTOBUF_NODISCARD std::string* release_nickname();
  void set_allocated_nickname(std::string* value);

  private:
  const std::string& _internal_nickname() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_nickname(
      const std::string& value);
  std::string* _internal_mutable_nickname();

  public:
  // string photo = 3;
  void clear_photo() ;
  const std::string& photo() const;
  template <typename Arg_ = const std::string&, typename... Args_>
  void set_photo(Arg_&& arg, Args_... args);
  std::string* mutable_photo();
  PROTOBUF_NODISCARD std::string* release_photo();
  void set_allocated_photo(std::string* value);

  private:
  const std::string& _internal_photo() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_photo(
      const std::string& value);
  std::string* _internal_mutable_photo();

  public:
  // string quote = 4;
  void clear_quote() ;
  const std::string& quote() const;
  template <typename Arg_ = const std::string&, typename... Args_>
  void set_quote(Arg_&& arg, Args_... args);
  std::string* mutable_quote();
  PROTOBUF_NODISCARD std::string* release_quote();
  void set_allocated_quote(std::string* value);

  private:
  const std::string& _internal_quote() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_quote(
      const std::string& value);
  std::string* _internal_mutable_quote();

  public:
  // string bio = 5;
  void clear_bio() ;
  const std::string& bio() const;
  template <typename Arg_ = const std::string&, typename... Args_>
  void set_bio(Arg_&& arg, Args_... args);
  std::string* mutable_bio();
  PROTOBUF_NODISCARD std::string* release_bio();
  void set_allocated_bio(std::string* value);

  private:
  const std::string& _internal_bio() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_bio(
      const std::string& value);
  std::string* _internal_mutable_bio();

  public:
  // @@protoc_insertion_point(class_scope:profile.ProfileRequest)
 private:
  class _Internal;

  friend class ::google::protobuf::internal::TcParser;
  static const ::google::protobuf::internal::TcParseTable<
      3, 5, 0,
      54, 2>
      _table_;
  friend class ::google::protobuf::MessageLite;
  friend class ::google::protobuf::Arena;
  template <typename T>
  friend class ::google::protobuf::Arena::InternalHelper;
  using InternalArenaConstructable_ = void;
  using DestructorSkippable_ = void;
  struct Impl_ {

        inline explicit constexpr Impl_(
            ::google::protobuf::internal::ConstantInitialized) noexcept;
        inline explicit Impl_(::google::protobuf::internal::InternalVisibility visibility,
                              ::google::protobuf::Arena* arena);
        inline explicit Impl_(::google::protobuf::internal::InternalVisibility visibility,
                              ::google::protobuf::Arena* arena, const Impl_& from);
    ::google::protobuf::internal::ArenaStringPtr id_;
    ::google::protobuf::internal::ArenaStringPtr nickname_;
    ::google::protobuf::internal::ArenaStringPtr photo_;
    ::google::protobuf::internal::ArenaStringPtr quote_;
    ::google::protobuf::internal::ArenaStringPtr bio_;
    mutable ::google::protobuf::internal::CachedSize _cached_size_;
    PROTOBUF_TSAN_DECLARE_MEMBER
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_services_2fprofile_5fservice_2fprofile_2eproto;
};// -------------------------------------------------------------------

class ProfileReply final :
    public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:profile.ProfileReply) */ {
 public:
  inline ProfileReply() : ProfileReply(nullptr) {}
  ~ProfileReply() override;
  template<typename = void>
  explicit PROTOBUF_CONSTEXPR ProfileReply(::google::protobuf::internal::ConstantInitialized);

  inline ProfileReply(const ProfileReply& from)
      : ProfileReply(nullptr, from) {}
  ProfileReply(ProfileReply&& from) noexcept
    : ProfileReply() {
    *this = ::std::move(from);
  }

  inline ProfileReply& operator=(const ProfileReply& from) {
    CopyFrom(from);
    return *this;
  }
  inline ProfileReply& operator=(ProfileReply&& from) noexcept {
    if (this == &from) return *this;
    if (GetArena() == from.GetArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const
      ABSL_ATTRIBUTE_LIFETIME_BOUND {
    return _internal_metadata_.unknown_fields<::google::protobuf::UnknownFieldSet>(::google::protobuf::UnknownFieldSet::default_instance);
  }
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields()
      ABSL_ATTRIBUTE_LIFETIME_BOUND {
    return _internal_metadata_.mutable_unknown_fields<::google::protobuf::UnknownFieldSet>();
  }

  static const ::google::protobuf::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::google::protobuf::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::google::protobuf::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const ProfileReply& default_instance() {
    return *internal_default_instance();
  }
  static inline const ProfileReply* internal_default_instance() {
    return reinterpret_cast<const ProfileReply*>(
               &_ProfileReply_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(ProfileReply& a, ProfileReply& b) {
    a.Swap(&b);
  }
  inline void Swap(ProfileReply* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetArena() != nullptr &&
        GetArena() == other->GetArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetArena() == other->GetArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::google::protobuf::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(ProfileReply* other) {
    if (other == this) return;
    ABSL_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  ProfileReply* New(::google::protobuf::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<ProfileReply>(arena);
  }
  using ::google::protobuf::Message::CopyFrom;
  void CopyFrom(const ProfileReply& from);
  using ::google::protobuf::Message::MergeFrom;
  void MergeFrom( const ProfileReply& from) {
    ProfileReply::MergeImpl(*this, from);
  }
  private:
  static void MergeImpl(::google::protobuf::Message& to_msg, const ::google::protobuf::Message& from_msg);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  ::size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::google::protobuf::internal::ParseContext* ctx) final;
  ::uint8_t* _InternalSerialize(
      ::uint8_t* target, ::google::protobuf::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const { return _impl_._cached_size_.Get(); }

  private:
  ::google::protobuf::internal::CachedSize* AccessCachedSize() const final;
  void SharedCtor(::google::protobuf::Arena* arena);
  void SharedDtor();
  void InternalSwap(ProfileReply* other);

  private:
  friend class ::google::protobuf::internal::AnyMetadata;
  static ::absl::string_view FullMessageName() {
    return "profile.ProfileReply";
  }
  protected:
  explicit ProfileReply(::google::protobuf::Arena* arena);
  ProfileReply(::google::protobuf::Arena* arena, const ProfileReply& from);
  public:

  static const ClassData _class_data_;
  const ::google::protobuf::Message::ClassData*GetClassData() const final;

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kMessageFieldNumber = 1,
  };
  // string message = 1;
  void clear_message() ;
  const std::string& message() const;
  template <typename Arg_ = const std::string&, typename... Args_>
  void set_message(Arg_&& arg, Args_... args);
  std::string* mutable_message();
  PROTOBUF_NODISCARD std::string* release_message();
  void set_allocated_message(std::string* value);

  private:
  const std::string& _internal_message() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_message(
      const std::string& value);
  std::string* _internal_mutable_message();

  public:
  // @@protoc_insertion_point(class_scope:profile.ProfileReply)
 private:
  class _Internal;

  friend class ::google::protobuf::internal::TcParser;
  static const ::google::protobuf::internal::TcParseTable<
      0, 1, 0,
      36, 2>
      _table_;
  friend class ::google::protobuf::MessageLite;
  friend class ::google::protobuf::Arena;
  template <typename T>
  friend class ::google::protobuf::Arena::InternalHelper;
  using InternalArenaConstructable_ = void;
  using DestructorSkippable_ = void;
  struct Impl_ {

        inline explicit constexpr Impl_(
            ::google::protobuf::internal::ConstantInitialized) noexcept;
        inline explicit Impl_(::google::protobuf::internal::InternalVisibility visibility,
                              ::google::protobuf::Arena* arena);
        inline explicit Impl_(::google::protobuf::internal::InternalVisibility visibility,
                              ::google::protobuf::Arena* arena, const Impl_& from);
    ::google::protobuf::internal::ArenaStringPtr message_;
    mutable ::google::protobuf::internal::CachedSize _cached_size_;
    PROTOBUF_TSAN_DECLARE_MEMBER
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_services_2fprofile_5fservice_2fprofile_2eproto;
};

// ===================================================================




// ===================================================================


#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// -------------------------------------------------------------------

// ProfileRequest

// string id = 1;
inline void ProfileRequest::clear_id() {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  _impl_.id_.ClearToEmpty();
}
inline const std::string& ProfileRequest::id() const
    ABSL_ATTRIBUTE_LIFETIME_BOUND {
  // @@protoc_insertion_point(field_get:profile.ProfileRequest.id)
  return _internal_id();
}
template <typename Arg_, typename... Args_>
inline PROTOBUF_ALWAYS_INLINE void ProfileRequest::set_id(Arg_&& arg,
                                                     Args_... args) {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  ;
  _impl_.id_.Set(static_cast<Arg_&&>(arg), args..., GetArena());
  // @@protoc_insertion_point(field_set:profile.ProfileRequest.id)
}
inline std::string* ProfileRequest::mutable_id() ABSL_ATTRIBUTE_LIFETIME_BOUND {
  std::string* _s = _internal_mutable_id();
  // @@protoc_insertion_point(field_mutable:profile.ProfileRequest.id)
  return _s;
}
inline const std::string& ProfileRequest::_internal_id() const {
  PROTOBUF_TSAN_READ(&_impl_._tsan_detect_race);
  return _impl_.id_.Get();
}
inline void ProfileRequest::_internal_set_id(const std::string& value) {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  ;
  _impl_.id_.Set(value, GetArena());
}
inline std::string* ProfileRequest::_internal_mutable_id() {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  ;
  return _impl_.id_.Mutable( GetArena());
}
inline std::string* ProfileRequest::release_id() {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  // @@protoc_insertion_point(field_release:profile.ProfileRequest.id)
  return _impl_.id_.Release();
}
inline void ProfileRequest::set_allocated_id(std::string* value) {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  _impl_.id_.SetAllocated(value, GetArena());
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
        if (_impl_.id_.IsDefault()) {
          _impl_.id_.Set("", GetArena());
        }
  #endif  // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:profile.ProfileRequest.id)
}

// string nickname = 2;
inline void ProfileRequest::clear_nickname() {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  _impl_.nickname_.ClearToEmpty();
}
inline const std::string& ProfileRequest::nickname() const
    ABSL_ATTRIBUTE_LIFETIME_BOUND {
  // @@protoc_insertion_point(field_get:profile.ProfileRequest.nickname)
  return _internal_nickname();
}
template <typename Arg_, typename... Args_>
inline PROTOBUF_ALWAYS_INLINE void ProfileRequest::set_nickname(Arg_&& arg,
                                                     Args_... args) {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  ;
  _impl_.nickname_.Set(static_cast<Arg_&&>(arg), args..., GetArena());
  // @@protoc_insertion_point(field_set:profile.ProfileRequest.nickname)
}
inline std::string* ProfileRequest::mutable_nickname() ABSL_ATTRIBUTE_LIFETIME_BOUND {
  std::string* _s = _internal_mutable_nickname();
  // @@protoc_insertion_point(field_mutable:profile.ProfileRequest.nickname)
  return _s;
}
inline const std::string& ProfileRequest::_internal_nickname() const {
  PROTOBUF_TSAN_READ(&_impl_._tsan_detect_race);
  return _impl_.nickname_.Get();
}
inline void ProfileRequest::_internal_set_nickname(const std::string& value) {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  ;
  _impl_.nickname_.Set(value, GetArena());
}
inline std::string* ProfileRequest::_internal_mutable_nickname() {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  ;
  return _impl_.nickname_.Mutable( GetArena());
}
inline std::string* ProfileRequest::release_nickname() {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  // @@protoc_insertion_point(field_release:profile.ProfileRequest.nickname)
  return _impl_.nickname_.Release();
}
inline void ProfileRequest::set_allocated_nickname(std::string* value) {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  _impl_.nickname_.SetAllocated(value, GetArena());
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
        if (_impl_.nickname_.IsDefault()) {
          _impl_.nickname_.Set("", GetArena());
        }
  #endif  // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:profile.ProfileRequest.nickname)
}

// string photo = 3;
inline void ProfileRequest::clear_photo() {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  _impl_.photo_.ClearToEmpty();
}
inline const std::string& ProfileRequest::photo() const
    ABSL_ATTRIBUTE_LIFETIME_BOUND {
  // @@protoc_insertion_point(field_get:profile.ProfileRequest.photo)
  return _internal_photo();
}
template <typename Arg_, typename... Args_>
inline PROTOBUF_ALWAYS_INLINE void ProfileRequest::set_photo(Arg_&& arg,
                                                     Args_... args) {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  ;
  _impl_.photo_.Set(static_cast<Arg_&&>(arg), args..., GetArena());
  // @@protoc_insertion_point(field_set:profile.ProfileRequest.photo)
}
inline std::string* ProfileRequest::mutable_photo() ABSL_ATTRIBUTE_LIFETIME_BOUND {
  std::string* _s = _internal_mutable_photo();
  // @@protoc_insertion_point(field_mutable:profile.ProfileRequest.photo)
  return _s;
}
inline const std::string& ProfileRequest::_internal_photo() const {
  PROTOBUF_TSAN_READ(&_impl_._tsan_detect_race);
  return _impl_.photo_.Get();
}
inline void ProfileRequest::_internal_set_photo(const std::string& value) {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  ;
  _impl_.photo_.Set(value, GetArena());
}
inline std::string* ProfileRequest::_internal_mutable_photo() {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  ;
  return _impl_.photo_.Mutable( GetArena());
}
inline std::string* ProfileRequest::release_photo() {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  // @@protoc_insertion_point(field_release:profile.ProfileRequest.photo)
  return _impl_.photo_.Release();
}
inline void ProfileRequest::set_allocated_photo(std::string* value) {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  _impl_.photo_.SetAllocated(value, GetArena());
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
        if (_impl_.photo_.IsDefault()) {
          _impl_.photo_.Set("", GetArena());
        }
  #endif  // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:profile.ProfileRequest.photo)
}

// string quote = 4;
inline void ProfileRequest::clear_quote() {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  _impl_.quote_.ClearToEmpty();
}
inline const std::string& ProfileRequest::quote() const
    ABSL_ATTRIBUTE_LIFETIME_BOUND {
  // @@protoc_insertion_point(field_get:profile.ProfileRequest.quote)
  return _internal_quote();
}
template <typename Arg_, typename... Args_>
inline PROTOBUF_ALWAYS_INLINE void ProfileRequest::set_quote(Arg_&& arg,
                                                     Args_... args) {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  ;
  _impl_.quote_.Set(static_cast<Arg_&&>(arg), args..., GetArena());
  // @@protoc_insertion_point(field_set:profile.ProfileRequest.quote)
}
inline std::string* ProfileRequest::mutable_quote() ABSL_ATTRIBUTE_LIFETIME_BOUND {
  std::string* _s = _internal_mutable_quote();
  // @@protoc_insertion_point(field_mutable:profile.ProfileRequest.quote)
  return _s;
}
inline const std::string& ProfileRequest::_internal_quote() const {
  PROTOBUF_TSAN_READ(&_impl_._tsan_detect_race);
  return _impl_.quote_.Get();
}
inline void ProfileRequest::_internal_set_quote(const std::string& value) {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  ;
  _impl_.quote_.Set(value, GetArena());
}
inline std::string* ProfileRequest::_internal_mutable_quote() {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  ;
  return _impl_.quote_.Mutable( GetArena());
}
inline std::string* ProfileRequest::release_quote() {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  // @@protoc_insertion_point(field_release:profile.ProfileRequest.quote)
  return _impl_.quote_.Release();
}
inline void ProfileRequest::set_allocated_quote(std::string* value) {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  _impl_.quote_.SetAllocated(value, GetArena());
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
        if (_impl_.quote_.IsDefault()) {
          _impl_.quote_.Set("", GetArena());
        }
  #endif  // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:profile.ProfileRequest.quote)
}

// string bio = 5;
inline void ProfileRequest::clear_bio() {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  _impl_.bio_.ClearToEmpty();
}
inline const std::string& ProfileRequest::bio() const
    ABSL_ATTRIBUTE_LIFETIME_BOUND {
  // @@protoc_insertion_point(field_get:profile.ProfileRequest.bio)
  return _internal_bio();
}
template <typename Arg_, typename... Args_>
inline PROTOBUF_ALWAYS_INLINE void ProfileRequest::set_bio(Arg_&& arg,
                                                     Args_... args) {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  ;
  _impl_.bio_.Set(static_cast<Arg_&&>(arg), args..., GetArena());
  // @@protoc_insertion_point(field_set:profile.ProfileRequest.bio)
}
inline std::string* ProfileRequest::mutable_bio() ABSL_ATTRIBUTE_LIFETIME_BOUND {
  std::string* _s = _internal_mutable_bio();
  // @@protoc_insertion_point(field_mutable:profile.ProfileRequest.bio)
  return _s;
}
inline const std::string& ProfileRequest::_internal_bio() const {
  PROTOBUF_TSAN_READ(&_impl_._tsan_detect_race);
  return _impl_.bio_.Get();
}
inline void ProfileRequest::_internal_set_bio(const std::string& value) {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  ;
  _impl_.bio_.Set(value, GetArena());
}
inline std::string* ProfileRequest::_internal_mutable_bio() {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  ;
  return _impl_.bio_.Mutable( GetArena());
}
inline std::string* ProfileRequest::release_bio() {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  // @@protoc_insertion_point(field_release:profile.ProfileRequest.bio)
  return _impl_.bio_.Release();
}
inline void ProfileRequest::set_allocated_bio(std::string* value) {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  _impl_.bio_.SetAllocated(value, GetArena());
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
        if (_impl_.bio_.IsDefault()) {
          _impl_.bio_.Set("", GetArena());
        }
  #endif  // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:profile.ProfileRequest.bio)
}

// -------------------------------------------------------------------

// ProfileReply

// string message = 1;
inline void ProfileReply::clear_message() {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  _impl_.message_.ClearToEmpty();
}
inline const std::string& ProfileReply::message() const
    ABSL_ATTRIBUTE_LIFETIME_BOUND {
  // @@protoc_insertion_point(field_get:profile.ProfileReply.message)
  return _internal_message();
}
template <typename Arg_, typename... Args_>
inline PROTOBUF_ALWAYS_INLINE void ProfileReply::set_message(Arg_&& arg,
                                                     Args_... args) {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  ;
  _impl_.message_.Set(static_cast<Arg_&&>(arg), args..., GetArena());
  // @@protoc_insertion_point(field_set:profile.ProfileReply.message)
}
inline std::string* ProfileReply::mutable_message() ABSL_ATTRIBUTE_LIFETIME_BOUND {
  std::string* _s = _internal_mutable_message();
  // @@protoc_insertion_point(field_mutable:profile.ProfileReply.message)
  return _s;
}
inline const std::string& ProfileReply::_internal_message() const {
  PROTOBUF_TSAN_READ(&_impl_._tsan_detect_race);
  return _impl_.message_.Get();
}
inline void ProfileReply::_internal_set_message(const std::string& value) {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  ;
  _impl_.message_.Set(value, GetArena());
}
inline std::string* ProfileReply::_internal_mutable_message() {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  ;
  return _impl_.message_.Mutable( GetArena());
}
inline std::string* ProfileReply::release_message() {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  // @@protoc_insertion_point(field_release:profile.ProfileReply.message)
  return _impl_.message_.Release();
}
inline void ProfileReply::set_allocated_message(std::string* value) {
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  _impl_.message_.SetAllocated(value, GetArena());
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
        if (_impl_.message_.IsDefault()) {
          _impl_.message_.Set("", GetArena());
        }
  #endif  // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:profile.ProfileReply.message)
}

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)
}  // namespace profile


// @@protoc_insertion_point(global_scope)

#include "google/protobuf/port_undef.inc"

#endif  // GOOGLE_PROTOBUF_INCLUDED_services_2fprofile_5fservice_2fprofile_2eproto_2epb_2eh
