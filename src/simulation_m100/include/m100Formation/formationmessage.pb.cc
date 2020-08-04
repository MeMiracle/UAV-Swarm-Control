// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: formationmessage.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "formationmessage.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace formationmsg {
class FormationMsgDefaultTypeInternal {
public:
 ::google::protobuf::internal::ExplicitlyConstructed<FormationMsg>
     _instance;
} _FormationMsg_default_instance_;

namespace protobuf_formationmessage_2eproto {


namespace {

::google::protobuf::Metadata file_level_metadata[1];

}  // namespace

PROTOBUF_CONSTEXPR_VAR ::google::protobuf::internal::ParseTableField
    const TableStruct::entries[] GOOGLE_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  {0, 0, 0, ::google::protobuf::internal::kInvalidMask, 0, 0},
};

PROTOBUF_CONSTEXPR_VAR ::google::protobuf::internal::AuxillaryParseTableField
    const TableStruct::aux[] GOOGLE_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  ::google::protobuf::internal::AuxillaryParseTableField(),
};
PROTOBUF_CONSTEXPR_VAR ::google::protobuf::internal::ParseTable const
    TableStruct::schema[] GOOGLE_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  { NULL, NULL, 0, -1, -1, -1, -1, NULL, false },
};

const ::google::protobuf::uint32 TableStruct::offsets[] GOOGLE_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(FormationMsg, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(FormationMsg, getformationid_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(FormationMsg, uavid_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(FormationMsg, formationflag_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(FormationMsg, changeformation_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(FormationMsg, formationpointx_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(FormationMsg, formationpointy_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(FormationMsg, formationpointz_),
};
static const ::google::protobuf::internal::MigrationSchema schemas[] GOOGLE_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(FormationMsg)},
};

static ::google::protobuf::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::google::protobuf::Message*>(&_FormationMsg_default_instance_),
};

namespace {

void protobuf_AssignDescriptors() {
  AddDescriptors();
  ::google::protobuf::MessageFactory* factory = NULL;
  AssignDescriptors(
      "formationmessage.proto", schemas, file_default_instances, TableStruct::offsets, factory,
      file_level_metadata, NULL, NULL);
}

void protobuf_AssignDescriptorsOnce() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &protobuf_AssignDescriptors);
}

void protobuf_RegisterTypes(const ::std::string&) GOOGLE_ATTRIBUTE_COLD;
void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::internal::RegisterAllTypes(file_level_metadata, 1);
}

}  // namespace
void TableStruct::InitDefaultsImpl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::internal::InitProtobufDefaults();
  _FormationMsg_default_instance_._instance.DefaultConstruct();
  ::google::protobuf::internal::OnShutdownDestroyMessage(
      &_FormationMsg_default_instance_);}

void InitDefaults() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &TableStruct::InitDefaultsImpl);
}
namespace {
void AddDescriptorsImpl() {
  InitDefaults();
  static const char descriptor[] GOOGLE_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
      "\n\026formationmessage.proto\022\014formationmsg\"\260"
      "\001\n\014FormationMsg\022\026\n\016getFormationID\030\001 \001(\r\022"
      "\r\n\005uavID\030\002 \001(\r\022\025\n\rformationflag\030\003 \001(\010\022\027\n"
      "\017changeFormation\030\004 \001(\010\022\027\n\017formationPoint"
      "X\030\005 \001(\002\022\027\n\017formationPointY\030\006 \001(\002\022\027\n\017form"
      "ationPointZ\030\007 \001(\002b\006proto3"
  };
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
      descriptor, 225);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "formationmessage.proto", &protobuf_RegisterTypes);
}
} // anonymous namespace

void AddDescriptors() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &AddDescriptorsImpl);
}
// Force AddDescriptors() to be called at dynamic initialization time.
struct StaticDescriptorInitializer {
  StaticDescriptorInitializer() {
    AddDescriptors();
  }
} static_descriptor_initializer;

}  // namespace protobuf_formationmessage_2eproto


