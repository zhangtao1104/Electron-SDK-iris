import { VideoFrame, User, Channel } from "../Api/types";
import { IRenderer } from "./IRender";
import { YUVCanvasRenderer } from "./YUVCanvasRenderer";
import { NodeIrisRtcEngine } from '../Api/internal/native_interface'
import {
  RendererOptions,
  RENDER_MODE,
  CONTENT_MODE,
  VideoFrameCacheConfig,
} from "./type";
import { logInfo, logWarn, logError } from "../Utils";

class RendererManager {
  _config: {
    videoFps: number;
    videoFrameUpdateInterval?: NodeJS.Timeout;
    renderers: Map<
      string,
      Map<string, { render?: IRenderer[], cachedVideoFrame?: VideoFrame }>
    >;
    renderMode: RENDER_MODE;
  };
  _rtcEngine: NodeIrisRtcEngine

  constructor(rtcEngine: NodeIrisRtcEngine) {
    this._config = {
      videoFps: 10,
      renderers: new Map(),
      renderMode: this._checkWebGL() ? RENDER_MODE.WEBGL : RENDER_MODE.SOFTWARE,
    };
    this._rtcEngine = rtcEngine
  }

  /**
   * @private
   * @ignore
   * check if WebGL will be available with appropriate features
   * @return {boolean}
   */
  _checkWebGL(): boolean {
    let gl;
    const canvas = document.createElement("canvas");
    canvas.width = 1;
    canvas.height = 1;
    const options = {
      // Turn off things we don't need
      alpha: false,
      depth: false,
      stencil: false,
      antialias: false,
      preferLowPowerToHighPerformance: true,
    };

    try {
      gl =
        canvas.getContext("webgl", options) ||
        canvas.getContext("experimental-webgl", options);
    } catch (e) {
      logWarn("webGL not support");
      return false;
    }

    if (gl) {
      return true;
    } else {
      return false;
    }
  }

  setRenderMode(mode: RENDER_MODE) {
    this._config.renderMode = mode
  }

  resizeBuffer(
    uid: number,
    channelId: string,
    width: number,
    height: number
  ): VideoFrame {
    return {
      uid,
      channelId,
      yBuffer: Buffer.alloc(width * height),
      uBuffer: Buffer.alloc((width * height) / 4),
      vBuffer: Buffer.alloc((width * height) / 4),
      width,
      height,
    };
  }

  /**
   * @private
   * @ignore
   */
  setRenderer(
    user: User,
    view: Element,
    channelId: Channel = "",
    options: RendererOptions = {
      append: false,
      contentMode: CONTENT_MODE.FIT,
      mirror: false,
    }
  ): void {
    let _renders = this.getRenderer(user, channelId);
    if (_renders) {
      options.append
        ? _renders.forEach((item) => {
            if (item.equalsElement(view)) {
              console.warn("setVideoView: this view exists in list, ignore");
              return;
            }
          })
        : this.removeRenderer(user, channelId);
    }

    this.enableVideoFrameCache(user, channelId, 0, 0);
    this.addRenderer(user, view, channelId);
    this.setupViewContentMode(
      user,
      channelId,
      options.contentMode,
      options.mirror
    );
  }

  /**
   * @private
   * @ignore
   */
  createRenderer(): IRenderer {
    return new YUVCanvasRenderer(this._config.renderMode === RENDER_MODE.WEBGL);
  }

  /**
   * @private
   * @ignore
   */
  addRenderer(user: User, view: Element, channelId: Channel): void {
    let rendererMap = this.ensureRendererMap(user, channelId);
    if (!rendererMap?.get(String(user))) {
      rendererMap?.set(String(user), { render: [] });
    }

    let renderer = this.createRenderer();
    renderer.bind(view);
    rendererMap?.get(String(user))?.render?.push(renderer);
  }

  /**
   * @private
   * @ignore
   */
  getRenderer(user: User, channelId: Channel = ""): IRenderer[] | undefined {
    return this._config.renderers.get(channelId)?.get(String(user))?.render;
  }

  removeRenderer(user: User, channelId: Channel = ""): void {
    this.disableVideoFrameCache(user, channelId, 0, 0);
    this.removeVideoFrameCacheFromMap(user, channelId);
    this._config.renderers
      .get(channelId)
      ?.get(String(user))
      ?.render?.forEach((renderItem) => {
        renderItem.unbind();
      });

    this._config.renderers.get(channelId)?.delete(String(user));
  }

  removeAllRenderer(): void {
    this._config.renderers.forEach((renderMap, channelId) => {
      renderMap.forEach((renderObject, user) => {
        this.disableVideoFrameCache(user, channelId, 0, 0);
        this.removeVideoFrameCacheFromMap(user, channelId);
        renderObject.render?.forEach((renderItem) => {
          renderItem.unbind();
        });
        renderMap.delete(user);
      });
      this._config.renderers.delete(channelId);
    });
    this._config.renderers.clear()
  }

