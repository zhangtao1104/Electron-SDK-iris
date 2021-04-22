/*
 * @Author: zhangtao@agora.io 
 * @Date: 2021-04-22 11:38:45 
 * @Last Modified by: zhangtao@agora.io
 * @Last Modified time: 2021-04-22 11:39:16
 */


import {
  ApiTypeEngine,
  ApiTypeChannel,
  ApiTypeAudioDeviceManager,
  ApiTypeVideoDeviceManager,
} from "./native_api_type";

/**
 * interface for c++ addon (.node)
 * @ignore
 */
export interface NodeIrisEngine {
  CallApi(
    apiType: ApiTypeEngine,
    params: string
  ): { retCode: number; result: string };
  CallApiWithBuffer(
    apiType: ApiTypeEngine,
    params: string,
    buffer: string
  ): { retCode: number; result: string };
  OnEvent(callbackName: string, callback: Function): void;
  GetChannel(): NodeIrisChannel;
  GetDeviceManager(): NodeIrisDeviceManager;
  GetScreenDisplaysInfo(): Array<Object>;
  GetScreenWindowsInfo(): Array<Object>;
  VideoSourceInitialize(params: string): { retCode: number; result: string };
  VideoSourceCallApi(
    apiType: ApiTypeEngine,
    params: string
  ): { retCode: number; result: string };
  VideoSourceCallApiWithBuffer(
    apiType: ApiTypeEngine,
    params: string
  ): { retCode: number; result: string };
  VideoSourceRelease(): { retCode: number; result: string };
  SetAddonLogFile(filePath: string): { retCode: number; result: number };
}

/**
 * @ignore
 */
export interface NodeIrisChannel {
  CallApi(
    apiType: ApiTypeChannel,
    params: string
  ): { retCode: number; result: string };
  CallApiWithBuffer(
    apiType: ApiTypeChannel,
    params: string,
    buffer: string
  ): { retCode: number; result: string };
  OnEvent(callbackName: string, callback: Function): void;
}

/**
 * @ignore
 */
export interface NodeIrisDeviceManager {
  CallApiAudioDevice(
    apiType: ApiTypeAudioDeviceManager,
    params: string
  ): { retCode: number; result: string };
  CallApiVideoDevice(
    apiType: ApiTypeVideoDeviceManager,
    params: string
  ): { retCode: number; result: string };
}