// ===================================================================

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int FormationMsg::kGetFormationIDFieldNumber;
const int FormationMsg::kUavIDFieldNumber;
const int FormationMsg::kFormationflagFieldNumber;
const int FormationMsg::kChangeFormationFieldNumber;
const int FormationMsg::kFormationPointXFieldNumber;
const int FormationMsg::kFormationPointYFieldNumber;
const int FormationMsg::kFormationPointZFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

FormationMsg::FormationMsg()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  if (GOOGLE_PREDICT_TRUE(this != internal_default_instance())) {
    protobuf_formationmessage_2eproto::InitDefaults();
  }
  SharedCtor();
  // @@protoc_insertion_point(constructor:formationmsg.FormationMsg)
}
FormationMsg::FormationMsg(const FormationMsg& from)
  : ::google::protobuf::Message(),
      _internal_metadata_(NULL),
      _cached_size_(0) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::memcpy(&getformationid_, &from.getformationid_,
    static_cast<size_t>(reinterpret_cast<char*>(&formationpointz_) -
    reinterpret_cast<char*>(&getformationid_)) + sizeof(formationpointz_));
  // @@protoc_insertion_point(copy_constructor:formationmsg.FormationMsg)
}

void FormationMsg::SharedCtor() {
  ::memset(&getformationid_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&formationpointz_) -
      reinterpret_cast<char*>(&getformationid_)) + sizeof(formationpointz_));
  _cached_size_ = 0;
}

FormationMsg::~FormationMsg() {
  // @@protoc_insertion_point(destructor:formationmsg.FormationMsg)
  SharedDtor();
}

void FormationMsg::SharedDtor() {
}

void FormationMsg::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* FormationMsg::descriptor() {
  protobuf_formationmessage_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_formationmessage_2eproto::file_level_metadata[kIndexInFileMessages].descriptor;
}

const FormationMsg& FormationMsg::default_instance() {
  protobuf_formationmessage_2eproto::InitDefaults();
  return *internal_default_instance();
}

FormationMsg* FormationMsg::New(::google::protobuf::Arena* arena) const {
  FormationMsg* n = new FormationMsg;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void FormationMsg::Clear() {
// @@protoc_insertion_point(message_clear_start:formationmsg.FormationMsg)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  ::memset(&getformationid_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&formationpointz_) -
      reinterpret_cast<char*>(&getformationid_)) + sizeof(formationpointz_));
  _internal_metadata_.Clear();
}

