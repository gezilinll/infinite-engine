import { ElementRBush } from "../base/ElementRBush";
import { Rect } from "../base/Rect";

export class ElementStatus {
    isInScene: boolean = false;
}

export class Element {
    private _id: number;
    protected _sceneTree: ElementRBush | null = null;

    constructor(id: number) {
        this._id = id;
    }

    get ID() {
        return this._id;
    }

    get dstRect() {
        return Rect.MakeEmpty();
    }

    bindSceneTree(sceneTree: ElementRBush) { this._sceneTree = sceneTree; }

    requestRender(context: CanvasRenderingContext2D) {
    }

}