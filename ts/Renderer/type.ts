import { CONTENT_MODE } from "ts/Api/types";

export interface VideoFrame {
    left: number;
    right: number
    top: number;
    bottom: number;
    width: number;
    height: number;
    yBuffer: Uint8Array;
    uBuffer: Uint8Array;
    vBuffer: Uint8Array;
    mirror: boolean;
    rotation: number;
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