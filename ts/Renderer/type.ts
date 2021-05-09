/*
 * @Author: zhangtao@agora.io
 * @Date: 2021-04-28 13:34:39
 * @Last Modified by: zhangtao@agora.io
 * @Last Modified time: 2021-05-09 21:28:14
 */

export enum CONTENT_MODE {
  CROPPED = 0,
  FIT = 1,
}
export interface CanvasOptions {
  frameWidth: number;
  frameHeight: number;
  rotation: number;
  mirror: boolean;
  contentMode: CONTENT_MODE;
  clientWidth: number;
  clientHeight: number;
}

export interface RendererOptions {
  append: boolean;
  contentMode: CONTENT_MODE;
  mirror: boolean;
}

export enum RENDER_MODE {
  WEBGL = 1,
  SOFTWARE = 2,
  CUSTOME = 3,
}

export interface VideoFrameCacheConfig {
  uid: number;
  channelId: string;
  width: number;
  height: number;
}
