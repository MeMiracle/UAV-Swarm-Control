// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: feedbackdata.proto

#ifndef PROTOBUF_feedbackdata_2eproto__INCLUDED
#define PROTOBUF_feedbackdata_2eproto__INCLUDED

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
namespace feedbackData {
class FeedbackData;
class FeedbackDataDefaultTypeInternal;
extern FeedbackDataDefaultTypeInternal _FeedbackData_default_instance_;
}  // namespace feedbackData

namespace feedbackData {

namespace protobuf_feedbackdata_2eproto {
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
}  // namespace protobuf_feedbackdata_2eproto

// ===================================================================

class FeedbackData : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:feedbackData.FeedbackData) */ {
 public:
  FeedbackData();
  virtual ~FeedbackData();

  FeedbackData(const FeedbackData& from);

  inline FeedbackData& operator=(const FeedbackData& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  FeedbackData(FeedbackData&& from) noexcept
    : FeedbackData() {
    *this = ::std::move(from);
  }

  inline FeedbackData& operator=(FeedbackData&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const FeedbackData& default_instance();

  static inline const FeedbackData* internal_default_instance() {
    return reinterpret_cast<const FeedbackData*>(
               &_FeedbackData_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    0;

  void Swap(FeedbackData* other);
  friend void swap(FeedbackData& a, FeedbackData& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline FeedbackData* New() const PROTOBUF_FINAL { return New(NULL); }

  FeedbackData* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const FeedbackData& from);
  void MergeFrom(const FeedbackData& from);
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
  void InternalSwap(FeedbackData* other);
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

  // float batteryPercent = 1;
  void clear_batterypercent();
  static const int kBatteryPercentFieldNumber = 1;
  float batterypercent() const;
  void set_batterypercent(float value);

  // uint32 state = 2;
  void clear_state();
  static const int kStateFieldNumber = 2;
  ::google::protobuf::uint32 state() const;
  void set_state(::google::protobuf::uint32 value);

  // double px = 25;
  void clear_px();
  static const int kPxFieldNumber = 25;
  double px() const;
  void set_px(double value);

  // double py = 26;
  void clear_py();
  static const int kPyFieldNumber = 26;
  double py() const;
  void set_py(double value);

  // double pz = 27;
  void clear_pz();
  static const int kPzFieldNumber = 27;
  double pz() const;
  void set_pz(double value);

  // double roll = 28;
  void clear_roll();
  static const int kRollFieldNumber = 28;
  double roll() const;
  void set_roll(double value);

  // double pitch = 29;
  void clear_pitch();
  static const int kPitchFieldNumber = 29;
  double pitch() const;
  void set_pitch(double value);

  // double yaw = 30;
  void clear_yaw();
  static const int kYawFieldNumber = 30;
  double yaw() const;
  void set_yaw(double value);

  // double vx = 31;
  void clear_vx();
  static const int kVxFieldNumber = 31;
  double vx() const;
  void set_vx(double value);

  // double vy = 32;
  void clear_vy();
  static const int kVyFieldNumber = 32;
  double vy() const;
  void set_vy(double value);

  // double vz = 33;
  void clear_vz();
  static const int kVzFieldNumber = 33;
  double vz() const;
  void set_vz(double value);

  // double vw = 34;
  void clear_vw();
  static const int kVwFieldNumber = 34;
  double vw() const;
  void set_vw(double value);

  // double ax = 35;
  void clear_ax();
  static const int kAxFieldNumber = 35;
  double ax() const;
  void set_ax(double value);

  // double ay = 36;
  void clear_ay();
  static const int kAyFieldNumber = 36;
  double ay() const;
  void set_ay(double value);

  // double az = 37;
  void clear_az();
  static const int kAzFieldNumber = 37;
  double az() const;
  void set_az(double value);

  // double lat = 38;
  void clear_lat();
  static const int kLatFieldNumber = 38;
  double lat() const;
  void set_lat(double value);

  // double lon = 39;
  void clear_lon();
  static const int kLonFieldNumber = 39;
  double lon() const;
  void set_lon(double value);

  // double alt = 40;
  void clear_alt();
  static const int kAltFieldNumber = 40;
  double alt() const;
  void set_alt(double value);

  // @@protoc_insertion_point(class_scope:feedbackData.FeedbackData)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  float batterypercent_;
  ::google::protobuf::uint32 state_;
  double px_;
  double py_;
  double pz_;
  double roll_;
  double pitch_;
  double yaw_;
  double vx_;
  double vy_;
  double vz_;
  double vw_;
  double ax_;
  double ay_;
  double az_;
  double lat_;
  double lon_;
  double alt_;
  mutable int _cached_size_;
  friend struct protobuf_feedbackdata_2eproto::TableStruct;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// FeedbackData

// float batteryPercent = 1;
inline void FeedbackData::clear_batterypercent() {
  batterypercent_ = 0;
}
inline float FeedbackData::batterypercent() const {
  // @@protoc_insertion_point(field_get:feedbackData.FeedbackData.batteryPercent)
  return batterypercent_;
}
inline void FeedbackData::set_batterypercent(float value) {
  
  batterypercent_ = value;
  // @@protoc_insertion_point(field_set:feedbackData.FeedbackData.batteryPercent)
}

// uint32 state = 2;
inline void FeedbackData::clear_state() {
  state_ = 0u;
}
inline ::google::protobuf::uint32 FeedbackData::state() const {
  // @@protoc_insertion_point(field_get:feedbackData.FeedbackData.state)
  return state_;
}
inline void FeedbackData::set_state(::google::protobuf::uint32 value) {
  
  state_ = value;
  // @@protoc_insertion_point(field_set:feedbackData.FeedbackData.state)
}

// double px = 25;
inline void FeedbackData::clear_px() {
  px_ = 0;
}
inline double FeedbackData::px() const {
  // @@protoc_insertion_point(field_get:feedbackData.FeedbackData.px)
  return px_;
}
inline void FeedbackData::set_px(double value) {
  
  px_ = value;
  // @@protoc_insertion_point(field_set:feedbackData.FeedbackData.px)
}

// double py = 26;
inline void FeedbackData::clear_py() {
  py_ = 0;
}
inline double FeedbackData::py() const {
  // @@protoc_insertion_point(field_get:feedbackData.FeedbackData.py)
  return py_;
}
inline void FeedbackData::set_py(double value) {
  
  py_ = value;
  // @@protoc_insertion_point(field_set:feedbackData.FeedbackData.py)
}

// double pz = 27;
inline void FeedbackData::clear_pz() {
  pz_ = 0;
}
inline double FeedbackData::pz() const {
  // @@protoc_insertion_point(field_get:feedbackData.FeedbackData.pz)
  return pz_;
}
inline void FeedbackData::set_pz(double value) {
  
  pz_ = value;
  // @@protoc_insertion_point(field_set:feedbackData.FeedbackData.pz)
}

// double roll = 28;
inline void FeedbackData::clear_roll() {
  roll_ = 0;
}
inline double FeedbackData::roll() const {
  // @@protoc_insertion_point(field_get:feedbackData.FeedbackData.roll)
  return roll_;
}
inline void FeedbackData::set_roll(double value) {
  
  roll_ = value;
  // @@protoc_insertion_point(field_set:feedbackData.FeedbackData.roll)
}

// double pitch = 29;
inline void FeedbackData::clear_pitch() {
  pitch_ = 0;
}
inline double FeedbackData::pitch() const {
  // @@protoc_insertion_point(field_get:feedbackData.FeedbackData.pitch)
  return pitch_;
}
inline void FeedbackData::set_pitch(double value) {
  
  pitch_ = value;
  // @@protoc_insertion_point(field_set:feedbackData.FeedbackData.pitch)
}

// double yaw = 30;
inline void FeedbackData::clear_yaw() {
  yaw_ = 0;
}
inline double FeedbackData::yaw() const {
  // @@protoc_insertion_point(field_get:feedbackData.FeedbackData.yaw)
  return yaw_;
}
inline void FeedbackData::set_yaw(double value) {
  
  yaw_ = value;
  // @@protoc_insertion_point(field_set:feedbackData.FeedbackData.yaw)
}

// double vx = 31;
inline void FeedbackData::clear_vx() {
  vx_ = 0;
}
inline double FeedbackData::vx() const {
  // @@protoc_insertion_point(field_get:feedbackData.FeedbackData.vx)
  return vx_;
}
inline void FeedbackData::set_vx(double value) {
  
  vx_ = value;
  // @@protoc_insertion_point(field_set:feedbackData.FeedbackData.vx)
}

// double vy = 32;
inline void FeedbackData::clear_vy() {
  vy_ = 0;
}
inline double FeedbackData::vy() const {
  // @@protoc_insertion_point(field_get:feedbackData.FeedbackData.vy)
  return vy_;
}
inline void FeedbackData::set_vy(double value) {
  
  vy_ = value;
  // @@protoc_insertion_point(field_set:feedbackData.FeedbackData.vy)
}

// double vz = 33;
inline void FeedbackData::clear_vz() {
  vz_ = 0;
}
inline double FeedbackData::vz() const {
  // @@protoc_insertion_point(field_get:feedbackData.FeedbackData.vz)
  return vz_;
}
inline void FeedbackData::set_vz(double value) {
  
  vz_ = value;
  // @@protoc_insertion_point(field_set:feedbackData.FeedbackData.vz)
}

// double vw = 34;
inline void FeedbackData::clear_vw() {
  vw_ = 0;
}
inline double FeedbackData::vw() const {
  // @@protoc_insertion_point(field_get:feedbackData.FeedbackData.vw)
  return vw_;
}
inline void FeedbackData::set_vw(double value) {
  
  vw_ = value;
  // @@protoc_insertion_point(field_set:feedbackData.FeedbackData.vw)
}

// double ax = 35;
inline void FeedbackData::clear_ax() {
  ax_ = 0;
}
inline double FeedbackData::ax() const {
  // @@protoc_insertion_point(field_get:feedbackData.FeedbackData.ax)
  return ax_;
}
inline void FeedbackData::set_ax(double value) {
  
  ax_ = value;
  // @@protoc_insertion_point(field_set:feedbackData.FeedbackData.ax)
}

// double ay = 36;
inline void FeedbackData::clear_ay() {
  ay_ = 0;
}
inline double FeedbackData::ay() const {
  // @@protoc_insertion_point(field_get:feedbackData.FeedbackData.ay)
  return ay_;
}
inline void FeedbackData::set_ay(double value) {
  
  ay_ = value;
  // @@protoc_insertion_point(field_set:feedbackData.FeedbackData.ay)
}

// double az = 37;
inline void FeedbackData::clear_az() {
  az_ = 0;
}
inline double FeedbackData::az() const {
  // @@protoc_insertion_point(field_get:feedbackData.FeedbackData.az)
  return az_;
}
inline void FeedbackData::set_az(double value) {
  
  az_ = value;
  // @@protoc_insertion_point(field_set:feedbackData.FeedbackData.az)
}

// double lat = 38;
inline void FeedbackData::clear_lat() {
  lat_ = 0;
}
inline double FeedbackData::lat() const {
  // @@protoc_insertion_point(field_get:feedbackData.FeedbackData.lat)
  return lat_;
}
inline void FeedbackData::set_lat(double value) {
  
  lat_ = value;
  // @@protoc_insertion_point(field_set:feedbackData.FeedbackData.lat)
}

// double lon = 39;
inline void FeedbackData::clear_lon() {
  lon_ = 0;
}
inline double FeedbackData::lon() const {
  // @@protoc_insertion_point(field_get:feedbackData.FeedbackData.lon)
  return lon_;
}
inline void FeedbackData::set_lon(double value) {
  
  lon_ = value;
  // @@protoc_insertion_point(field_set:feedbackData.FeedbackData.lon)
}

// double alt = 40;
inline void FeedbackData::clear_alt() {
  alt_ = 0;
}
inline double FeedbackData::alt() const {
  // @@protoc_insertion_point(field_get:feedbackData.FeedbackData.alt)
  return alt_;
}
inline void FeedbackData::set_alt(double value) {
  
  alt_ = value;
  // @@protoc_insertion_point(field_set:feedbackData.FeedbackData.alt)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)


}  // namespace feedbackData

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_feedbackdata_2eproto__INCLUDED
