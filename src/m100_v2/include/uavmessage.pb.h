// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: uavmessage.proto

#ifndef PROTOBUF_uavmessage_2eproto__INCLUDED
#define PROTOBUF_uavmessage_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3004000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3004000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
namespace uavMessage {
class Message;
class MessageDefaultTypeInternal;
extern MessageDefaultTypeInternal _Message_default_instance_;
class MsgHead;
class MsgHeadDefaultTypeInternal;
extern MsgHeadDefaultTypeInternal _MsgHead_default_instance_;
}  // namespace uavMessage

namespace uavMessage {

namespace protobuf_uavmessage_2eproto {
// Internal implementation detail -- do not call these.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[];
  static const ::google::protobuf::uint32 offsets[];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static void InitDefaultsImpl();
};
void AddDescriptors();
void InitDefaults();
}  // namespace protobuf_uavmessage_2eproto

// ===================================================================

class MsgHead : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:uavMessage.MsgHead) */ {
 public:
  MsgHead();
  virtual ~MsgHead();

  MsgHead(const MsgHead& from);

  inline MsgHead& operator=(const MsgHead& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  MsgHead(MsgHead&& from) noexcept
    : MsgHead() {
    *this = ::std::move(from);
  }

  inline MsgHead& operator=(MsgHead&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const MsgHead& default_instance();

  static inline const MsgHead* internal_default_instance() {
    return reinterpret_cast<const MsgHead*>(
               &_MsgHead_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    0;

  void Swap(MsgHead* other);
  friend void swap(MsgHead& a, MsgHead& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline MsgHead* New() const PROTOBUF_FINAL { return New(NULL); }

  MsgHead* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const MsgHead& from);
  void MergeFrom(const MsgHead& from);
  void Clear() PROTOBUF_FINAL;
  bool IsInitialized() const PROTOBUF_FINAL;

  size_t ByteSizeLong() const PROTOBUF_FINAL;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) PROTOBUF_FINAL;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const PROTOBUF_FINAL;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const PROTOBUF_FINAL;
  int GetCachedSize() const PROTOBUF_FINAL { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const PROTOBUF_FINAL;
  void InternalSwap(MsgHead* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const PROTOBUF_FINAL;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // repeated uint32 tgt_uav_id = 10;
  int tgt_uav_id_size() const;
  void clear_tgt_uav_id();
  static const int kTgtUavIdFieldNumber = 10;
  ::google::protobuf::uint32 tgt_uav_id(int index) const;
  void set_tgt_uav_id(int index, ::google::protobuf::uint32 value);
  void add_tgt_uav_id(::google::protobuf::uint32 value);
  const ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >&
      tgt_uav_id() const;
  ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >*
      mutable_tgt_uav_id();

  // uint32 stx = 1;
  void clear_stx();
  static const int kStxFieldNumber = 1;
  ::google::protobuf::uint32 stx() const;
  void set_stx(::google::protobuf::uint32 value);

  // uint32 msg_type = 2;
  void clear_msg_type();
  static const int kMsgTypeFieldNumber = 2;
  ::google::protobuf::uint32 msg_type() const;
  void set_msg_type(::google::protobuf::uint32 value);

  // uint32 cluster_id = 3;
  void clear_cluster_id();
  static const int kClusterIdFieldNumber = 3;
  ::google::protobuf::uint32 cluster_id() const;
  void set_cluster_id(::google::protobuf::uint32 value);

  // uint32 src_uav_id = 4;
  void clear_src_uav_id();
  static const int kSrcUavIdFieldNumber = 4;
  ::google::protobuf::uint32 src_uav_id() const;
  void set_src_uav_id(::google::protobuf::uint32 value);

  // uint32 tgt_uav_count = 5;
  void clear_tgt_uav_count();
  static const int kTgtUavCountFieldNumber = 5;
  ::google::protobuf::uint32 tgt_uav_count() const;
  void set_tgt_uav_count(::google::protobuf::uint32 value);

  // uint32 topic_id = 6;
  void clear_topic_id();
  static const int kTopicIdFieldNumber = 6;
  ::google::protobuf::uint32 topic_id() const;
  void set_topic_id(::google::protobuf::uint32 value);

  // uint32 msg_id = 7;
  void clear_msg_id();
  static const int kMsgIdFieldNumber = 7;
  ::google::protobuf::uint32 msg_id() const;
  void set_msg_id(::google::protobuf::uint32 value);

  // uint32 msg_length = 8;
  void clear_msg_length();
  static const int kMsgLengthFieldNumber = 8;
  ::google::protobuf::uint32 msg_length() const;
  void set_msg_length(::google::protobuf::uint32 value);

  // uint32 seq_num = 9;
  void clear_seq_num();
  static const int kSeqNumFieldNumber = 9;
  ::google::protobuf::uint32 seq_num() const;
  void set_seq_num(::google::protobuf::uint32 value);

  // uint32 reserved = 20;
  void clear_reserved();
  static const int kReservedFieldNumber = 20;
  ::google::protobuf::uint32 reserved() const;
  void set_reserved(::google::protobuf::uint32 value);

  // double time_stamp = 19;
  void clear_time_stamp();
  static const int kTimeStampFieldNumber = 19;
  double time_stamp() const;
  void set_time_stamp(double value);

  // @@protoc_insertion_point(class_scope:uavMessage.MsgHead)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::RepeatedField< ::google::protobuf::uint32 > tgt_uav_id_;
  mutable int _tgt_uav_id_cached_byte_size_;
  ::google::protobuf::uint32 stx_;
  ::google::protobuf::uint32 msg_type_;
  ::google::protobuf::uint32 cluster_id_;
  ::google::protobuf::uint32 src_uav_id_;
  ::google::protobuf::uint32 tgt_uav_count_;
  ::google::protobuf::uint32 topic_id_;
  ::google::protobuf::uint32 msg_id_;
  ::google::protobuf::uint32 msg_length_;
  ::google::protobuf::uint32 seq_num_;
  ::google::protobuf::uint32 reserved_;
  double time_stamp_;
  mutable int _cached_size_;
  friend struct protobuf_uavmessage_2eproto::TableStruct;
};
// -------------------------------------------------------------------

class Message : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:uavMessage.Message) */ {
 public:
  Message();
  virtual ~Message();

  Message(const Message& from);

  inline Message& operator=(const Message& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  Message(Message&& from) noexcept
    : Message() {
    *this = ::std::move(from);
  }

  inline Message& operator=(Message&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const Message& default_instance();

  static inline const Message* internal_default_instance() {
    return reinterpret_cast<const Message*>(
               &_Message_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    1;

  void Swap(Message* other);
  friend void swap(Message& a, Message& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline Message* New() const PROTOBUF_FINAL { return New(NULL); }

  Message* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const Message& from);
  void MergeFrom(const Message& from);
  void Clear() PROTOBUF_FINAL;
  bool IsInitialized() const PROTOBUF_FINAL;

  size_t ByteSizeLong() const PROTOBUF_FINAL;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) PROTOBUF_FINAL;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const PROTOBUF_FINAL;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const PROTOBUF_FINAL;
  int GetCachedSize() const PROTOBUF_FINAL { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const PROTOBUF_FINAL;
  void InternalSwap(Message* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const PROTOBUF_FINAL;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // bytes playload = 2;
  void clear_playload();
  static const int kPlayloadFieldNumber = 2;
  const ::std::string& playload() const;
  void set_playload(const ::std::string& value);
  #if LANG_CXX11
  void set_playload(::std::string&& value);
  #endif
  void set_playload(const char* value);
  void set_playload(const void* value, size_t size);
  ::std::string* mutable_playload();
  ::std::string* release_playload();
  void set_allocated_playload(::std::string* playload);

  // .uavMessage.MsgHead msghead = 1;
  bool has_msghead() const;
  void clear_msghead();
  static const int kMsgheadFieldNumber = 1;
  const ::uavMessage::MsgHead& msghead() const;
  ::uavMessage::MsgHead* mutable_msghead();
  ::uavMessage::MsgHead* release_msghead();
  void set_allocated_msghead(::uavMessage::MsgHead* msghead);

  // @@protoc_insertion_point(class_scope:uavMessage.Message)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::ArenaStringPtr playload_;
  ::uavMessage::MsgHead* msghead_;
  mutable int _cached_size_;
  friend struct protobuf_uavmessage_2eproto::TableStruct;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// MsgHead

// uint32 stx = 1;
inline void MsgHead::clear_stx() {
  stx_ = 0u;
}
inline ::google::protobuf::uint32 MsgHead::stx() const {
  // @@protoc_insertion_point(field_get:uavMessage.MsgHead.stx)
  return stx_;
}
inline void MsgHead::set_stx(::google::protobuf::uint32 value) {
  
  stx_ = value;
  // @@protoc_insertion_point(field_set:uavMessage.MsgHead.stx)
}

// uint32 msg_type = 2;
inline void MsgHead::clear_msg_type() {
  msg_type_ = 0u;
}
inline ::google::protobuf::uint32 MsgHead::msg_type() const {
  // @@protoc_insertion_point(field_get:uavMessage.MsgHead.msg_type)
  return msg_type_;
}
inline void MsgHead::set_msg_type(::google::protobuf::uint32 value) {
  
  msg_type_ = value;
  // @@protoc_insertion_point(field_set:uavMessage.MsgHead.msg_type)
}

// uint32 cluster_id = 3;
inline void MsgHead::clear_cluster_id() {
  cluster_id_ = 0u;
}
inline ::google::protobuf::uint32 MsgHead::cluster_id() const {
  // @@protoc_insertion_point(field_get:uavMessage.MsgHead.cluster_id)
  return cluster_id_;
}
inline void MsgHead::set_cluster_id(::google::protobuf::uint32 value) {
  
  cluster_id_ = value;
  // @@protoc_insertion_point(field_set:uavMessage.MsgHead.cluster_id)
}

// uint32 src_uav_id = 4;
inline void MsgHead::clear_src_uav_id() {
  src_uav_id_ = 0u;
}
inline ::google::protobuf::uint32 MsgHead::src_uav_id() const {
  // @@protoc_insertion_point(field_get:uavMessage.MsgHead.src_uav_id)
  return src_uav_id_;
}
inline void MsgHead::set_src_uav_id(::google::protobuf::uint32 value) {
  
  src_uav_id_ = value;
  // @@protoc_insertion_point(field_set:uavMessage.MsgHead.src_uav_id)
}

// uint32 tgt_uav_count = 5;
inline void MsgHead::clear_tgt_uav_count() {
  tgt_uav_count_ = 0u;
}
inline ::google::protobuf::uint32 MsgHead::tgt_uav_count() const {
  // @@protoc_insertion_point(field_get:uavMessage.MsgHead.tgt_uav_count)
  return tgt_uav_count_;
}
inline void MsgHead::set_tgt_uav_count(::google::protobuf::uint32 value) {
  
  tgt_uav_count_ = value;
  // @@protoc_insertion_point(field_set:uavMessage.MsgHead.tgt_uav_count)
}

// uint32 topic_id = 6;
inline void MsgHead::clear_topic_id() {
  topic_id_ = 0u;
}
inline ::google::protobuf::uint32 MsgHead::topic_id() const {
  // @@protoc_insertion_point(field_get:uavMessage.MsgHead.topic_id)
  return topic_id_;
}
inline void MsgHead::set_topic_id(::google::protobuf::uint32 value) {
  
  topic_id_ = value;
  // @@protoc_insertion_point(field_set:uavMessage.MsgHead.topic_id)
}

// uint32 msg_id = 7;
inline void MsgHead::clear_msg_id() {
  msg_id_ = 0u;
}
inline ::google::protobuf::uint32 MsgHead::msg_id() const {
  // @@protoc_insertion_point(field_get:uavMessage.MsgHead.msg_id)
  return msg_id_;
}
inline void MsgHead::set_msg_id(::google::protobuf::uint32 value) {
  
  msg_id_ = value;
  // @@protoc_insertion_point(field_set:uavMessage.MsgHead.msg_id)
}

// uint32 msg_length = 8;
inline void MsgHead::clear_msg_length() {
  msg_length_ = 0u;
}
inline ::google::protobuf::uint32 MsgHead::msg_length() const {
  // @@protoc_insertion_point(field_get:uavMessage.MsgHead.msg_length)
  return msg_length_;
}
inline void MsgHead::set_msg_length(::google::protobuf::uint32 value) {
  
  msg_length_ = value;
  // @@protoc_insertion_point(field_set:uavMessage.MsgHead.msg_length)
}

// uint32 seq_num = 9;
inline void MsgHead::clear_seq_num() {
  seq_num_ = 0u;
}
inline ::google::protobuf::uint32 MsgHead::seq_num() const {
  // @@protoc_insertion_point(field_get:uavMessage.MsgHead.seq_num)
  return seq_num_;
}
inline void MsgHead::set_seq_num(::google::protobuf::uint32 value) {
  
  seq_num_ = value;
  // @@protoc_insertion_point(field_set:uavMessage.MsgHead.seq_num)
}

// repeated uint32 tgt_uav_id = 10;
inline int MsgHead::tgt_uav_id_size() const {
  return tgt_uav_id_.size();
}
inline void MsgHead::clear_tgt_uav_id() {
  tgt_uav_id_.Clear();
}
inline ::google::protobuf::uint32 MsgHead::tgt_uav_id(int index) const {
  // @@protoc_insertion_point(field_get:uavMessage.MsgHead.tgt_uav_id)
  return tgt_uav_id_.Get(index);
}
inline void MsgHead::set_tgt_uav_id(int index, ::google::protobuf::uint32 value) {
  tgt_uav_id_.Set(index, value);
  // @@protoc_insertion_point(field_set:uavMessage.MsgHead.tgt_uav_id)
}
inline void MsgHead::add_tgt_uav_id(::google::protobuf::uint32 value) {
  tgt_uav_id_.Add(value);
  // @@protoc_insertion_point(field_add:uavMessage.MsgHead.tgt_uav_id)
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >&
MsgHead::tgt_uav_id() const {
  // @@protoc_insertion_point(field_list:uavMessage.MsgHead.tgt_uav_id)
  return tgt_uav_id_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >*
MsgHead::mutable_tgt_uav_id() {
  // @@protoc_insertion_point(field_mutable_list:uavMessage.MsgHead.tgt_uav_id)
  return &tgt_uav_id_;
}

// double time_stamp = 19;
inline void MsgHead::clear_time_stamp() {
  time_stamp_ = 0;
}
inline double MsgHead::time_stamp() const {
  // @@protoc_insertion_point(field_get:uavMessage.MsgHead.time_stamp)
  return time_stamp_;
}
inline void MsgHead::set_time_stamp(double value) {
  
  time_stamp_ = value;
  // @@protoc_insertion_point(field_set:uavMessage.MsgHead.time_stamp)
}

// uint32 reserved = 20;
inline void MsgHead::clear_reserved() {
  reserved_ = 0u;
}
inline ::google::protobuf::uint32 MsgHead::reserved() const {
  // @@protoc_insertion_point(field_get:uavMessage.MsgHead.reserved)
  return reserved_;
}
inline void MsgHead::set_reserved(::google::protobuf::uint32 value) {
  
  reserved_ = value;
  // @@protoc_insertion_point(field_set:uavMessage.MsgHead.reserved)
}

// -------------------------------------------------------------------

// Message

// .uavMessage.MsgHead msghead = 1;
inline bool Message::has_msghead() const {
  return this != internal_default_instance() && msghead_ != NULL;
}
inline void Message::clear_msghead() {
  if (GetArenaNoVirtual() == NULL && msghead_ != NULL) delete msghead_;
  msghead_ = NULL;
}
inline const ::uavMessage::MsgHead& Message::msghead() const {
  const ::uavMessage::MsgHead* p = msghead_;
  // @@protoc_insertion_point(field_get:uavMessage.Message.msghead)
  return p != NULL ? *p : *reinterpret_cast<const ::uavMessage::MsgHead*>(
      &::uavMessage::_MsgHead_default_instance_);
}
inline ::uavMessage::MsgHead* Message::mutable_msghead() {
  
  if (msghead_ == NULL) {
    msghead_ = new ::uavMessage::MsgHead;
  }
  // @@protoc_insertion_point(field_mutable:uavMessage.Message.msghead)
  return msghead_;
}
inline ::uavMessage::MsgHead* Message::release_msghead() {
  // @@protoc_insertion_point(field_release:uavMessage.Message.msghead)
  
  ::uavMessage::MsgHead* temp = msghead_;
  msghead_ = NULL;
  return temp;
}
inline void Message::set_allocated_msghead(::uavMessage::MsgHead* msghead) {
  delete msghead_;
  msghead_ = msghead;
  if (msghead) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_set_allocated:uavMessage.Message.msghead)
}

// bytes playload = 2;
inline void Message::clear_playload() {
  playload_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& Message::playload() const {
  // @@protoc_insertion_point(field_get:uavMessage.Message.playload)
  return playload_.GetNoArena();
}
inline void Message::set_playload(const ::std::string& value) {
  
  playload_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:uavMessage.Message.playload)
}
#if LANG_CXX11
inline void Message::set_playload(::std::string&& value) {
  
  playload_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:uavMessage.Message.playload)
}
#endif
inline void Message::set_playload(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  playload_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:uavMessage.Message.playload)
}
inline void Message::set_playload(const void* value, size_t size) {
  
  playload_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:uavMessage.Message.playload)
}
inline ::std::string* Message::mutable_playload() {
  
  // @@protoc_insertion_point(field_mutable:uavMessage.Message.playload)
  return playload_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* Message::release_playload() {
  // @@protoc_insertion_point(field_release:uavMessage.Message.playload)
  
  return playload_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void Message::set_allocated_playload(::std::string* playload) {
  if (playload != NULL) {
    
  } else {
    
  }
  playload_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), playload);
  // @@protoc_insertion_point(field_set_allocated:uavMessage.Message.playload)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)


}  // namespace uavMessage

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_uavmessage_2eproto__INCLUDED