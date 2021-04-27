const YUVBuffer = require('yuv-buffer');
const YUVCanvas = require('yuv-canvas')
const isEqual = require("lodash.isequal");

import { CONTENT_MODE } from "../Api/types";
import { VideoFrame, CanvasOptions } from "./type";
import { IRenderer } from './IRender'
import {EventEmitter} from 'events'



export class Renderer implements IRenderer{
  private _cacheCanvasOptions?: CanvasOptions;
  private _yuvCanvasSink?: any;
  private _ready: boolean;
  private _contentMode: CONTENT_MODE;
  private _canvas?: HTMLCanvasElement;
  private _customeElement?: Element;
  private _container?: Element;
  private _isWebGL: boolean;
  event?: EventEmitter;

  constructor(isWebGL: boolean) {
    this._yuvCanvasSink = {};
    this._ready = false;
    this._contentMode = CONTENT_MODE.CROPPED;
    this._isWebGL = isWebGL;
  }

  bind(element: Element) {
    this._customeElement = element;
    let container = document.createElement("div");
    Object.assign(container.style, {
      width: "100%",
      height: "100%",
      display: "flex",
      justifyContent: "center",
      alignItems: "center",
    });
    this._container = container;
    this._customeElement.appendChild(this._container);
    this._canvas = document.createElement("canvas");
    this._container.appendChild(this._canvas);
    this._yuvCanvasSink = YUVCanvas.attah(this._canvas, {
      webGL: this._isWebGL,
    });
  }

  unbind() {
    this._container &&
      this._canvas &&
      this._container.removeChild(this._canvas);
    this._customeElement &&
      this._container &&
      this._customeElement.removeChild(this._container);
    this._yuvCanvasSink && this._yuvCanvasSink.loseContext();
    this._yuvCanvasSink = {};
    this._canvas && (this._canvas = undefined);
    this._container && (this._container = undefined);
  }

  zoom(
    vertical: boolean,
    contentMode: CONTENT_MODE = CONTENT_MODE.CROPPED,
    width: number,
    height: number,
    clientWidth: number,
    clientHeight: number
  ): number {
    let localRatio = clientWidth / clientHeight;
    let tempRatio = width / height;
    if (isNaN(localRatio) || isNaN(tempRatio)) {
      return 1;
    }

    if (contentMode === CONTENT_MODE.CROPPED) {
      if (vertical) {
        return clientHeight / clientWidth < width / height
          ? clientWidth / height
          : clientHeight / width;
      } else {
        return clientWidth / clientHeight > width / height
          ? clientWidth / width
          : clientHeight / height;
      }
    } else {
      if (vertical) {
        return clientHeight / clientWidth < width / height
          ? clientHeight / width
          : clientWidth / height;
      } else {
        return clientWidth / clientHeight > width / height
          ? clientHeight / height
          : clientWidth / width;
      }
    }
  }

  updateCanvas(
    options: CanvasOptions = {
      frameWidth: 0,
      frameHeight: 0,
      rotation: 0,
      mirror: false,
      contentMode: 0,
      clientWidth: 0,
      clientHeight: 0,
    }
  ) {
    if (this._cacheCanvasOptions) {
      if (isEqual(this._cacheCanvasOptions, options)) {
        return;
      }
    }

    this._cacheCanvasOptions = Object.assign({}, options);

    if (this._canvas) {
      if (options.rotation === 0 || options.rotation === 180) {
        this._canvas.width = options.frameWidth;
        this._canvas.height = options.frameHeight;
        Object.assign(this._canvas.style, {
          width: options.frameWidth + "px",
          height: options.frameHeight + "px",
          "object-fit": "cover",
        });
      } else if (options.rotation === 90 || options.rotation === 270) {
        this._canvas.height = options.frameWidth;
        this._canvas.width = options.frameHeight;
      } else {
        throw new Error(
          "Invalid value for rotation. Only support 0, 90, 180, 270"
        );
      }

      let transformItems = [];
      transformItems.push(`rotateZ(${options.rotation}deg)`);

      let scale = this.zoom(
        options.rotation === 90 || options.rotation === 270,
        options.contentMode,
        options.frameWidth,
        options.frameHeight,
        options.clientWidth,
        options.clientHeight
      );

      this._canvas.style.zoom = scale.toString();

      if (options.mirror) {
        transformItems.push("rotateY(180deg)");
      }

      if (transformItems.length > 0) {
        let transform = `${transformItems.join(" ")}`;
        this._canvas.style.transform = transform;
      }
    }
  }

  drawFrame(frame: VideoFrame) {
    if (!this._container || !this._yuvCanvasSink) {
        return
    }

      if (!this._ready) {
          this._ready = true
      }

      let frameWidth = frame.width + frame.left + frame.right
      let frameHeight = frame.height + frame.top + frame.bottom

      let options: CanvasOptions = {
        frameWidth: frameWidth,
        frameHeight: frameHeight,
        rotation: frame.rotation,
        mirror: frame.mirror,
        contentMode: this._contentMode,
        clientWidth: this._container.clientWidth,
        clientHeight: this._container.clientHeight
      }

      this.updateCanvas(options)

      let format: object = YUVBuffer.format({
          frameWidth,
          frameHeight,
          chromaWidth: frameWidth/2,
          chromaHeight: frameHeight/2,
      });

      let y = YUVBuffer.lumaPlane(format, frame.yBuffer)
      let u = YUVBuffer.chromaPlane(format, frame.uBuffer)
      let v = YUVBuffer.chromaPlane(format, frame.vBuffer)
      let yuvBufferFrame = YUVBuffer.frame(format, y, u, v);
      this._yuvCanvasSink.drawFrame(yuvBufferFrame);
  }

  setContentMode(mode: CONTENT_MODE = CONTENT_MODE.CROPPED) {
      this._contentMode = mode
  }

  equalsElement(element: Element) {
    return this._customeElement ? false : this._customeElement === element
  }

  refreshCanvas() {

  }
}
