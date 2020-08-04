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

class UavBaseInfo {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.cluster_id = null;
    }
    else {
      if (initObj.hasOwnProperty('cluster_id')) {
        this.cluster_id = initObj.cluster_id
      }
      else {
        this.cluster_id = 0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type UavBaseInfo
    // Serialize message field [cluster_id]
    bufferOffset = _serializer.int32(obj.cluster_id, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type UavBaseInfo
    let len;
    let data = new UavBaseInfo(null);
    // Deserialize message field [cluster_id]
    data.cluster_id = _deserializer.int32(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 4;
  }

  static datatype() {
    // Returns string type for a message object
    return 'simulation_m100/UavBaseInfo';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'e7dc3c0adeebdd7978013511967e61fa';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    int32 cluster_id
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new UavBaseInfo(null);
    if (msg.cluster_id !== undefined) {
      resolved.cluster_id = msg.cluster_id;
    }
    else {
      resolved.cluster_id = 0
    }

    return resolved;
    }
};

module.exports = UavBaseInfo;
