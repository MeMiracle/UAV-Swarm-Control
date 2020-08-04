// Auto-generated. Do not edit!

// (in-package apm.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------

class send_msg {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.send_msg = null;
    }
    else {
      if (initObj.hasOwnProperty('send_msg')) {
        this.send_msg = initObj.send_msg
      }
      else {
        this.send_msg = new Array(259).fill(0);
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type send_msg
    // Check that the constant length array field [send_msg] has the right length
    if (obj.send_msg.length !== 259) {
      throw new Error('Unable to serialize array field send_msg - length must be 259')
    }
    // Serialize message field [send_msg]
    bufferOffset = _arraySerializer.uint8(obj.send_msg, buffer, bufferOffset, 259);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type send_msg
    let len;
    let data = new send_msg(null);
    // Deserialize message field [send_msg]
    data.send_msg = _arrayDeserializer.uint8(buffer, bufferOffset, 259)
    return data;
  }

  static getMessageSize(object) {
    return 259;
  }

  static datatype() {
    // Returns string type for a message object
    return 'apm/send_msg';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'c80dcf834ebc4faedd2627bb33ad2ae3';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    uint8[259] send_msg
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new send_msg(null);
    if (msg.send_msg !== undefined) {
      resolved.send_msg = msg.send_msg;
    }
    else {
      resolved.send_msg = new Array(259).fill(0)
    }

    return resolved;
    }
};

module.exports = send_msg;
