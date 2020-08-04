// Auto-generated. Do not edit!

// (in-package simulation_m100.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------

class CommInfo {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.frame_id = null;
      this.src_address = null;
      this.comm_bytes = null;
    }
    else {
      if (initObj.hasOwnProperty('frame_id')) {
        this.frame_id = initObj.frame_id
      }
      else {
        this.frame_id = 0;
      }
      if (initObj.hasOwnProperty('src_address')) {
        this.src_address = initObj.src_address
      }
      else {
        this.src_address = '';
      }
      if (initObj.hasOwnProperty('comm_bytes')) {
        this.comm_bytes = initObj.comm_bytes
      }
      else {
        this.comm_bytes = 0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type CommInfo
    // Serialize message field [frame_id]
    bufferOffset = _serializer.int32(obj.frame_id, buffer, bufferOffset);
    // Serialize message field [src_address]
    bufferOffset = _serializer.string(obj.src_address, buffer, bufferOffset);
    // Serialize message field [comm_bytes]
    bufferOffset = _serializer.int32(obj.comm_bytes, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type CommInfo
    let len;
    let data = new CommInfo(null);
    // Deserialize message field [frame_id]
    data.frame_id = _deserializer.int32(buffer, bufferOffset);
    // Deserialize message field [src_address]
    data.src_address = _deserializer.string(buffer, bufferOffset);
    // Deserialize message field [comm_bytes]
    data.comm_bytes = _deserializer.int32(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += object.src_address.length;
    return length + 12;
  }

  static datatype() {
    // Returns string type for a message object
    return 'simulation_m100/CommInfo';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '2925ddf0c2ed340db59c3f89c9d288a7';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    int32 frame_id
    string src_address
    int32 comm_bytes
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new CommInfo(null);
    if (msg.frame_id !== undefined) {
      resolved.frame_id = msg.frame_id;
    }
    else {
      resolved.frame_id = 0
    }

    if (msg.src_address !== undefined) {
      resolved.src_address = msg.src_address;
    }
    else {
      resolved.src_address = ''
    }

    if (msg.comm_bytes !== undefined) {
      resolved.comm_bytes = msg.comm_bytes;
    }
    else {
      resolved.comm_bytes = 0
    }

    return resolved;
    }
};

module.exports = CommInfo;
