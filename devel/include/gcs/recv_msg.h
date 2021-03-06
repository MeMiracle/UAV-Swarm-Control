// Generated by gencpp from file gcs/recv_msg.msg
// DO NOT EDIT!


#ifndef GCS_MESSAGE_RECV_MSG_H
#define GCS_MESSAGE_RECV_MSG_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace gcs
{
template <class ContainerAllocator>
struct recv_msg_
{
  typedef recv_msg_<ContainerAllocator> Type;

  recv_msg_()
    : recv_msg()  {
      recv_msg.assign(0);
  }
  recv_msg_(const ContainerAllocator& _alloc)
    : recv_msg()  {
  (void)_alloc;
      recv_msg.assign(0);
  }



   typedef boost::array<uint8_t, 263>  _recv_msg_type;
  _recv_msg_type recv_msg;





  typedef boost::shared_ptr< ::gcs::recv_msg_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::gcs::recv_msg_<ContainerAllocator> const> ConstPtr;

}; // struct recv_msg_

typedef ::gcs::recv_msg_<std::allocator<void> > recv_msg;

typedef boost::shared_ptr< ::gcs::recv_msg > recv_msgPtr;
typedef boost::shared_ptr< ::gcs::recv_msg const> recv_msgConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::gcs::recv_msg_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::gcs::recv_msg_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace gcs

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': True, 'IsMessage': True, 'HasHeader': False}
// {'gcs': ['/home/uav/ROS/m100/src/gcs/msg'], 'std_msgs': ['/opt/ros/kinetic/share/std_msgs/cmake/../msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::gcs::recv_msg_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::gcs::recv_msg_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::gcs::recv_msg_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::gcs::recv_msg_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::gcs::recv_msg_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::gcs::recv_msg_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::gcs::recv_msg_<ContainerAllocator> >
{
  static const char* value()
  {
    return "dc444b49bfa57eb79eb16e3052e264e0";
  }

  static const char* value(const ::gcs::recv_msg_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0xdc444b49bfa57eb7ULL;
  static const uint64_t static_value2 = 0x9eb16e3052e264e0ULL;
};

template<class ContainerAllocator>
struct DataType< ::gcs::recv_msg_<ContainerAllocator> >
{
  static const char* value()
  {
    return "gcs/recv_msg";
  }

  static const char* value(const ::gcs::recv_msg_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::gcs::recv_msg_<ContainerAllocator> >
{
  static const char* value()
  {
    return "uint8[263] recv_msg\n\
";
  }

  static const char* value(const ::gcs::recv_msg_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::gcs::recv_msg_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.recv_msg);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct recv_msg_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::gcs::recv_msg_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::gcs::recv_msg_<ContainerAllocator>& v)
  {
    s << indent << "recv_msg[]" << std::endl;
    for (size_t i = 0; i < v.recv_msg.size(); ++i)
    {
      s << indent << "  recv_msg[" << i << "]: ";
      Printer<uint8_t>::stream(s, indent + "  ", v.recv_msg[i]);
    }
  }
};

} // namespace message_operations
} // namespace ros

#endif // GCS_MESSAGE_RECV_MSG_H
