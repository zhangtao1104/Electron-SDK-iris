/*
 * @Author: zhangtao@agora.io 
 * @Date: 2021-04-28 13:34:31 
 * @Last Modified by:   zhangtao@agora.io 
 * @Last Modified time: 2021-04-28 13:34:31 
 */

export const deprecate = (originApi?: string, replaceApi?: string) => {
  console.warn(`This method ${originApi} will be deprecated soon. `, replaceApi ? `Please use ${replaceApi} instead` : '');
};

export const TAG = "[Agora]: "

export const logWarn = (msg: string, tag: string = TAG) => {
  console.warn(`${tag} ${msg}`)
}

export const logError = (msg: string, tag: string = TAG) => {
  console.error(`${tag} ${msg}`)
}

export const logInfo = (msg: string, tag: string = TAG) => {
  console.log(`${tag} ${msg}`)
}