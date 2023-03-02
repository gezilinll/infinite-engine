import { ElementRBush } from "../base/ElementRBush";
import { Element, ElementStatus } from "./Element";
import { Rect } from "../base/Rect";


class ImageElementStatus extends ElementStatus {
    lastDstRect: Rect = Rect.MakeEmpty();
    srcRect: Rect = Rect.MakeEmpty();
    dstRect: Rect = Rect.MakeEmpty();
}

export class ImageElement extends Element {
    private _sourceW: number = 0;
    private _sourceH: number = 0;
    private _source: CanvasImageSource | null = null;
    private _status: ImageElementStatus = new ImageElementStatus();

    constructor(id: number) {
        super(id);
    }

    get dstRect() {
        return this._status.dstRect;
    }
    get sourceWidth() {
        return this._sourceW;
    }

    get sourceHeight() {
        return this._sourceH;
    }

    get rectToClear() {
        return this._status.lastDstRect;
    }

    get rectToDraw() {
        return this._status.dstRect;
    }

    bindSceneTree(sceneTree: ElementRBush): void {
        super.bindSceneTree(sceneTree);
        this._updateSceneTree(false);
    }

    private _updateSceneTree(removed: boolean) {
        if (removed) {
            // TODO
        } else if (this._sceneTree && this._source) {
            if (this._status.isInScene) {
                this._sceneTree.remove(this);
            }
            this._sceneTree.insert(this);
            this._status.isInScene = true;
        }
    }

    set source(image: CanvasImageSource) {
        this._source = image;
        this._sourceW = image.width as number;
        this._sourceH = image.height as number;
        if (this._status.srcRect.width <= 0 || this._status.srcRect.height <= 0) {
            this._status.srcRect = Rect.MakeXYWH(0, 0, image.width as number, image.height as number);
        }
        if (this._status.dstRect.width <= 0 || this._status.dstRect.height <= 0) {
            this._status.dstRect = Rect.MakeXYWH(0, 0, image.width as number, image.height as number);
        }
        super._invokeStatusObserver();
        this._updateSceneTree(false);
    }

    set srcRect(rect: Rect) {
        this._status.srcRect = rect;
        super._invokeStatusObserver();
        this._updateSceneTree(false);
    }

    set dstRect(rect: Rect) {
        this._status.dstRect = rect;
        super._invokeStatusObserver();
        this._updateSceneTree(false);
    }

    requestRender(context: CanvasRenderingContext2D): void {
        if (this._source) {
            context.drawImage(this._source,
                this._status.srcRect.left, this._status.srcRect.top, this._status.srcRect.width, this._status.srcRect.height,
                this._status.dstRect.left, this._status.dstRect.top, this._status.dstRect.width, this._status.dstRect.height);
            this._status.lastDstRect = this._status.dstRect;
        }
    }

    requestRenderDirty(context: CanvasRenderingContext2D, dirtyRect: Rect): void {
        if (this._source) {
            let dstRect = Rect.Intersects(this._status.dstRect, dirtyRect);
            let sx = 0, sy = 0, sw = 0, sh = 0;
            let wScale = this._status.srcRect.width / this._status.dstRect.width;
            sx = this._status.srcRect.left + ((dstRect.left - this._status.dstRect.left) * wScale);
            sw = Math.min(dstRect.width * wScale, this._source.width as number);;
            let hScale = this._status.srcRect.height / this._status.dstRect.height;
            sy = ((dstRect.top - this._status.dstRect.top) * hScale) + this._status.srcRect.top;
            sh = Math.min(dstRect.height * hScale, this._source.height as number);
            context.drawImage(this._source, sx, sy, sw, sh, dstRect.left, dstRect.top, dstRect.width, dstRect.height);
            this._status.lastDstRect = this._status.dstRect;
        }
    }
}