import { Element } from "./element/Element";
import { ImageElement } from "./element/ImageElement";
import { InfiniteLoader } from "./InfiniteLoader";

type WebGLContextHandle = number;

export class InfiniteEngine {
    private _userCanvas: HTMLCanvasElement | OffscreenCanvas | null = null;
    private _nativeEngine: any = undefined;
    private _glContextHandle: any = undefined;
    private _screenCanvas: HTMLCanvasElement | null = null;
    private elementCount: number = 0;

    constructor(idOrElement: HTMLCanvasElement | OffscreenCanvas | string) {
        var isHTMLCanvas = typeof HTMLCanvasElement !== 'undefined' && idOrElement instanceof HTMLCanvasElement;
        var isOffscreenCanvas = typeof OffscreenCanvas !== 'undefined' && idOrElement instanceof OffscreenCanvas;
        if (!isHTMLCanvas && !isOffscreenCanvas) {
            if (!document.getElementById(idOrElement as string)) {
                throw 'Canvas with id ' + idOrElement + ' was not found';
            } else {
                this._userCanvas = document.getElementById(idOrElement as string) as HTMLCanvasElement;
                isHTMLCanvas = true;
            }
        } else {
            this._userCanvas = isHTMLCanvas ? idOrElement as HTMLCanvasElement : idOrElement as OffscreenCanvas;
        }
        if (isHTMLCanvas) {
            this._screenCanvas = document.createElement('canvas');
            this._screenCanvas.width = this._userCanvas.width;
            this._screenCanvas.height = this._userCanvas.height;
            (this._userCanvas as HTMLCanvasElement).parentNode!.replaceChild(this._screenCanvas, this._userCanvas as HTMLCanvasElement);
        }

        var ctx = this._getWebGLContext(this._userCanvas!);
        if (!ctx || ctx < 0) {
            throw 'failed to create webgl context: err ' + ctx;
        }
        this._glContextHandle = ctx;
        this._nativeEngine = InfiniteLoader.module.makeEngine(this._userCanvas!.width, this._userCanvas!.height);

        requestAnimationFrame(this._requestRenderFrame.bind(this));
    }

    delete() {

    }

    addElement(element: Element) {
        this.elementCount++;
        if (element instanceof ImageElement) {
            this._nativeEngine?.addImageElement(element.getNativeElement());
        }
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
        var handle = InfiniteLoader.module.GL.createContext(canvas, contextAttributes);
        if (!handle) {
            throw 'GL.createContext failed';
        }
        InfiniteLoader.module.GL.makeContextCurrent(handle);
        // Emscripten does not enable this by default and Skia needs this to handle certain GPU corner cases.
        InfiniteLoader.module.GL.currentContext.GLctx.getExtension('WEBGL_debug_renderer_info');
        return handle;
    }

    private _requestRenderFrame() {
        let startTime = +new Date();
        InfiniteLoader.module.GL.makeContextCurrent(this._glContextHandle);
        let frameUpdated = this._nativeEngine ? this._nativeEngine.requestRenderFrame() : false;
        if (frameUpdated && this._nativeEngine && this._screenCanvas) {
            let pixelsPtr = this._nativeEngine.readPixels();
            var pixels = new Uint8ClampedArray(InfiniteLoader.module.HEAPU8.buffer, pixelsPtr, this._userCanvas!.width * this._userCanvas!.height * 4);
            var imageData = new ImageData(pixels, this._userCanvas!.width, this._userCanvas!.height);
            this._screenCanvas.getContext('2d')?.putImageData(imageData, 0, 0);
            let endTime = +new Date();
            console.log("元素数量：" + this.elementCount + " 耗时：" + (endTime - startTime) + "ms")
        }
        requestAnimationFrame(this._requestRenderFrame.bind(this));
    }
}