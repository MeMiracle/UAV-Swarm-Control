// Auto-generated. Do not edit!

// (in-package gcs.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------

class recv_msg {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.recv_msg = null;
    }
    else {
      if (initObj.hasOwnProperty('recv_msg')) {
        this.recv_msg = initObj.recv_msg
      }
      else {
        this.recv_msg = new Array(263).fill(0);
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type recv_msg
    // Check that the constant length array field [recv_msg] has the right length
    if (obj.recv_msg.length !== 263) {
      throw new Error('Unable to serialize array field recv_msg - length must be 263')
    }
    // Serialize message field [recv_msg]
    bufferOffset = _arraySerializer.uint8(obj.recv_msg, buffer, bufferOffset, 263);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type recv_msg
    let len;
    let data = new recv_msg(null);
    // Deserialize message field [recv_msg]
    data.recv_msg = _arrayDeserializer.uint8(buffer, bufferOffset, 263)
    return data;
  }

  static getMessageSize(object) {
    return 263;
  }

  static datatype() {
    // Returns string type for a message object
    return 'gcs/recv_msg';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'dc444b49bfa57eb79eb16e3052e264e0';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    uint8[263] recv_msg
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new recv_msg(null);
    if (msg.recv_msg !== undefined) {
      resolved.recv_msg = msg.recv_msg;
    }
    else {
      resolved.recv_msg = new Array(263).fill(0)
    }

    return resolved;
    }
};

module.exports = recv_msg;
