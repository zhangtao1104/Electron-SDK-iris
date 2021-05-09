/*
 * @Author: zhangtao@agora.io 
 * @Date: 2021-04-22 11:38:45 
 * @Last Modified by: zhangtao@agora.io
 * @Last Modified time: 2021-05-09 20:17:15
 */

import {
  Result,
  ApiTypeEngine,
  ApiTypeChannel,
  ApiTypeAudioDeviceManager,
  ApiTypeVideoDeviceManager,
  ApiTypeRawDataPlugin
} from "./native_api_type";

import {WindowInfo, VideoFrame} from "../types";

import { VideoFrameCacheConfig } from '../../renderer/type'

/**
 * interface for c++ addon (.node)
 * @private
 * @ignore
 */
export interface NodeIrisRtcEngine {
  CallApi(
    apiType: ApiTypeEngine,
    params: string
  ): Result;
  CallApiWithBuffer(
    apiType: ApiTypeEngine,
    params: string,
    buffer: string
  ): Result;
  SetAddonLogFile(filePath: string): Result;
  PluginCallApi(apiType: ApiTypeRawDataPlugin, params: string): Result;
  OnEvent(callbackName: string, callback: Function): void;
  GetChannel(): NodeIrisRtcChannel;
  GetDeviceManager(): NodeIrisRtcDeviceManager;
  GetScreenDisplaysInfo(): Array<Object>;
  GetScreenWindowsInfo(): Array<WindowInfo>;
  VideoSourceInitialize(params: string): Result;
  VideoSourceCallApi(
    apiType: ApiTypeEngine,
    params: string
  ): Result;
  VideoSourceCallApiWithBuffer(
    apiType: ApiTypeEngine,
    params: string
  ): Result;
  VideoSourceRelease(): Result;
  VideoSourcePluginCallApi(apiType: ApiTypeRawDataPlugin, params: string): Result;
  EnableVideoFrameCache(config: VideoFrameCacheConfig): number;
  DisableVideoFrameCache(config: VideoFrameCacheConfig): number;
  GetVideoStreamData(streamInfo: VideoFrame): {
    ret: boolean,
    isNewFrame: boolean,
    width: number,
    height: number,
    left: number,
    right: number,
    top: number,
    bottom: number,
    rotation: number,
    timestamp: number
  };
}

/**
 * @private
 * @ignore
 */
export interface NodeIrisRtcChannel {
  CallApi(
    apiType: ApiTypeChannel,
    params: string
  ): Result;
  CallApiWithBuffer(
    apiType: ApiTypeChannel,
    params: string,
    buffer: string
  ): Result;
  OnEvent(callbackName: string, callback: Function): void;
}

/**
 * @ignore
 */
export interface NodeIrisRtcDeviceManager {
  CallApiAudioDevice(
    apiType: ApiTypeAudioDeviceManager,
    params: string
  ): Result;
  CallApiVideoDevice(
    apiType: ApiTypeVideoDeviceManager,
    params: string
  ): Result;
}