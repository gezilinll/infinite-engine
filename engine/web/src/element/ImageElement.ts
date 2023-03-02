import { InfiniteLoader } from "../InfiniteLoader";
import { Element } from "./Element";

export class ImageElement extends Element {
    private _freeCanvas: HTMLCanvasElement | null = null;
    private _sourceW: number = 0;
    private _sourceH: number = 0;

    constructor(id: number) {
        super(id);
        this.setNativeElement(InfiniteLoader.module.makeImageElement(id));
    }

    get sourceWidth() {
        return this._sourceW;
    }

    get sourceHeight() {
        return this._sourceH;
    }

    setSource(image: CanvasImageSource) {
        this._sourceW = image.width as number;
        this._sourceH = image.height as number;
        if (!this._freeCanvas) {
            this._freeCanvas = document.createElement('canvas');
        }
        let ctx = this._freeCanvas.getContext('2d', { willReadFrequently: true });
        this._freeCanvas.width = this._sourceW;
        this._freeCanvas.height = this._sourceH;
        ctx!.drawImage(image, 0, 0);

        let imageData = ctx!.getImageData(0, 0, this._sourceW, this._sourceH);
        var pptr = InfiniteLoader.module._malloc(4 * this._sourceW * this._sourceH);
        InfiniteLoader.module.HEAPU8.set(imageData.data, pptr); // We always want to copy the bytes into the WASM heap.
        // No need to _free pptr, Image takes it with SkData::MakeFromMalloc
        this._nativeElement?.setSource(pptr, this._sourceW, this._sourceH, imageData.data.length, 4 * this._sourceW);
    }

    setSrcRect(x: number, y: number, width: number, height: number) {
        this._nativeElement?.setSrcRect(x, y, width, height);
    }

    setDstRect(x: number, y: number, width: number, height: number) {
        this._nativeElement?.setDstRect(x, y, width, height);
    }
}