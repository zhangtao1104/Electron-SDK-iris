import { EventEmitter } from 'events';
import { VideoFrame } from './type';

export interface IRenderer {
    event?: EventEmitter;
    bind(element: Element): void;
    unbind(): void;
    equalsElement(element: Element): boolean;
    drawFrame(imageData: VideoFrame): void;
    setContentMode(mode: number): void;
    refreshCanvas(): void;
}