// Auto-generated. Do not edit!

// (in-package simulation_m100.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;
let geometry_msgs = _finder('geometry_msgs');
let std_msgs = _finder('std_msgs');

//-----------------------------------------------------------

class ObstacleMsg {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.header = null;
      this.center = null;
      this.point1 = null;
      this.point2 = null;
      this.minRadius = null;
    }
    else {
      if (initObj.hasOwnProperty('header')) {
        this.header = initObj.header
      }
      else {
        this.header = new std_msgs.msg.Header();
      }
      if (initObj.hasOwnProperty('center')) {
        this.center = initObj.center
      }
      else {
        this.center = [];
      }
      if (initObj.hasOwnProperty('point1')) {
        this.point1 = initObj.point1
      }
      else {
        this.point1 = [];
      }
      if (initObj.hasOwnProperty('point2')) {
        this.point2 = initObj.point2
      }
      else {
        this.point2 = [];
      }
      if (initObj.hasOwnProperty('minRadius')) {
        this.minRadius = initObj.minRadius
      }
      else {
        this.minRadius = [];
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type ObstacleMsg
    // Serialize message field [header]
    bufferOffset = std_msgs.msg.Header.serialize(obj.header, buffer, bufferOffset);
    // Serialize message field [center]
    // Serialize the length for message field [center]
    bufferOffset = _serializer.uint32(obj.center.length, buffer, bufferOffset);
    obj.center.forEach((val) => {
      bufferOffset = geometry_msgs.msg.Point32.serialize(val, buffer, bufferOffset);
    });
    // Serialize message field [point1]
    // Serialize the length for message field [point1]
    bufferOffset = _serializer.uint32(obj.point1.length, buffer, bufferOffset);
    obj.point1.forEach((val) => {
      bufferOffset = geometry_msgs.msg.Point32.serialize(val, buffer, bufferOffset);
    });
    // Serialize message field [point2]
    // Serialize the length for message field [point2]
    bufferOffset = _serializer.uint32(obj.point2.length, buffer, bufferOffset);
    obj.point2.forEach((val) => {
      bufferOffset = geometry_msgs.msg.Point32.serialize(val, buffer, bufferOffset);
    });
    // Serialize message field [minRadius]
    bufferOffset = _arraySerializer.float32(obj.minRadius, buffer, bufferOffset, null);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type ObstacleMsg
    let len;
    let data = new ObstacleMsg(null);
    // Deserialize message field [header]
    data.header = std_msgs.msg.Header.deserialize(buffer, bufferOffset);
    // Deserialize message field [center]
    // Deserialize array length for message field [center]
    len = _deserializer.uint32(buffer, bufferOffset);
    data.center = new Array(len);
    for (let i = 0; i < len; ++i) {
      data.center[i] = geometry_msgs.msg.Point32.deserialize(buffer, bufferOffset)
    }
    // Deserialize message field [point1]
    // Deserialize array length for message field [point1]
    len = _deserializer.uint32(buffer, bufferOffset);
    data.point1 = new Array(len);
    for (let i = 0; i < len; ++i) {
      data.point1[i] = geometry_msgs.msg.Point32.deserialize(buffer, bufferOffset)
    }
    // Deserialize message field [point2]
    // Deserialize array length for message field [point2]
    len = _deserializer.uint32(buffer, bufferOffset);
    data.point2 = new Array(len);
    for (let i = 0; i < len; ++i) {
      data.point2[i] = geometry_msgs.msg.Point32.deserialize(buffer, bufferOffset)
    }
    // Deserialize message field [minRadius]
    data.minRadius = _arrayDeserializer.float32(buffer, bufferOffset, null)
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += std_msgs.msg.Header.getMessageSize(object.header);
    length += 12 * object.center.length;
    length += 12 * object.point1.length;
    length += 12 * object.point2.length;
    length += 4 * object.minRadius.length;
    return length + 16;
  }

  static datatype() {
    // Returns string type for a message object
    return 'simulation_m100/ObstacleMsg';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '1f9c26cc298db55081856b7b6f06a61c';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    Header header
    geometry_msgs/Point32[] center
    geometry_msgs/Point32[] point1
    geometry_msgs/Point32[] point2
    float32[] minRadius
    ================================================================================
    MSG: std_msgs/Header
    # Standard metadata for higher-level stamped data types.
    # This is generally used to communicate timestamped data 
    # in a particular coordinate frame.
    # 
    # sequence ID: consecutively increasing ID 
    uint32 seq
    #Two-integer timestamp that is expressed as:
    # * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')
    # * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')
    # time-handling sugar is provided by the client library
    time stamp
    #Frame this data is associated with
    # 0: no frame
    # 1: global frame
    string frame_id
    
    ================================================================================
    MSG: geometry_msgs/Point32
    # This contains the position of a point in free space(with 32 bits of precision).
    # It is recommeded to use Point wherever possible instead of Point32.  
    # 
    # This recommendation is to promote interoperability.  
    #
    # This message is designed to take up less space when sending
    # lots of points at once, as in the case of a PointCloud.  
    
    float32 x
    float32 y
    float32 z
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new ObstacleMsg(null);
    if (msg.header !== undefined) {
      resolved.header = std_msgs.msg.Header.Resolve(msg.header)
    }
    else {
      resolved.header = new std_msgs.msg.Header()
    }

    if (msg.center !== undefined) {
      resolved.center = new Array(msg.center.length);
      for (let i = 0; i < resolved.center.length; ++i) {
        resolved.center[i] = geometry_msgs.msg.Point32.Resolve(msg.center[i]);
      }
    }
    else {
      resolved.center = []
    }

    if (msg.point1 !== undefined) {
      resolved.point1 = new Array(msg.point1.length);
      for (let i = 0; i < resolved.point1.length; ++i) {
        resolved.point1[i] = geometry_msgs.msg.Point32.Resolve(msg.point1[i]);
      }
    }
    else {
      resolved.point1 = []
    }

    if (msg.point2 !== undefined) {
      resolved.point2 = new Array(msg.point2.length);
      for (let i = 0; i < resolved.point2.length; ++i) {
        resolved.point2[i] = geometry_msgs.msg.Point32.Resolve(msg.point2[i]);
      }
    }
    else {
      resolved.point2 = []
    }

    if (msg.minRadius !== undefined) {
      resolved.minRadius = msg.minRadius;
    }
    else {
      resolved.minRadius = []
    }

    return resolved;
    }
};

module.exports = ObstacleMsg;
