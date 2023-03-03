import { ElementRBush } from "./base/ElementRBush";
import { Element } from "./element/Element";

export class DebugFrame {
    sceneTree?: ElementRBush;
    totalElementCount: number = 0;
    newElementCount: number = 0;
    changedElementCount: number = 0;
    updatedElementCount: number = 0;
}

export class Debuger {
    private _drawSceneEnabled: boolean = false;
    private _baseCanvas: HTMLCanvasElement | null = null;
    private _debugCanvas: HTMLCanvasElement | null = null;
    private _messageOutput?: string;
    private _startTime: number = 0;

    set baseCanvas(canvas: HTMLCanvasElement) {
        this._baseCanvas = canvas;
    }

    set messageOutput(elementID: string) {
        console.log(elementID);
        this._messageOutput = elementID;
    }

    set drawScene(enabled: boolean) {
        this._drawSceneEnabled = enabled;
    }

    onRenderStart() {
        this._startTime = +new Date();
    }

    onRenderEnd(frame: DebugFrame) {
        // if (this._drawSceneEnabled && this._baseCanvas) {
        //     if (!this._debugCanvas) {
        //         this._debugCanvas = document.createElement('canvas');
        //         this._debugCanvas.width = this.baseCanvas.width;
        //         this._debugCanvas.height = this.baseCanvas.height;
        //         this.baseCanvas.parentNode!.appendChild(this._debugCanvas);
        //     }
        // }
        console.log(this._messageOutput);
        if (this._messageOutput) {
            let endTime = +new Date();
            let message = "元素总量：" + frame.totalElementCount
                + " 新增元素：" + frame.newElementCount
                + " 变更元素：" + frame.changedElementCount
                + " 联动更新元素：" + frame.updatedElementCount
                + " 渲染耗时：" + (endTime - this._startTime) + "ms";
            document.getElementById(this._messageOutput)!.textContent = message;
        }
    }
}