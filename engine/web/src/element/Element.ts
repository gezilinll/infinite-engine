import { ElementRBush } from "../base/ElementRBush";
import { Rect } from "../base/Rect";

export declare type ElementStatusObserver = (element: Element) => void;

export class ElementStatus {
    isInScene: boolean = false;
}

export class Element {
    private _id: number;
    protected _sceneTree: ElementRBush | null = null;
    private _statusObserver: ElementStatusObserver | null = null;

    constructor(id: number) {
        this._id = id;
    }

    get ID() {
        return this._id;
    }

    get rectToClear() {
        return Rect.MakeEmpty();
    }

    get rectToDraw() {
        return Rect.MakeEmpty();
    }

    get dstRect() {
        return Rect.MakeEmpty();
    }

    bindSceneTree(sceneTree: ElementRBush) { this._sceneTree = sceneTree; }

    registerStatusObserver(observer: ElementStatusObserver) {
        this._statusObserver = observer;
    }

    protected _invokeStatusObserver() {
        if (this._statusObserver) {
            this._statusObserver(this);
        }
    }

    requestRender(context: CanvasRenderingContext2D) { }

    requestRenderDirty(context: CanvasRenderingContext2D, dirtyRect: Rect) { }
}