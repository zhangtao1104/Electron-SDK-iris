/*
 * @Author: zhangtao@agora.io 
 * @Date: 2021-04-22 11:38:45 
 * @Last Modified by: zhangtao@agora.io
 * @Last Modified time: 2021-04-28 13:37:33
 */

import {
  Result,
  ApiTypeEngine,
  ApiTypeChannel,
  ApiTypeAudioDeviceManager,
  ApiTypeVideoDeviceManager,
  ApiTypeRawDataPlugin,
} from "./native_api_type";

import {WindowInfo} from "../types";

/**
 * interface for c++ addon (.node)
 * @private
 * @ignore
 */
export interface NodeIrisEngine {
  CallApi(
    apiType: ApiTypeEngine,
    params: string
  ): Result;
  CallApiWithBuffer(
    apiType: ApiTypeEngine,
    params: string,
    buffer: string
  ): Result;
  OnEvent(callbackName: string, callback: Function): void;
  GetChannel(): NodeIrisChannel;
  GetDeviceManager(): NodeIrisDeviceManager;
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
  SetAddonLogFile(filePath: string): Result;
  PluginCallApi(apiType: ApiTypeRawDataPlugin, params: string): Result;
}

/**
 * @private
 * @ignore
 */
export interface NodeIrisChannel {
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
export interface NodeIrisDeviceManager {
  CallApiAudioDevice(
    apiType: ApiTypeAudioDeviceManager,
    params: string
  ): Result;
  CallApiVideoDevice(
    apiType: ApiTypeVideoDeviceManager,
    params: string
  ): Result;
}
