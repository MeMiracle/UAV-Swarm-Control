// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: taskmessage.proto

#ifndef PROTOBUF_taskmessage_2eproto__INCLUDED
#define PROTOBUF_taskmessage_2eproto__INCLUDED

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
namespace myTaskMessage {
class MissionMessage;
class MissionMessageDefaultTypeInternal;
extern MissionMessageDefaultTypeInternal _MissionMessage_default_instance_;
class MyPoint;
class MyPointDefaultTypeInternal;
extern MyPointDefaultTypeInternal _MyPoint_default_instance_;
class TaskMessage;
class TaskMessageDefaultTypeInternal;
extern TaskMessageDefaultTypeInternal _TaskMessage_default_instance_;
}  // namespace myTaskMessage

namespace myTaskMessage {

namespace protobuf_taskmessage_2eproto {
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
}  // namespace protobuf_taskmessage_2eproto

// ===================================================================

class MyPoint : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:myTaskMessage.MyPoint) */ {
 public:
  MyPoint();
  virtual ~MyPoint();

  MyPoint(const MyPoint& from);

  inline MyPoint& operator=(const MyPoint& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  MyPoint(MyPoint&& from) noexcept
    : MyPoint() {
    *this = ::std::move(from);
  }

  inline MyPoint& operator=(MyPoint&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const MyPoint& default_instance();

  static inline const MyPoint* internal_default_instance() {
    return reinterpret_cast<const MyPoint*>(
               &_MyPoint_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    0;

  void Swap(MyPoint* other);
  friend void swap(MyPoint& a, MyPoint& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline MyPoint* New() const PROTOBUF_FINAL { return New(NULL); }

  MyPoint* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const MyPoint& from);
  void MergeFrom(const MyPoint& from);
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
  void InternalSwap(MyPoint* other);
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

  // double x = 1;
  void clear_x();
  static const int kXFieldNumber = 1;
  double x() const;
  void set_x(double value);

  // double y = 2;
  void clear_y();
  static const int kYFieldNumber = 2;
  double y() const;
  void set_y(double value);

  // double z = 3;
  void clear_z();
  static const int kZFieldNumber = 3;
  double z() const;
  void set_z(double value);

  // double yaw = 4;
  void clear_yaw();
  static const int kYawFieldNumber = 4;
  double yaw() const;
  void set_yaw(double value);

  // uint32 type = 5;
  void clear_type();
  static const int kTypeFieldNumber = 5;
  ::google::protobuf::uint32 type() const;
  void set_type(::google::protobuf::uint32 value);

  // uint32 mark = 6;
  void clear_mark();
  static const int kMarkFieldNumber = 6;
  ::google::protobuf::uint32 mark() const;
  void set_mark(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:myTaskMessage.MyPoint)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  double x_;
  double y_;
  double z_;
  double yaw_;
  ::google::protobuf::uint32 type_;
  ::google::protobuf::uint32 mark_;
  mutable int _cached_size_;
  friend struct protobuf_taskmessage_2eproto::TableStruct;
};
// -------------------------------------------------------------------

class TaskMessage : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:myTaskMessage.TaskMessage) */ {
 public:
  TaskMessage();
  virtual ~TaskMessage();

  TaskMessage(const TaskMessage& from);

  inline TaskMessage& operator=(const TaskMessage& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  TaskMessage(TaskMessage&& from) noexcept
    : TaskMessage() {
    *this = ::std::move(from);
  }

  inline TaskMessage& operator=(TaskMessage&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const TaskMessage& default_instance();

  static inline const TaskMessage* internal_default_instance() {
    return reinterpret_cast<const TaskMessage*>(
               &_TaskMessage_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    1;

  void Swap(TaskMessage* other);
  friend void swap(TaskMessage& a, TaskMessage& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline TaskMessage* New() const PROTOBUF_FINAL { return New(NULL); }

  TaskMessage* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const TaskMessage& from);
  void MergeFrom(const TaskMessage& from);
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
  void InternalSwap(TaskMessage* other);
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

  // repeated .myTaskMessage.MyPoint point = 1;
  int point_size() const;
  void clear_point();
  static const int kPointFieldNumber = 1;
  const ::myTaskMessage::MyPoint& point(int index) const;
  ::myTaskMessage::MyPoint* mutable_point(int index);
  ::myTaskMessage::MyPoint* add_point();
  ::google::protobuf::RepeatedPtrField< ::myTaskMessage::MyPoint >*
      mutable_point();
  const ::google::protobuf::RepeatedPtrField< ::myTaskMessage::MyPoint >&
      point() const;

  // uint32 clusterid = 2;
  void clear_clusterid();
  static const int kClusteridFieldNumber = 2;
  ::google::protobuf::uint32 clusterid() const;
  void set_clusterid(::google::protobuf::uint32 value);

  // uint32 kind = 3;
  void clear_kind();
  static const int kKindFieldNumber = 3;
  ::google::protobuf::uint32 kind() const;
  void set_kind(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:myTaskMessage.TaskMessage)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::RepeatedPtrField< ::myTaskMessage::MyPoint > point_;
  ::google::protobuf::uint32 clusterid_;
  ::google::protobuf::uint32 kind_;
  mutable int _cached_size_;
  friend struct protobuf_taskmessage_2eproto::TableStruct;
};
// -------------------------------------------------------------------

class MissionMessage : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:myTaskMessage.MissionMessage) */ {
 public:
  MissionMessage();
  virtual ~MissionMessage();

  MissionMessage(const MissionMessage& from);

  inline MissionMessage& operator=(const MissionMessage& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  MissionMessage(MissionMessage&& from) noexcept
    : MissionMessage() {
    *this = ::std::move(from);
  }

  inline MissionMessage& operator=(MissionMessage&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const MissionMessage& default_instance();

  static inline const MissionMessage* internal_default_instance() {
    return reinterpret_cast<const MissionMessage*>(
               &_MissionMessage_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    2;

  void Swap(MissionMessage* other);
  friend void swap(MissionMessage& a, MissionMessage& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline MissionMessage* New() const PROTOBUF_FINAL { return New(NULL); }

  MissionMessage* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const MissionMessage& from);
  void MergeFrom(const MissionMessage& from);
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
  void InternalSwap(MissionMessage* other);
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

  // repeated uint32 uavids = 1;
  int uavids_size() const;
  void clear_uavids();
  static const int kUavidsFieldNumber = 1;
  ::google::protobuf::uint32 uavids(int index) const;
  void set_uavids(int index, ::google::protobuf::uint32 value);
  void add_uavids(::google::protobuf::uint32 value);
  const ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >&
      uavids() const;
  ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >*
      mutable_uavids();

  // .myTaskMessage.TaskMessage task = 2;
  bool has_task() const;
  void clear_task();
  static const int kTaskFieldNumber = 2;
  const ::myTaskMessage::TaskMessage& task() const;
  ::myTaskMessage::TaskMessage* mutable_task();
  ::myTaskMessage::TaskMessage* release_task();
  void set_allocated_task(::myTaskMessage::TaskMessage* task);

  // @@protoc_insertion_point(class_scope:myTaskMessage.MissionMessage)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::RepeatedField< ::google::protobuf::uint32 > uavids_;
  mutable int _uavids_cached_byte_size_;
  ::myTaskMessage::TaskMessage* task_;
  mutable int _cached_size_;
  friend struct protobuf_taskmessage_2eproto::TableStruct;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// MyPoint

// double x = 1;
inline void MyPoint::clear_x() {
  x_ = 0;
}
inline double MyPoint::x() const {
  // @@protoc_insertion_point(field_get:myTaskMessage.MyPoint.x)
  return x_;
}
inline void MyPoint::set_x(double value) {
  
  x_ = value;
  // @@protoc_insertion_point(field_set:myTaskMessage.MyPoint.x)
}

// double y = 2;
inline void MyPoint::clear_y() {
  y_ = 0;
}
inline double MyPoint::y() const {
  // @@protoc_insertion_point(field_get:myTaskMessage.MyPoint.y)
  return y_;
}
inline void MyPoint::set_y(double value) {
  
  y_ = value;
  // @@protoc_insertion_point(field_set:myTaskMessage.MyPoint.y)
}

// double z = 3;
inline void MyPoint::clear_z() {
  z_ = 0;
}
inline double MyPoint::z() const {
  // @@protoc_insertion_point(field_get:myTaskMessage.MyPoint.z)
  return z_;
}
inline void MyPoint::set_z(double value) {
  
  z_ = value;
  // @@protoc_insertion_point(field_set:myTaskMessage.MyPoint.z)
}

// double yaw = 4;
inline void MyPoint::clear_yaw() {
  yaw_ = 0;
}
inline double MyPoint::yaw() const {
  // @@protoc_insertion_point(field_get:myTaskMessage.MyPoint.yaw)
  return yaw_;
}
inline void MyPoint::set_yaw(double value) {
  
  yaw_ = value;
  // @@protoc_insertion_point(field_set:myTaskMessage.MyPoint.yaw)
}

// uint32 type = 5;
inline void MyPoint::clear_type() {
  type_ = 0u;
}
inline ::google::protobuf::uint32 MyPoint::type() const {
  // @@protoc_insertion_point(field_get:myTaskMessage.MyPoint.type)
  return type_;
}
inline void MyPoint::set_type(::google::protobuf::uint32 value) {
  
  type_ = value;
  // @@protoc_insertion_point(field_set:myTaskMessage.MyPoint.type)
}

// uint32 mark = 6;
inline void MyPoint::clear_mark() {
  mark_ = 0u;
}
inline ::google::protobuf::uint32 MyPoint::mark() const {
  // @@protoc_insertion_point(field_get:myTaskMessage.MyPoint.mark)
  return mark_;
}
inline void MyPoint::set_mark(::google::protobuf::uint32 value) {
  
  mark_ = value;
  // @@protoc_insertion_point(field_set:myTaskMessage.MyPoint.mark)
}

// -------------------------------------------------------------------

// TaskMessage

// repeated .myTaskMessage.MyPoint point = 1;
inline int TaskMessage::point_size() const {
  return point_.size();
}
inline void TaskMessage::clear_point() {
  point_.Clear();
}
inline const ::myTaskMessage::MyPoint& TaskMessage::point(int index) const {
  // @@protoc_insertion_point(field_get:myTaskMessage.TaskMessage.point)
  return point_.Get(index);
}
inline ::myTaskMessage::MyPoint* TaskMessage::mutable_point(int index) {
  // @@protoc_insertion_point(field_mutable:myTaskMessage.TaskMessage.point)
  return point_.Mutable(index);
}
inline ::myTaskMessage::MyPoint* TaskMessage::add_point() {
  // @@protoc_insertion_point(field_add:myTaskMessage.TaskMessage.point)
  return point_.Add();
}
inline ::google::protobuf::RepeatedPtrField< ::myTaskMessage::MyPoint >*
TaskMessage::mutable_point() {
  // @@protoc_insertion_point(field_mutable_list:myTaskMessage.TaskMessage.point)
  return &point_;
}
inline const ::google::protobuf::RepeatedPtrField< ::myTaskMessage::MyPoint >&
TaskMessage::point() const {
  // @@protoc_insertion_point(field_list:myTaskMessage.TaskMessage.point)
  return point_;
}

// uint32 clusterid = 2;
inline void TaskMessage::clear_clusterid() {
  clusterid_ = 0u;
}
inline ::google::protobuf::uint32 TaskMessage::clusterid() const {
  // @@protoc_insertion_point(field_get:myTaskMessage.TaskMessage.clusterid)
  return clusterid_;
}
inline void TaskMessage::set_clusterid(::google::protobuf::uint32 value) {
  
  clusterid_ = value;
  // @@protoc_insertion_point(field_set:myTaskMessage.TaskMessage.clusterid)
}

// uint32 kind = 3;
inline void TaskMessage::clear_kind() {
  kind_ = 0u;
}
inline ::google::protobuf::uint32 TaskMessage::kind() const {
  // @@protoc_insertion_point(field_get:myTaskMessage.TaskMessage.kind)
  return kind_;
}
inline void TaskMessage::set_kind(::google::protobuf::uint32 value) {
  
  kind_ = value;
  // @@protoc_insertion_point(field_set:myTaskMessage.TaskMessage.kind)
}

// -------------------------------------------------------------------

// MissionMessage

// repeated uint32 uavids = 1;
inline int MissionMessage::uavids_size() const {
  return uavids_.size();
}
inline void MissionMessage::clear_uavids() {
  uavids_.Clear();
}
inline ::google::protobuf::uint32 MissionMessage::uavids(int index) const {
  // @@protoc_insertion_point(field_get:myTaskMessage.MissionMessage.uavids)
  return uavids_.Get(index);
}
inline void MissionMessage::set_uavids(int index, ::google::protobuf::uint32 value) {
  uavids_.Set(index, value);
  // @@protoc_insertion_point(field_set:myTaskMessage.MissionMessage.uavids)
}
inline void MissionMessage::add_uavids(::google::protobuf::uint32 value) {
  uavids_.Add(value);
  // @@protoc_insertion_point(field_add:myTaskMessage.MissionMessage.uavids)
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >&
MissionMessage::uavids() const {
  // @@protoc_insertion_point(field_list:myTaskMessage.MissionMessage.uavids)
  return uavids_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >*
MissionMessage::mutable_uavids() {
  // @@protoc_insertion_point(field_mutable_list:myTaskMessage.MissionMessage.uavids)
  return &uavids_;
}

// .myTaskMessage.TaskMessage task = 2;
inline bool MissionMessage::has_task() const {
  return this != internal_default_instance() && task_ != NULL;
}
inline void MissionMessage::clear_task() {
  if (GetArenaNoVirtual() == NULL && task_ != NULL) delete task_;
  task_ = NULL;
}
inline const ::myTaskMessage::TaskMessage& MissionMessage::task() const {
  const ::myTaskMessage::TaskMessage* p = task_;
  // @@protoc_insertion_point(field_get:myTaskMessage.MissionMessage.task)
  return p != NULL ? *p : *reinterpret_cast<const ::myTaskMessage::TaskMessage*>(
      &::myTaskMessage::_TaskMessage_default_instance_);
}
inline ::myTaskMessage::TaskMessage* MissionMessage::mutable_task() {
  
  if (task_ == NULL) {
    task_ = new ::myTaskMessage::TaskMessage;
  }
  // @@protoc_insertion_point(field_mutable:myTaskMessage.MissionMessage.task)
  return task_;
}
inline ::myTaskMessage::TaskMessage* MissionMessage::release_task() {
  // @@protoc_insertion_point(field_release:myTaskMessage.MissionMessage.task)
  
  ::myTaskMessage::TaskMessage* temp = task_;
  task_ = NULL;
  return temp;
}
inline void MissionMessage::set_allocated_task(::myTaskMessage::TaskMessage* task) {
  delete task_;
  task_ = task;
  if (task) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_set_allocated:myTaskMessage.MissionMessage.task)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)


}  // namespace myTaskMessage

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_taskmessage_2eproto__INCLUDED
