import { EmbindObject } from "./WasmLoader";

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
        this._nativeContext?.setLineWidth(newWidth);
    }

    strokeRect(x: number, y: number, width: number, height: number) {
        this._nativeContext?.strokeRect(x, y, width, height);
    }

    fillRect(x: number, y: number, width: number, height: number) {
        this._nativeContext?.fillRect(x, y, width, height);
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