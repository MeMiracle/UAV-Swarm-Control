
"use strict";

let SDKControlAuthority = require('./SDKControlAuthority.js')
let MissionStatus = require('./MissionStatus.js')
let StereoVGASubscription = require('./StereoVGASubscription.js')
let MissionHpGetInfo = require('./MissionHpGetInfo.js')
let MissionWpAction = require('./MissionWpAction.js')
let Activation = require('./Activation.js')
let CameraAction = require('./CameraAction.js')
let Stereo240pSubscription = require('./Stereo240pSubscription.js')
let MissionWpGetInfo = require('./MissionWpGetInfo.js')
let MissionHpAction = require('./MissionHpAction.js')
let StereoDepthSubscription = require('./StereoDepthSubscription.js')
let MissionHpUpload = require('./MissionHpUpload.js')
let DroneTaskControl = require('./DroneTaskControl.js')
let MFIOConfig = require('./MFIOConfig.js')
let MFIOSetValue = require('./MFIOSetValue.js')
let MissionWpUpload = require('./MissionWpUpload.js')
let MissionHpUpdateRadius = require('./MissionHpUpdateRadius.js')
let SendMobileData = require('./SendMobileData.js')
let DroneArmControl = require('./DroneArmControl.js')
let SetHardSync = require('./SetHardSync.js')
let MissionWpGetSpeed = require('./MissionWpGetSpeed.js')
let SetLocalPosRef = require('./SetLocalPosRef.js')
let MissionHpUpdateYawRate = require('./MissionHpUpdateYawRate.js')
let QueryDroneVersion = require('./QueryDroneVersion.js')
let MissionHpResetYaw = require('./MissionHpResetYaw.js')
let MissionWpSetSpeed = require('./MissionWpSetSpeed.js')

module.exports = {
  SDKControlAuthority: SDKControlAuthority,
  MissionStatus: MissionStatus,
  StereoVGASubscription: StereoVGASubscription,
  MissionHpGetInfo: MissionHpGetInfo,
  MissionWpAction: MissionWpAction,
  Activation: Activation,
  CameraAction: CameraAction,
  Stereo240pSubscription: Stereo240pSubscription,
  MissionWpGetInfo: MissionWpGetInfo,
  MissionHpAction: MissionHpAction,
  StereoDepthSubscription: StereoDepthSubscription,
  MissionHpUpload: MissionHpUpload,
  DroneTaskControl: DroneTaskControl,
  MFIOConfig: MFIOConfig,
  MFIOSetValue: MFIOSetValue,
  MissionWpUpload: MissionWpUpload,
  MissionHpUpdateRadius: MissionHpUpdateRadius,
  SendMobileData: SendMobileData,
  DroneArmControl: DroneArmControl,
  SetHardSync: SetHardSync,
  MissionWpGetSpeed: MissionWpGetSpeed,
  SetLocalPosRef: SetLocalPosRef,
  MissionHpUpdateYawRate: MissionHpUpdateYawRate,
  QueryDroneVersion: QueryDroneVersion,
  MissionHpResetYaw: MissionHpResetYaw,
  MissionWpSetSpeed: MissionWpSetSpeed,
};
