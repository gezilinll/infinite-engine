import { CanvasLoader } from "./CanvasLoader";

export class CanvasRenderingContext2D {
    private _nativeContext: any = undefined;
    private _freeCanvas: HTMLCanvasElement | null = null;

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

    set lineJoin(join: string) {
        this._nativeContext?.setLineJoin(join);
    }

    set lineDashOffset(value: number) {
        this._nativeContext?.setLineDashOffset(value);
    }

    set miterLimit(value: number) {
        if (!value || !isFinite(value)) {
            return;
        }
        this._nativeContext?.setMiterLimit(value);
    }

    set font(value: string) {
        this._nativeContext?.setFont(value);
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

    strokeText(text: string, x: number, y: number, maxWidth?: number) {
        this._nativeContext?.strokeText(text, x, y, 0);
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

    rect(x: number, y: number, width: number, height: number) {
        this._nativeContext?.rect(x, y, width, height);
    }

    stroke() {
        this._nativeContext?.stroke();
    }

    clearRect(x: number, y: number, width: number, height: number) {
        this._nativeContext?.clearRect(x, y, width, height);
    }

    drawImage(image: CanvasImageSource, ...args: number[]) {
        let width = image.width as number;
        let height = image.height as number;
        if (!this._freeCanvas) {
            this._freeCanvas = document.createElement('canvas');
        }
        let ctx = this._freeCanvas.getContext('2d', { willReadFrequently: true });
        this._freeCanvas.width = width;
        this._freeCanvas.height = height;
        ctx!.drawImage(image, 0, 0);

        let imageData = ctx!.getImageData(0, 0, width, height);
        var pptr = CanvasLoader.module._malloc(4 * width * height);
        CanvasLoader.module.HEAPU8.set(imageData.data, pptr); // We always want to copy the bytes into the WASM heap.
        // No need to _free pptr, Image takes it with SkData::MakeFromMalloc
        if (arguments.length === 3 || arguments.length === 5) {
            this._nativeContext?.drawImageWithoutClip(pptr, width, height, imageData.data.length, 4 * width,
                arguments[1], arguments[2], arguments[3] || width, arguments[4] || height);
        } else if (arguments.length === 9) {
            this._nativeContext?.drawImageWithClip(pptr, width, height, imageData.data.length, 4 * width,
                arguments[1], arguments[2], arguments[3], arguments[4],
                arguments[5], arguments[6], arguments[7], arguments[8]);
        } else {
            throw 'invalid number of args for drawImage, need 3, 5, or 9; got ' + arguments.length;
        }
    }

    flush() {
        this._nativeContext?.flush();
    }
}