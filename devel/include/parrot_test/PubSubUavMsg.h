// Generated by gencpp from file parrot_test/PubSubUavMsg.msg
// DO NOT EDIT!


#ifndef PARROT_TEST_MESSAGE_PUBSUBUAVMSG_H
#define PARROT_TEST_MESSAGE_PUBSUBUAVMSG_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace parrot_test
{
template <class ContainerAllocator>
struct PubSubUavMsg_
{
  typedef PubSubUavMsg_<ContainerAllocator> Type;

  PubSubUavMsg_()
    : str()  {
    }
  PubSubUavMsg_(const ContainerAllocator& _alloc)
    : str(_alloc)  {
  (void)_alloc;
    }



   typedef std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  _str_type;
  _str_type str;





  typedef boost::shared_ptr< ::parrot_test::PubSubUavMsg_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::parrot_test::PubSubUavMsg_<ContainerAllocator> const> ConstPtr;

}; // struct PubSubUavMsg_

typedef ::parrot_test::PubSubUavMsg_<std::allocator<void> > PubSubUavMsg;

typedef boost::shared_ptr< ::parrot_test::PubSubUavMsg > PubSubUavMsgPtr;
typedef boost::shared_ptr< ::parrot_test::PubSubUavMsg const> PubSubUavMsgConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::parrot_test::PubSubUavMsg_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::parrot_test::PubSubUavMsg_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace parrot_test

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': False, 'IsMessage': True, 'HasHeader': False}
// {'std_msgs': ['/opt/ros/kinetic/share/std_msgs/cmake/../msg'], 'parrot_test': ['/home/uav/ROS/m100/src/parrot_test/msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::parrot_test::PubSubUavMsg_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::parrot_test::PubSubUavMsg_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::parrot_test::PubSubUavMsg_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::parrot_test::PubSubUavMsg_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::parrot_test::PubSubUavMsg_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::parrot_test::PubSubUavMsg_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::parrot_test::PubSubUavMsg_<ContainerAllocator> >
{
  static const char* value()
  {
    return "994972b6e03928b2476860ce6c4c8e17";
  }

  static const char* value(const ::parrot_test::PubSubUavMsg_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x994972b6e03928b2ULL;
  static const uint64_t static_value2 = 0x476860ce6c4c8e17ULL;
};

template<class ContainerAllocator>
struct DataType< ::parrot_test::PubSubUavMsg_<ContainerAllocator> >
{
  static const char* value()
  {
    return "parrot_test/PubSubUavMsg";
  }

  static const char* value(const ::parrot_test::PubSubUavMsg_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::parrot_test::PubSubUavMsg_<ContainerAllocator> >
{
  static const char* value()
  {
    return "string str\n\
";
  }

  static const char* value(const ::parrot_test::PubSubUavMsg_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::parrot_test::PubSubUavMsg_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.str);
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
struct Printer< ::parrot_test::PubSubUavMsg_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::parrot_test::PubSubUavMsg_<ContainerAllocator>& v)
  {
    s << indent << "str: ";
    Printer<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > >::stream(s, indent + "  ", v.str);
  }
};

} // namespace message_operations
} // namespace ros

#endif // PARROT_TEST_MESSAGE_PUBSUBUAVMSG_H
