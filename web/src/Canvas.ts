import { WasmLoader } from "./WasmLoader";
import { CanvasRenderingContext2D } from "./CanvasRenderingContext2D";

type WebGLContextHandle = number;

export class Canvas {
    private userCanvas: HTMLCanvasElement | OffscreenCanvas | null = null;
    private nativeCanvas: any = undefined;
    private context2D: any = undefined;

    constructor(idOrElement: HTMLCanvasElement | OffscreenCanvas | string) {
        var isHTMLCanvas = typeof HTMLCanvasElement !== 'undefined' && idOrElement instanceof HTMLCanvasElement;
        var isOffscreenCanvas = typeof OffscreenCanvas !== 'undefined' && idOrElement instanceof OffscreenCanvas;
        if (!isHTMLCanvas && !isOffscreenCanvas) {
            if (!document.getElementById(idOrElement as string)) {
                throw 'Canvas with id ' + idOrElement + ' was not found';
            } else {
                this.userCanvas = document.getElementById(idOrElement as string) as HTMLCanvasElement;
            }
        } else {
            this.userCanvas = isHTMLCanvas ? idOrElement as HTMLCanvasElement : idOrElement as OffscreenCanvas;
        }

        this.nativeCanvas = WasmLoader.module.makeCanvas(this.userCanvas!.width, this.userCanvas!.height);
    }

    getContext(contextId: "2d", options?: CanvasRenderingContext2DSettings): CanvasRenderingContext2D | null {
        if (!this.context2D) {
            var ctx = this._getWebGLContext(this.userCanvas!);
            if (!ctx || ctx < 0) {
                throw 'failed to create webgl context: err ' + ctx;
            }
            this.context2D = this.nativeCanvas.get2DContext();
        }
        return this.context2D;
    }

    private _getWebGLContext(canvas: HTMLCanvasElement | OffscreenCanvas): WebGLContextHandle {
        var contextAttributes = {
            'alpha': 1,
            'depth': 1,
            'stencil': 8,
            'antialias': 0,
            'premultipliedAlpha': 1,
            'preserveDrawingBuffer': 0,
            'preferLowPowerToHighPerformance': 0,
            'failIfMajorPerformanceCaveat': 0,
            'enableExtensionsByDefault': 1,
            'explicitSwapControl': 0,
            'renderViaOffscreenBackBuffer': 0,
            'majorVersion': (typeof WebGL2RenderingContext !== 'undefined') ? 2 : 1,
        };
        console.log("_getWebGLContext");
        console.log(canvas);
        var handle = WasmLoader.module.GL.createContext(canvas, contextAttributes);
        if (!handle) {
            throw 'GL.createContext failed';
        }
        WasmLoader.module.GL.makeContextCurrent(handle);
        // Emscripten does not enable this by default and Skia needs this to handle certain GPU corner cases.
        WasmLoader.module.GL.currentContext.GLctx.getExtension('WEBGL_debug_renderer_info');
        return handle;
    }
}