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

    setSource(image: CanvasImageSource) {
        this._source = image;
        this._updateSceneTree(false);
    }

    set srcRect(rect: Rect) {
        this._status.srcRect = rect;
        this._updateSceneTree(false);
    }

    set dstRect(rect: Rect) {
        this._status.dstRect = rect;
        this._updateSceneTree(false);
    }

    requestRender(context: CanvasRenderingContext2D): void {
        if (this._source) {
            context.drawImage(this._source, 0, 0, this._source.width as number, this._source.height as number, 0, 0, 1000, 1000);
        }
    }
}