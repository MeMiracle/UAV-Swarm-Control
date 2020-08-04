// Auto-generated. Do not edit!

// (in-package gcs_test.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------

class PubSubUavMsg {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.src_uav_id = null;
      this.component = null;
      this.cmd_priority = null;
      this.playload = null;
      this.start_time = null;
      this.tgt_uav_count = null;
    }
    else {
      if (initObj.hasOwnProperty('src_uav_id')) {
        this.src_uav_id = initObj.src_uav_id
      }
      else {
        this.src_uav_id = 0;
      }
      if (initObj.hasOwnProperty('component')) {
        this.component = initObj.component
      }
      else {
        this.component = '';
      }
      if (initObj.hasOwnProperty('cmd_priority')) {
        this.cmd_priority = initObj.cmd_priority
      }
      else {
        this.cmd_priority = 0;
      }
      if (initObj.hasOwnProperty('playload')) {
        this.playload = initObj.playload
      }
      else {
        this.playload = '';
      }
      if (initObj.hasOwnProperty('start_time')) {
        this.start_time = initObj.start_time
      }
      else {
        this.start_time = 0.0;
      }
      if (initObj.hasOwnProperty('tgt_uav_count')) {
        this.tgt_uav_count = initObj.tgt_uav_count
      }
      else {
        this.tgt_uav_count = 0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type PubSubUavMsg
    // Serialize message field [src_uav_id]
    bufferOffset = _serializer.int32(obj.src_uav_id, buffer, bufferOffset);
    // Serialize message field [component]
    bufferOffset = _serializer.string(obj.component, buffer, bufferOffset);
    // Serialize message field [cmd_priority]
    bufferOffset = _serializer.int32(obj.cmd_priority, buffer, bufferOffset);
    // Serialize message field [playload]
    bufferOffset = _serializer.string(obj.playload, buffer, bufferOffset);
    // Serialize message field [start_time]
    bufferOffset = _serializer.float64(obj.start_time, buffer, bufferOffset);
    // Serialize message field [tgt_uav_count]
    bufferOffset = _serializer.int32(obj.tgt_uav_count, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type PubSubUavMsg
    let len;
    let data = new PubSubUavMsg(null);
    // Deserialize message field [src_uav_id]
    data.src_uav_id = _deserializer.int32(buffer, bufferOffset);
    // Deserialize message field [component]
    data.component = _deserializer.string(buffer, bufferOffset);
    // Deserialize message field [cmd_priority]
    data.cmd_priority = _deserializer.int32(buffer, bufferOffset);
    // Deserialize message field [playload]
    data.playload = _deserializer.string(buffer, bufferOffset);
    // Deserialize message field [start_time]
    data.start_time = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [tgt_uav_count]
    data.tgt_uav_count = _deserializer.int32(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += object.component.length;
    length += object.playload.length;
    return length + 28;
  }

  static datatype() {
    // Returns string type for a message object
    return 'gcs_test/PubSubUavMsg';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '67891a76d6771782154f9e5d90983fb9';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    int32 src_uav_id
    string component
    int32 cmd_priority
    string playload
    float64 start_time
    int32 tgt_uav_count
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new PubSubUavMsg(null);
    if (msg.src_uav_id !== undefined) {
      resolved.src_uav_id = msg.src_uav_id;
    }
    else {
      resolved.src_uav_id = 0
    }

    if (msg.component !== undefined) {
      resolved.component = msg.component;
    }
    else {
      resolved.component = ''
    }

    if (msg.cmd_priority !== undefined) {
      resolved.cmd_priority = msg.cmd_priority;
    }
    else {
      resolved.cmd_priority = 0
    }

    if (msg.playload !== undefined) {
      resolved.playload = msg.playload;
    }
    else {
      resolved.playload = ''
    }

    if (msg.start_time !== undefined) {
      resolved.start_time = msg.start_time;
    }
    else {
      resolved.start_time = 0.0
    }

    if (msg.tgt_uav_count !== undefined) {
      resolved.tgt_uav_count = msg.tgt_uav_count;
    }
    else {
      resolved.tgt_uav_count = 0
    }

    return resolved;
    }
};

module.exports = PubSubUavMsg;
