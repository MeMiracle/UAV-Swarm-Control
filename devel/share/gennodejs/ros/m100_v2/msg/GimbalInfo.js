// Auto-generated. Do not edit!

// (in-package m100_v2.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------

class GimbalInfo {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.target_x = null;
      this.target_y = null;
      this.gimbal_x = null;
      this.gimbal_y = null;
    }
    else {
      if (initObj.hasOwnProperty('target_x')) {
        this.target_x = initObj.target_x
      }
      else {
        this.target_x = 0.0;
      }
      if (initObj.hasOwnProperty('target_y')) {
        this.target_y = initObj.target_y
      }
      else {
        this.target_y = 0.0;
      }
      if (initObj.hasOwnProperty('gimbal_x')) {
        this.gimbal_x = initObj.gimbal_x
      }
      else {
        this.gimbal_x = 0.0;
      }
      if (initObj.hasOwnProperty('gimbal_y')) {
        this.gimbal_y = initObj.gimbal_y
      }
      else {
        this.gimbal_y = 0.0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type GimbalInfo
    // Serialize message field [target_x]
    bufferOffset = _serializer.float64(obj.target_x, buffer, bufferOffset);
    // Serialize message field [target_y]
    bufferOffset = _serializer.float64(obj.target_y, buffer, bufferOffset);
    // Serialize message field [gimbal_x]
    bufferOffset = _serializer.float64(obj.gimbal_x, buffer, bufferOffset);
    // Serialize message field [gimbal_y]
    bufferOffset = _serializer.float64(obj.gimbal_y, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type GimbalInfo
    let len;
    let data = new GimbalInfo(null);
    // Deserialize message field [target_x]
    data.target_x = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [target_y]
    data.target_y = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [gimbal_x]
    data.gimbal_x = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [gimbal_y]
    data.gimbal_y = _deserializer.float64(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 32;
  }

  static datatype() {
    // Returns string type for a message object
    return 'm100_v2/GimbalInfo';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '622571951a464a0a539fdb797d0d7edd';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    float64 target_x
    float64 target_y
    float64 gimbal_x
    float64 gimbal_y
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new GimbalInfo(null);
    if (msg.target_x !== undefined) {
      resolved.target_x = msg.target_x;
    }
    else {
      resolved.target_x = 0.0
    }

    if (msg.target_y !== undefined) {
      resolved.target_y = msg.target_y;
    }
    else {
      resolved.target_y = 0.0
    }

    if (msg.gimbal_x !== undefined) {
      resolved.gimbal_x = msg.gimbal_x;
    }
    else {
      resolved.gimbal_x = 0.0
    }

    if (msg.gimbal_y !== undefined) {
      resolved.gimbal_y = msg.gimbal_y;
    }
    else {
      resolved.gimbal_y = 0.0
    }

    return resolved;
    }
};

module.exports = GimbalInfo;
