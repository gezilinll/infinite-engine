import { CanvasLoader } from "./CanvasLoader";

export class CanvasRenderingContext2D {
    private _nativeContext: any = undefined;

    constructor(nativeContext: any) {
        this._nativeContext = nativeContext;
    }

    delete() {
        this._nativeContext?.delete();
        this._nativeContext = undefined;
    }

    set lineWidth(newWidth: number) {
        if (!isFinite(newWidth)) {
            return;
        }
        this._nativeContext?.setLineWidth(newWidth);
    }

    set shadowOffsetX(offset: number) {
        if (!isFinite(offset)) {
            return;
        }
        this._nativeContext?.setShadowOffsetX(offset);
    }

    set shadowOffsetY(offset: number) {
        if (!isFinite(offset)) {
            return;
        }
        this._nativeContext?.setShadowOffsetY(offset);
    }

    set shadowBlur(blurValue: number) {
        if (!isFinite(blurValue)) {
            return;
        }
        this._nativeContext?.setShadowBlur(blurValue);
    }

    set shadowColor(color: string) {
        this._nativeContext?.setShadowColor(color);
    }

    set strokeStyle(style: string) {
        this._nativeContext?.setStrokeStyleColor(style);
    }

    set fillStyle(style: string) {
        this._nativeContext?.setFillStyleColor(style);
    }

    set globalAlpha(alpha: number) {
        if (!isFinite(alpha)) {
            return;
        }
        this._nativeContext?.setGlobalAlpha(alpha);
    }

    set lineCap(cap: string) {
        this._nativeContext?.setLineCap(cap);
    }

    set lineDashOffset(value: number) {
        this._nativeContext?.setLineDashOffset(value);
    }

    setLineDash(segments: number[]) {
        var ptr = CanvasLoader.module._malloc(segments.length * CanvasLoader.module['HEAPF32'].BYTES_PER_ELEMENT);
        CanvasLoader.module.HEAPF32.set(segments, ptr / CanvasLoader.module['HEAPF32'].BYTES_PER_ELEMENT);
        this._nativeContext?.setLineDash(ptr, segments.length);
        CanvasLoader.module._free(ptr);
    }

    set globalCompositeOperation(operation: string) {
        this._nativeContext?.setGlobalCompositeOperation(operation);
    }

    set direction(direction: string) {
        this._nativeContext?.setDirection(direction);
    }

    arc(x: number, y: number, radius: number, startAngle: number, endAngle: number, anticlockwise: boolean) {
        // Note input angles are radians.
        this._nativeContext?.arc(x, y, radius, startAngle, endAngle, anticlockwise);
    }

    fill() {
        this._nativeContext?.fill();
    }

    strokeRect(x: number, y: number, width: number, height: number) {
        this._nativeContext?.strokeRect(x, y, width, height);
    }

    fillRect(x: number, y: number, width: number, height: number) {
        this._nativeContext?.fillRect(x, y, width, height);
    }

    fillText(text: string, x: number, y: number, maxWidth?: number) {
        this._nativeContext?.fillText(text, x, y, 0);
    }

    beginPath() {
        this._nativeContext?.beginPath();
    }

    closePath() {
        this._nativeContext?.closePath();
    }

    moveTo(x: number, y: number) {
        this._nativeContext?.moveTo(x, y);
    }

    lineTo(x: number, y: number) {
        this._nativeContext?.lineTo(x, y);
    }

    stroke() {
        this._nativeContext?.stroke();
    }

    clearRect(x: number, y: number, width: number, height: number) {
        this._nativeContext?.clearRect(x, y, width, height);
    }

    flush() {
        this._nativeContext?.flush();
    }
}