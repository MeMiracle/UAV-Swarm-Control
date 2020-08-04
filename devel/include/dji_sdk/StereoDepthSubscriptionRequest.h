// Generated by gencpp from file dji_sdk/StereoDepthSubscriptionRequest.msg
// DO NOT EDIT!


#ifndef DJI_SDK_MESSAGE_STEREODEPTHSUBSCRIPTIONREQUEST_H
#define DJI_SDK_MESSAGE_STEREODEPTHSUBSCRIPTIONREQUEST_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace dji_sdk
{
template <class ContainerAllocator>
struct StereoDepthSubscriptionRequest_
{
  typedef StereoDepthSubscriptionRequest_<ContainerAllocator> Type;

  StereoDepthSubscriptionRequest_()
    : front_depth_240p(0)
    , unsubscribe_240p(0)  {
    }
  StereoDepthSubscriptionRequest_(const ContainerAllocator& _alloc)
    : front_depth_240p(0)
    , unsubscribe_240p(0)  {
  (void)_alloc;
    }



   typedef uint8_t _front_depth_240p_type;
  _front_depth_240p_type front_depth_240p;

   typedef uint8_t _unsubscribe_240p_type;
  _unsubscribe_240p_type unsubscribe_240p;





  typedef boost::shared_ptr< ::dji_sdk::StereoDepthSubscriptionRequest_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::dji_sdk::StereoDepthSubscriptionRequest_<ContainerAllocator> const> ConstPtr;

}; // struct StereoDepthSubscriptionRequest_

typedef ::dji_sdk::StereoDepthSubscriptionRequest_<std::allocator<void> > StereoDepthSubscriptionRequest;

typedef boost::shared_ptr< ::dji_sdk::StereoDepthSubscriptionRequest > StereoDepthSubscriptionRequestPtr;
typedef boost::shared_ptr< ::dji_sdk::StereoDepthSubscriptionRequest const> StereoDepthSubscriptionRequestConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::dji_sdk::StereoDepthSubscriptionRequest_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::dji_sdk::StereoDepthSubscriptionRequest_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace dji_sdk

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': True, 'IsMessage': True, 'HasHeader': False}
// {'nav_msgs': ['/opt/ros/kinetic/share/nav_msgs/cmake/../msg'], 'dji_sdk': ['/home/uav/ROS/m100/src/dji_sdk/msg'], 'std_msgs': ['/opt/ros/kinetic/share/std_msgs/cmake/../msg'], 'actionlib_msgs': ['/opt/ros/kinetic/share/actionlib_msgs/cmake/../msg'], 'sensor_msgs': ['/opt/ros/kinetic/share/sensor_msgs/cmake/../msg'], 'geometry_msgs': ['/opt/ros/kinetic/share/geometry_msgs/cmake/../msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::dji_sdk::StereoDepthSubscriptionRequest_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::dji_sdk::StereoDepthSubscriptionRequest_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::dji_sdk::StereoDepthSubscriptionRequest_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::dji_sdk::StereoDepthSubscriptionRequest_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::dji_sdk::StereoDepthSubscriptionRequest_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::dji_sdk::StereoDepthSubscriptionRequest_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::dji_sdk::StereoDepthSubscriptionRequest_<ContainerAllocator> >
{
  static const char* value()
  {
    return "fd98d26e6c026b94987bcb49dda81428";
  }

  static const char* value(const ::dji_sdk::StereoDepthSubscriptionRequest_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0xfd98d26e6c026b94ULL;
  static const uint64_t static_value2 = 0x987bcb49dda81428ULL;
};

template<class ContainerAllocator>
struct DataType< ::dji_sdk::StereoDepthSubscriptionRequest_<ContainerAllocator> >
{
  static const char* value()
  {
    return "dji_sdk/StereoDepthSubscriptionRequest";
  }

  static const char* value(const ::dji_sdk::StereoDepthSubscriptionRequest_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::dji_sdk::StereoDepthSubscriptionRequest_<ContainerAllocator> >
{
  static const char* value()
  {
    return "\n\
uint8 front_depth_240p\n\
\n\
\n\
\n\
uint8 unsubscribe_240p\n\
\n\
";
  }

  static const char* value(const ::dji_sdk::StereoDepthSubscriptionRequest_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::dji_sdk::StereoDepthSubscriptionRequest_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.front_depth_240p);
      stream.next(m.unsubscribe_240p);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct StereoDepthSubscriptionRequest_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::dji_sdk::StereoDepthSubscriptionRequest_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::dji_sdk::StereoDepthSubscriptionRequest_<ContainerAllocator>& v)
  {
    s << indent << "front_depth_240p: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.front_depth_240p);
    s << indent << "unsubscribe_240p: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.unsubscribe_240p);
  }
};

} // namespace message_operations
} // namespace ros

#endif // DJI_SDK_MESSAGE_STEREODEPTHSUBSCRIPTIONREQUEST_H