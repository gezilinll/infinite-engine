import { InfiniteLoader } from "../InfiniteLoader";
import { Element } from "./Element";

export class ImageElement extends Element {
    private _freeCanvas: HTMLCanvasElement | null = null;

    constructor(id: number) {
        super(id);
        this.setNativeElement(InfiniteLoader.module.makeImageElement(id));
    }

    setSource(image: CanvasImageSource) {
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
        var pptr = InfiniteLoader.module._malloc(4 * width * height);
        InfiniteLoader.module.HEAPU8.set(imageData.data, pptr); // We always want to copy the bytes into the WASM heap.
        // No need to _free pptr, Image takes it with SkData::MakeFromMalloc
        this._nativeElement?.setSource(pptr, width, height, imageData.data.length, 4 * width);
    }

    setSrcRect(x: number, y: number, width: number, height: number) {
        this._nativeElement?.setSrcRect(x, y, width, height);
    }

    setDstRect(x: number, y: number, width: number, height: number) {
        this._nativeElement?.setDstRect(x, y, width, height);
    }
}