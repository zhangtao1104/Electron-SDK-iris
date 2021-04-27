/*
 * @Author: zhangtao@agora.io 
 * @Date: 2021-04-22 20:52:07 
 * @Last Modified by: zhangtao@agora.io
 * @Last Modified time: 2021-04-24 13:33:10
 */

/** 
 * @ignore
 */
export interface PluginInfo {
  pluginId: string;
  pluginPath: string;
}

/** 
 * @ignore
 */
export interface Plugin {
  id: string;
  enable: () => number;
  disable: () => number;
  setParameter: (param: string) => number;
  getParameter: (paramKey: string) => string;
}