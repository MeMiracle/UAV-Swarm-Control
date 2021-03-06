// Generated by gencpp from file m100_v2/PubSubUavMsg.msg
// DO NOT EDIT!


#ifndef M100_V2_MESSAGE_PUBSUBUAVMSG_H
#define M100_V2_MESSAGE_PUBSUBUAVMSG_H


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
struct PubSubUavMsg_
{
  typedef PubSubUavMsg_<ContainerAllocator> Type;

  PubSubUavMsg_()
    : src_uav_id(0)
    , component()
    , cmd_priority(0)
    , playload()
    , start_time(0.0)
    , tgt_uav_count(0)  {
    }
  PubSubUavMsg_(const ContainerAllocator& _alloc)
    : src_uav_id(0)
    , component(_alloc)
    , cmd_priority(0)
    , playload(_alloc)
    , start_time(0.0)
    , tgt_uav_count(0)  {
  (void)_alloc;
    }



   typedef int32_t _src_uav_id_type;
  _src_uav_id_type src_uav_id;

   typedef std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  _component_type;
  _component_type component;

   typedef int32_t _cmd_priority_type;
  _cmd_priority_type cmd_priority;

   typedef std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  _playload_type;
  _playload_type playload;

   typedef double _start_time_type;
  _start_time_type start_time;

   typedef int32_t _tgt_uav_count_type;
  _tgt_uav_count_type tgt_uav_count;





  typedef boost::shared_ptr< ::m100_v2::PubSubUavMsg_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::m100_v2::PubSubUavMsg_<ContainerAllocator> const> ConstPtr;

}; // struct PubSubUavMsg_

typedef ::m100_v2::PubSubUavMsg_<std::allocator<void> > PubSubUavMsg;

typedef boost::shared_ptr< ::m100_v2::PubSubUavMsg > PubSubUavMsgPtr;
typedef boost::shared_ptr< ::m100_v2::PubSubUavMsg const> PubSubUavMsgConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::m100_v2::PubSubUavMsg_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::m100_v2::PubSubUavMsg_<ContainerAllocator> >::stream(s, "", v);
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
struct IsFixedSize< ::m100_v2::PubSubUavMsg_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::m100_v2::PubSubUavMsg_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::m100_v2::PubSubUavMsg_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::m100_v2::PubSubUavMsg_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::m100_v2::PubSubUavMsg_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::m100_v2::PubSubUavMsg_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::m100_v2::PubSubUavMsg_<ContainerAllocator> >
{
  static const char* value()
  {
    return "67891a76d6771782154f9e5d90983fb9";
  }

  static const char* value(const ::m100_v2::PubSubUavMsg_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x67891a76d6771782ULL;
  static const uint64_t static_value2 = 0x154f9e5d90983fb9ULL;
};

template<class ContainerAllocator>
struct DataType< ::m100_v2::PubSubUavMsg_<ContainerAllocator> >
{
  static const char* value()
  {
    return "m100_v2/PubSubUavMsg";
  }

  static const char* value(const ::m100_v2::PubSubUavMsg_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::m100_v2::PubSubUavMsg_<ContainerAllocator> >
{
  static const char* value()
  {
    return "int32 src_uav_id\n\
string component\n\
int32 cmd_priority\n\
string playload\n\
float64 start_time\n\
int32 tgt_uav_count\n\
";
  }

  static const char* value(const ::m100_v2::PubSubUavMsg_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::m100_v2::PubSubUavMsg_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.src_uav_id);
      stream.next(m.component);
      stream.next(m.cmd_priority);
      stream.next(m.playload);
      stream.next(m.start_time);
      stream.next(m.tgt_uav_count);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct PubSubUavMsg_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::m100_v2::PubSubUavMsg_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::m100_v2::PubSubUavMsg_<ContainerAllocator>& v)
  {
    s << indent << "src_uav_id: ";
    Printer<int32_t>::stream(s, indent + "  ", v.src_uav_id);
    s << indent << "component: ";
    Printer<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > >::stream(s, indent + "  ", v.component);
    s << indent << "cmd_priority: ";
    Printer<int32_t>::stream(s, indent + "  ", v.cmd_priority);
    s << indent << "playload: ";
    Printer<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > >::stream(s, indent + "  ", v.playload);
    s << indent << "start_time: ";
    Printer<double>::stream(s, indent + "  ", v.start_time);
    s << indent << "tgt_uav_count: ";
    Printer<int32_t>::stream(s, indent + "  ", v.tgt_uav_count);
  }
};

} // namespace message_operations
} // namespace ros

#endif // M100_V2_MESSAGE_PUBSUBUAVMSG_H
