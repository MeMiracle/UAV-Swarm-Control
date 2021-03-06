// Generated by gencpp from file m100_v2/CommInfo.msg
// DO NOT EDIT!


#ifndef M100_V2_MESSAGE_COMMINFO_H
#define M100_V2_MESSAGE_COMMINFO_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace m100_v2
{
template <class ContainerAllocator>
struct CommInfo_
{
  typedef CommInfo_<ContainerAllocator> Type;

  CommInfo_()
    : frame_id(0)
    , src_address()
    , comm_bytes(0)  {
    }
  CommInfo_(const ContainerAllocator& _alloc)
    : frame_id(0)
    , src_address(_alloc)
    , comm_bytes(0)  {
  (void)_alloc;
    }



   typedef int32_t _frame_id_type;
  _frame_id_type frame_id;

   typedef std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  _src_address_type;
  _src_address_type src_address;

   typedef int32_t _comm_bytes_type;
  _comm_bytes_type comm_bytes;





  typedef boost::shared_ptr< ::m100_v2::CommInfo_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::m100_v2::CommInfo_<ContainerAllocator> const> ConstPtr;

}; // struct CommInfo_

typedef ::m100_v2::CommInfo_<std::allocator<void> > CommInfo;

typedef boost::shared_ptr< ::m100_v2::CommInfo > CommInfoPtr;
typedef boost::shared_ptr< ::m100_v2::CommInfo const> CommInfoConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::m100_v2::CommInfo_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::m100_v2::CommInfo_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace m100_v2

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': False, 'IsMessage': True, 'HasHeader': False}
// {'std_msgs': ['/opt/ros/kinetic/share/std_msgs/cmake/../msg'], 'geometry_msgs': ['/opt/ros/kinetic/share/geometry_msgs/cmake/../msg'], 'm100_v2': ['/home/uav/ROS/m100/src/m100_v2/msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::m100_v2::CommInfo_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::m100_v2::CommInfo_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::m100_v2::CommInfo_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::m100_v2::CommInfo_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::m100_v2::CommInfo_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::m100_v2::CommInfo_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::m100_v2::CommInfo_<ContainerAllocator> >
{
  static const char* value()
  {
    return "2925ddf0c2ed340db59c3f89c9d288a7";
  }

  static const char* value(const ::m100_v2::CommInfo_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x2925ddf0c2ed340dULL;
  static const uint64_t static_value2 = 0xb59c3f89c9d288a7ULL;
};

template<class ContainerAllocator>
struct DataType< ::m100_v2::CommInfo_<ContainerAllocator> >
{
  static const char* value()
  {
    return "m100_v2/CommInfo";
  }

  static const char* value(const ::m100_v2::CommInfo_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::m100_v2::CommInfo_<ContainerAllocator> >
{
  static const char* value()
  {
    return "int32 frame_id\n\
string src_address\n\
int32 comm_bytes\n\
";
  }

  static const char* value(const ::m100_v2::CommInfo_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::m100_v2::CommInfo_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.frame_id);
      stream.next(m.src_address);
      stream.next(m.comm_bytes);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct CommInfo_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::m100_v2::CommInfo_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::m100_v2::CommInfo_<ContainerAllocator>& v)
  {
    s << indent << "frame_id: ";
    Printer<int32_t>::stream(s, indent + "  ", v.frame_id);
    s << indent << "src_address: ";
    Printer<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > >::stream(s, indent + "  ", v.src_address);
    s << indent << "comm_bytes: ";
    Printer<int32_t>::stream(s, indent + "  ", v.comm_bytes);
  }
};

} // namespace message_operations
} // namespace ros

#endif // M100_V2_MESSAGE_COMMINFO_H