  /**
   * @private
   * @ignore
   */
  startRenderer(): void {
    this._config.videoFrameUpdateInterval = setInterval(() => {
      this._config.renderers.forEach((rendererCache, channelId) => {
        rendererCache.forEach((rendererItem, user) => {
          let cachedVideoFrame = rendererItem.cachedVideoFrame;
          if (!cachedVideoFrame) {
            logWarn(
              `Channel: ${channelId} User: ${user} have no cachedVideoFrame`
            );
            return;
          }

          let retObj = this._rtcEngine.GetVideoStreamData(cachedVideoFrame);

          if (!retObj.ret) {
            logWarn(
              `Channel: ${channelId} User: ${user} uid: ${cachedVideoFrame.uid} have no stream`
            );
            return;
          }

          if (cachedVideoFrame.uid != null) {
            let user = this.uidToUser(cachedVideoFrame.uid);
            let render = this.getRenderer(user, cachedVideoFrame.channelId);
            let videoFrame: VideoFrame = {
              left: retObj.left,
              right: retObj.right,
              top: retObj.top,
              bottom: retObj.bottom,
              width: retObj.width,
              height: retObj.height,
              yBuffer: cachedVideoFrame.yBuffer as Buffer,
              uBuffer: cachedVideoFrame.uBuffer as Buffer,
              vBuffer: cachedVideoFrame.vBuffer as Buffer,
              mirror: false,
              rotation: retObj.rotation,
            };

            if (render) {
              render.forEach((renderItem) => {
                renderItem.drawFrame(videoFrame);
              });
            } else {
              logWarn(`Channel: ${channelId} User: ${user} have no renderer`);
            }
          }
        });
      });
    }, 1000 / this._config.videoFps);
  }

  /**
   * @private
   * @ignore
   */
  stopRenderer(): void {
    this._config.videoFrameUpdateInterval
      ? clearInterval(this._config.videoFrameUpdateInterval)
      : logWarn("video stream interval is not start!");

    this._config.videoFrameUpdateInterval = undefined;
  }

  /**
   * @private
   * @ignore
   */
  restartRenderer(): void {
    let self = this;
    this._config.videoFrameUpdateInterval
      ? () => {
          self.stopRenderer();
          self.startRenderer();
          logInfo(`setFps ${this._config.videoFps} restartInterval`);
        }
      : logInfo(`setFps ${this._config.videoFps}`);
  }

  userToUid(user: User): number {
    let uid;
    if (user === "local") {
      uid = 0;
    } else if (user === "videoSource") {
      uid = 0;
    } else {
      uid = user as number;
    }

    return uid;
  }

  uidToUser(uid: number): User {
    let user: User;
    if (uid === 0) {
      user = "local";
    } else {
      user = String(uid);
    }
    /**
     * @todo
     */
    return user;
  }

  enableVideoFrameCache(
    user: User,
    channelId: string,
    width: number,
    height: number
  ): number {
    let uid = this.userToUid(user);
    let config: VideoFrameCacheConfig = {
      uid,
      channelId,
      width,
      height,
    };

    logInfo(`enableVideoFrameCache ${JSON.stringify(config)}`);
    return this._rtcEngine.EnableVideoFrameCache(config);
  }

  disableVideoFrameCache(
    user: User,
    channelId: string,
    width: number,
    height: number
  ): number {
    let uid = this.userToUid(user);
    let config: VideoFrameCacheConfig = {
      uid,
      channelId,
      width,
      height,
    };

    logInfo(`disableVideoFrameCache ${JSON.stringify(config)}`);
    return this._rtcEngine.DisableVideoFrameCache(config);
  }

  ensureRendererMap(
    user: User,
    channelId: string
  ):
    | Map<string, { cachedVideoFrame?: VideoFrame; render?: IRenderer[] }>
    | undefined {
    let rendererMap = this._config.renderers.get(channelId);
    if (!rendererMap) {
      logWarn(`ensureRendererMap for ${channelId}  ${user}`);
      this._config.renderers.set(channelId, new Map());
    }
    rendererMap = this._config.renderers.get(channelId);
    return rendererMap;
  }

  addVideoFrameCacheToMap(
    user: User,
    channelId: string,
    videoFrame: VideoFrame
  ): void {
    let rendererMap = this.ensureRendererMap(user, channelId);
    rendererMap
      ? Object.assign(rendererMap.get(String(user)), {
          cachedVideoFrame: videoFrame,
        })
      : logWarn(
          `addVideoFrameCacheToMap rendererMap ${channelId}  ${user} is null`
        );
  }

  removeVideoFrameCacheFromMap(user: User, channelId: string): void {
    let rendererMap = this._config.renderers.get(channelId);
    let rendererItem = rendererMap?.get(String(user));
    rendererItem
      ? Object.assign(rendererItem, { cachedVideoFrame: undefined })
      : logWarn(
          `removeVideoFrameCacheFromMap rendererItem ${channelId}  ${user} is null`
        );
  }

  setupViewContentMode(
    user: User,
    channelId: Channel = "",
    mode: CONTENT_MODE = CONTENT_MODE.FIT,
    mirror: boolean = false
  ): number {
    let renderList = this.getRenderer(user, channelId);
    renderList
      ? renderList.forEach((renderItem) =>
          renderItem.setContentMode(mode, mirror)
        )
      : console.warn(
          `User: ${user} have no render view, you need to call this api after setView`
        );
    return 0;
  }
}

export { RendererManager };
