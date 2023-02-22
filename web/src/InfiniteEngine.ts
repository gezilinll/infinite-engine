//@ts-ignore
import Module from "./wasm/InfiniteEngine";

export type WebGLContextHandle = number;
export type ColorSpace = EmbindObject<ColorSpace>;

export interface EmbindObject<T extends EmbindObject<T>> {
    clone(): T;
    delete(): void;
    deleteLater(): void;
    isAliasOf(other: any): boolean;
    isDeleted(): boolean;
}

export interface GrDirectContext extends EmbindObject<GrDirectContext> {

}

export interface Canvas extends EmbindObject<Canvas> {
}

export interface Surface extends EmbindObject<Surface> {
    getCanvas(): Canvas;
}

export class InfiniteEngine {
    public static rawModule: any = undefined;
    public static surface: any = undefined;

    public static async init() {
        return new Promise(async (resolve, reject) => {
            const instance = await Module({
                locateFile: (path: string) => {
                    return path;
                },
            }).then((module: Module) => {
                return module;
            }).catch((err: any) => {
                console.error(err);
                return undefined;
            });

            if (instance) {
                InfiniteEngine.rawModule = instance;
                // @ts-ignore
                window.mainModule = instance;
            }
            if (InfiniteEngine.rawModule) {
                resolve(this.rawModule);
            } else {
                reject("!!!Load Wasm Error!!!");
            }
        });
    }

    private static GetWebGLContext(canvas: HTMLCanvasElement): WebGLContextHandle {
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
        };
        var handle = InfiniteEngine.rawModule.GL.createContext(canvas, contextAttributes);
        if (!handle) {
            return 0;
        }
        InfiniteEngine.rawModule.GL.makeContextCurrent(handle);
        // Emscripten does not enable this by default and Skia needs this to handle certain GPU
        // corner cases.
        InfiniteEngine.rawModule.GL.currentContext.GLctx.getExtension('WEBGL_debug_renderer_info');
        return handle;
        // contextAttributes['majorVersion'] = (typeof WebGL2RenderingContext !== 'undefined') ? 2 : 1;
        // contextAttributes.set("majorVersion", (typeof WebGL2RenderingContext !== 'undefined') ? 2 : 1);
    }

    private static setCurrentContext(ctx: WebGLContextHandle) {
        if (!ctx) {
            return false;
        }
        return InfiniteEngine.rawModule.GL.makeContextCurrent(ctx);
    }

    private static MakeWebGLContext(ctx: WebGLContextHandle): GrDirectContext | null {
        // Make sure we are pointing at the right WebGL context.
        if (!this.setCurrentContext(ctx)) {
            return null;
        }
        var grCtx = InfiniteEngine.rawModule._MakeGrContext();
        if (!grCtx) {
            return null;
        }
        // This context is an index into the emscripten-provided GL wrapper.
        grCtx._context = ctx;
        // var oldDelete = grCtx.delete.bind(grCtx);
        // We need to make sure we are focusing on the correct webgl context
        // when Skia cleans up the context.
        // grCtx['delete'] = function () {
        //     .setCurrentContext(this._context);
        //     oldDelete();
        // }.bind(grCtx);
        // Save this so it is easy to access (e.g. Image.readPixels)
        InfiniteEngine.rawModule.GL.currentContext.grDirectContext = grCtx;
        return grCtx;
    }

    private static MakeOnScreenGLSurface(ctx: any, width: number, height: number): Surface | null {
        if (!this.setCurrentContext(ctx._context)) {
            return null;
        }
        var surface;
        // zero is a valid value for sample count or stencil bits.
        console.log("MakeOnScreenGLSurface");
        console.log(ctx);
        surface = InfiniteEngine.rawModule._MakeOnScreenGLSurface(ctx as GrDirectContext, width, height);
        if (!surface) {
            return null;
        }
        surface._context = ctx._context;
        return surface;
    }

    public static MakeCanvasSurface(idOrElement: HTMLCanvasElement | string): Surface | null {
        var canvas: HTMLCanvasElement | null = null;
        var isHTMLCanvas = typeof HTMLCanvasElement !== 'undefined' && idOrElement instanceof HTMLCanvasElement;
        var isOffscreenCanvas = typeof OffscreenCanvas !== 'undefined' && idOrElement instanceof OffscreenCanvas;
        if (!isHTMLCanvas && !isOffscreenCanvas) {
            if (!document.getElementById(idOrElement as string)) {
                throw 'Canvas with id ' + idOrElement + ' was not found';
            } else {
                canvas = document.getElementById(idOrElement as string) as HTMLCanvasElement;
            }
        }

        var ctx = this.GetWebGLContext(canvas!);
        if (!ctx || ctx < 0) {
            throw 'failed to create webgl context: err ' + ctx;
        }
        console.log(ctx);

        var grcontext = this.MakeWebGLContext(ctx);
        console.log(grcontext);

        // Note that canvas.width/height here is used because it gives the size of the buffer we're
        // rendering into. This may not be the same size the element is displayed on the page, which
        // controlled by css, and available in canvas.clientWidth/height.
        var surface = this.MakeOnScreenGLSurface(grcontext!, canvas!.width, canvas!.height);
        console.log(surface);
        InfiniteEngine.surface = surface;
        function drawFrame() {
            InfiniteEngine.rawModule.DrawSomething(InfiniteEngine.surface);
            requestAnimationFrame(drawFrame);
        }
        requestAnimationFrame(drawFrame);
        // if (!surface) {
        //     // we need to throw away the old canvas (which was locked to
        //     // a webGL context) and create a new one so we can
        //     var newCanvas = canvas!.cloneNode(true);
        //     var parent = canvas!.parentNode;
        //     parent!.replaceChild(newCanvas, canvas!);
        //     // add a class so the user can detect that it was replaced.
        //     newCanvas.classList.add('ck-replaced');

        //     return CanvasKit.MakeSWCanvasSurface(newCanvas);
        // }
        return surface;
    }
}