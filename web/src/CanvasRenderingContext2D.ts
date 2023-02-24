
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

    set globalAlpha(alpha: number) {
        if (!isFinite(alpha)) {
            return;
        }
        this._nativeContext?.setGlobalAlpha(alpha);
    }

    set globalCompositeOperation(operation: string) {
        this._nativeContext?.setGlobalCompositeOperation(operation);
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
}