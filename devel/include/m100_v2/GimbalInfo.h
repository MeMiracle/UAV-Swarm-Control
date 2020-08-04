// Generated by gencpp from file m100_v2/GimbalInfo.msg
// DO NOT EDIT!


#ifndef M100_V2_MESSAGE_GIMBALINFO_H
#define M100_V2_MESSAGE_GIMBALINFO_H


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
struct GimbalInfo_
{
  typedef GimbalInfo_<ContainerAllocator> Type;

  GimbalInfo_()
    : target_x(0.0)
    , target_y(0.0)
    , gimbal_x(0.0)
    , gimbal_y(0.0)  {
    }
  GimbalInfo_(const ContainerAllocator& _alloc)
    : target_x(0.0)
    , target_y(0.0)
    , gimbal_x(0.0)
    , gimbal_y(0.0)  {
  (void)_alloc;
    }



   typedef double _target_x_type;
  _target_x_type target_x;

   typedef double _target_y_type;
  _target_y_type target_y;

   typedef double _gimbal_x_type;
  _gimbal_x_type gimbal_x;

   typedef double _gimbal_y_type;
  _gimbal_y_type gimbal_y;





  typedef boost::shared_ptr< ::m100_v2::GimbalInfo_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::m100_v2::GimbalInfo_<ContainerAllocator> const> ConstPtr;

}; // struct GimbalInfo_

typedef ::m100_v2::GimbalInfo_<std::allocator<void> > GimbalInfo;

typedef boost::shared_ptr< ::m100_v2::GimbalInfo > GimbalInfoPtr;
typedef boost::shared_ptr< ::m100_v2::GimbalInfo const> GimbalInfoConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::m100_v2::GimbalInfo_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::m100_v2::GimbalInfo_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace m100_v2

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': True, 'IsMessage': True, 'HasHeader': False}
// {'std_msgs': ['/opt/ros/kinetic/share/std_msgs/cmake/../msg'], 'geometry_msgs': ['/opt/ros/kinetic/share/geometry_msgs/cmake/../msg'], 'm100_v2': ['/home/uav/ROS/m100/src/m100_v2/msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::m100_v2::GimbalInfo_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::m100_v2::GimbalInfo_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::m100_v2::GimbalInfo_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::m100_v2::GimbalInfo_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::m100_v2::GimbalInfo_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::m100_v2::GimbalInfo_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::m100_v2::GimbalInfo_<ContainerAllocator> >
{
  static const char* value()
  {
    return "622571951a464a0a539fdb797d0d7edd";
  }

  static const char* value(const ::m100_v2::GimbalInfo_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x622571951a464a0aULL;
  static const uint64_t static_value2 = 0x539fdb797d0d7eddULL;
};

template<class ContainerAllocator>
struct DataType< ::m100_v2::GimbalInfo_<ContainerAllocator> >
{
  static const char* value()
  {
    return "m100_v2/GimbalInfo";
  }

  static const char* value(const ::m100_v2::GimbalInfo_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::m100_v2::GimbalInfo_<ContainerAllocator> >
{
  static const char* value()
  {
    return "float64 target_x\n\
float64 target_y\n\
float64 gimbal_x\n\
float64 gimbal_y\n\
";
  }

  static const char* value(const ::m100_v2::GimbalInfo_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::m100_v2::GimbalInfo_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.target_x);
      stream.next(m.target_y);
      stream.next(m.gimbal_x);
      stream.next(m.gimbal_y);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct GimbalInfo_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::m100_v2::GimbalInfo_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::m100_v2::GimbalInfo_<ContainerAllocator>& v)
  {
    s << indent << "target_x: ";
    Printer<double>::stream(s, indent + "  ", v.target_x);
    s << indent << "target_y: ";
    Printer<double>::stream(s, indent + "  ", v.target_y);
    s << indent << "gimbal_x: ";
    Printer<double>::stream(s, indent + "  ", v.gimbal_x);
    s << indent << "gimbal_y: ";
    Printer<double>::stream(s, indent + "  ", v.gimbal_y);
  }
};

} // namespace message_operations
} // namespace ros

#endif // M100_V2_MESSAGE_GIMBALINFO_H