import { InfiniteLoader } from "./InfiniteLoader";

type WebGLContextHandle = number;

export class InfiniteEngine {
    private _userCanvas: HTMLCanvasElement | OffscreenCanvas | null = null;

    constructor(idOrElement: HTMLCanvasElement | OffscreenCanvas | string) {
        // var isHTMLCanvas = typeof HTMLCanvasElement !== 'undefined' && idOrElement instanceof HTMLCanvasElement;
        // var isOffscreenCanvas = typeof OffscreenCanvas !== 'undefined' && idOrElement instanceof OffscreenCanvas;
        // if (!isHTMLCanvas && !isOffscreenCanvas) {
        //     if (!document.getElementById(idOrElement as string)) {
        //         throw 'Canvas with id ' + idOrElement + ' was not found';
        //     } else {
        //         this._userCanvas = document.getElementById(idOrElement as string) as HTMLCanvasElement;
        //     }
        // } else {
        //     this._userCanvas = isHTMLCanvas ? idOrElement as HTMLCanvasElement : idOrElement as OffscreenCanvas;
        // }

        // this._nativeCanvas = CanvasLoader.module.makeCanvas(this._userCanvas!.width, this._userCanvas!.height);
    }

    delete() {

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
        var handle = CanvasLoader.module.GL.createContext(canvas, contextAttributes);
        if (!handle) {
            throw 'GL.createContext failed';
        }
        CanvasLoader.module.GL.makeContextCurrent(handle);
        // Emscripten does not enable this by default and Skia needs this to handle certain GPU corner cases.
        CanvasLoader.module.GL.currentContext.GLctx.getExtension('WEBGL_debug_renderer_info');
        return handle;
    }
}