bool FormationMsg::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:formationmsg.FormationMsg)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // uint32 getFormationID = 1;
      case 1: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(8u /* 8 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &getformationid_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // uint32 uavID = 2;
      case 2: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(16u /* 16 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &uavid_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // bool formationflag = 3;
      case 3: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(24u /* 24 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &formationflag_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // bool changeFormation = 4;
      case 4: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(32u /* 32 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &changeformation_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // float formationPointX = 5;
      case 5: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(45u /* 45 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   float, ::google::protobuf::internal::WireFormatLite::TYPE_FLOAT>(
                 input, &formationpointx_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // float formationPointY = 6;
      case 6: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(53u /* 53 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   float, ::google::protobuf::internal::WireFormatLite::TYPE_FLOAT>(
                 input, &formationpointy_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // float formationPointZ = 7;
      case 7: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(61u /* 61 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   float, ::google::protobuf::internal::WireFormatLite::TYPE_FLOAT>(
                 input, &formationpointz_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, _internal_metadata_.mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:formationmsg.FormationMsg)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:formationmsg.FormationMsg)
  return false;
#undef DO_
}

void FormationMsg::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:formationmsg.FormationMsg)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // uint32 getFormationID = 1;
  if (this->getformationid() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->getformationid(), output);
  }

  // uint32 uavID = 2;
  if (this->uavid() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(2, this->uavid(), output);
  }

  // bool formationflag = 3;
  if (this->formationflag() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(3, this->formationflag(), output);
  }

  // bool changeFormation = 4;
  if (this->changeformation() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(4, this->changeformation(), output);
  }

  // float formationPointX = 5;
  if (this->formationpointx() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteFloat(5, this->formationpointx(), output);
  }

  // float formationPointY = 6;
  if (this->formationpointy() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteFloat(6, this->formationpointy(), output);
  }

  // float formationPointZ = 7;
  if (this->formationpointz() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteFloat(7, this->formationpointz(), output);
  }

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), output);
  }
  // @@protoc_insertion_point(serialize_end:formationmsg.FormationMsg)
}

::google::protobuf::uint8* FormationMsg::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:formationmsg.FormationMsg)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // uint32 getFormationID = 1;
  if (this->getformationid() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(1, this->getformationid(), target);
  }

  // uint32 uavID = 2;
  if (this->uavid() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(2, this->uavid(), target);
  }

  // bool formationflag = 3;
  if (this->formationflag() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(3, this->formationflag(), target);
  }

  // bool changeFormation = 4;
  if (this->changeformation() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(4, this->changeformation(), target);
  }

  // float formationPointX = 5;
  if (this->formationpointx() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteFloatToArray(5, this->formationpointx(), target);
  }

  // float formationPointY = 6;
  if (this->formationpointy() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteFloatToArray(6, this->formationpointy(), target);
  }

  // float formationPointZ = 7;
  if (this->formationpointz() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteFloatToArray(7, this->formationpointz(), target);
  }

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:formationmsg.FormationMsg)
  return target;
}

size_t FormationMsg::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:formationmsg.FormationMsg)
  size_t total_size = 0;

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()));
  }
  // uint32 getFormationID = 1;
  if (this->getformationid() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::UInt32Size(
        this->getformationid());
  }

  // uint32 uavID = 2;
  if (this->uavid() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::UInt32Size(
        this->uavid());
  }

  // bool formationflag = 3;
  if (this->formationflag() != 0) {
    total_size += 1 + 1;
  }

  // bool changeFormation = 4;
  if (this->changeformation() != 0) {
    total_size += 1 + 1;
  }

  // float formationPointX = 5;
  if (this->formationpointx() != 0) {
    total_size += 1 + 4;
  }

  // float formationPointY = 6;
  if (this->formationpointy() != 0) {
    total_size += 1 + 4;
  }

  // float formationPointZ = 7;
  if (this->formationpointz() != 0) {
    total_size += 1 + 4;
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = cached_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void FormationMsg::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:formationmsg.FormationMsg)
  GOOGLE_DCHECK_NE(&from, this);
  const FormationMsg* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const FormationMsg>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:formationmsg.FormationMsg)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:formationmsg.FormationMsg)
    MergeFrom(*source);
  }
}

void FormationMsg::MergeFrom(const FormationMsg& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:formationmsg.FormationMsg)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.getformationid() != 0) {
    set_getformationid(from.getformationid());
  }
  if (from.uavid() != 0) {
    set_uavid(from.uavid());
  }
  if (from.formationflag() != 0) {
    set_formationflag(from.formationflag());
  }
  if (from.changeformation() != 0) {
    set_changeformation(from.changeformation());
  }
  if (from.formationpointx() != 0) {
    set_formationpointx(from.formationpointx());
  }
  if (from.formationpointy() != 0) {
    set_formationpointy(from.formationpointy());
  }
  if (from.formationpointz() != 0) {
    set_formationpointz(from.formationpointz());
  }
}

void FormationMsg::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:formationmsg.FormationMsg)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void FormationMsg::CopyFrom(const FormationMsg& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:formationmsg.FormationMsg)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool FormationMsg::IsInitialized() const {
  return true;
}

void FormationMsg::Swap(FormationMsg* other) {
  if (other == this) return;
  InternalSwap(other);
}
void FormationMsg::InternalSwap(FormationMsg* other) {
  using std::swap;
  swap(getformationid_, other->getformationid_);
  swap(uavid_, other->uavid_);
  swap(formationflag_, other->formationflag_);
  swap(changeformation_, other->changeformation_);
  swap(formationpointx_, other->formationpointx_);
  swap(formationpointy_, other->formationpointy_);
  swap(formationpointz_, other->formationpointz_);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata FormationMsg::GetMetadata() const {
  protobuf_formationmessage_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_formationmessage_2eproto::file_level_metadata[kIndexInFileMessages];
}

#if PROTOBUF_INLINE_NOT_IN_HEADERS
// FormationMsg

// uint32 getFormationID = 1;
void FormationMsg::clear_getformationid() {
  getformationid_ = 0u;
}
::google::protobuf::uint32 FormationMsg::getformationid() const {
  // @@protoc_insertion_point(field_get:formationmsg.FormationMsg.getFormationID)
  return getformationid_;
}
void FormationMsg::set_getformationid(::google::protobuf::uint32 value) {
  
  getformationid_ = value;
  // @@protoc_insertion_point(field_set:formationmsg.FormationMsg.getFormationID)
}

// uint32 uavID = 2;
void FormationMsg::clear_uavid() {
  uavid_ = 0u;
}
::google::protobuf::uint32 FormationMsg::uavid() const {
  // @@protoc_insertion_point(field_get:formationmsg.FormationMsg.uavID)
  return uavid_;
}
void FormationMsg::set_uavid(::google::protobuf::uint32 value) {
  
  uavid_ = value;
  // @@protoc_insertion_point(field_set:formationmsg.FormationMsg.uavID)
}

// bool formationflag = 3;
void FormationMsg::clear_formationflag() {
  formationflag_ = false;
}
bool FormationMsg::formationflag() const {
  // @@protoc_insertion_point(field_get:formationmsg.FormationMsg.formationflag)
  return formationflag_;
}
void FormationMsg::set_formationflag(bool value) {
  
  formationflag_ = value;
  // @@protoc_insertion_point(field_set:formationmsg.FormationMsg.formationflag)
}

// bool changeFormation = 4;
void FormationMsg::clear_changeformation() {
  changeformation_ = false;
}
bool FormationMsg::changeformation() const {
  // @@protoc_insertion_point(field_get:formationmsg.FormationMsg.changeFormation)
  return changeformation_;
}
void FormationMsg::set_changeformation(bool value) {
  
  changeformation_ = value;
  // @@protoc_insertion_point(field_set:formationmsg.FormationMsg.changeFormation)
}

// float formationPointX = 5;
void FormationMsg::clear_formationpointx() {
  formationpointx_ = 0;
}
float FormationMsg::formationpointx() const {
  // @@protoc_insertion_point(field_get:formationmsg.FormationMsg.formationPointX)
  return formationpointx_;
}
void FormationMsg::set_formationpointx(float value) {
  
  formationpointx_ = value;
  // @@protoc_insertion_point(field_set:formationmsg.FormationMsg.formationPointX)
}

// float formationPointY = 6;
void FormationMsg::clear_formationpointy() {
  formationpointy_ = 0;
}
float FormationMsg::formationpointy() const {
  // @@protoc_insertion_point(field_get:formationmsg.FormationMsg.formationPointY)
  return formationpointy_;
}
void FormationMsg::set_formationpointy(float value) {
  
  formationpointy_ = value;
  // @@protoc_insertion_point(field_set:formationmsg.FormationMsg.formationPointY)
}

// float formationPointZ = 7;
void FormationMsg::clear_formationpointz() {
  formationpointz_ = 0;
}
float FormationMsg::formationpointz() const {
  // @@protoc_insertion_point(field_get:formationmsg.FormationMsg.formationPointZ)
  return formationpointz_;
}
void FormationMsg::set_formationpointz(float value) {
  
  formationpointz_ = value;
  // @@protoc_insertion_point(field_set:formationmsg.FormationMsg.formationPointZ)
}

#endif  // PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

}  // namespace formationmsg

// @@protoc_insertion_point(global_scope)