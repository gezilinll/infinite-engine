import { Element } from "./element/Element";
import { ElementRBush } from './base/ElementRBush'
export class InfiniteEngine {
    private _userCanvas: HTMLCanvasElement | null = null;
    private _context: CanvasRenderingContext2D | null;
    private _width: number;
    private _height: number;
    private _elements: Array<Element> = new Array();
    private _addedElements: Array<Element> = new Array();
    private _addedElementsMap: Map<number, Element> = new Map();
    private _changedElementsMap: Map<number, Element> = new Map();
    private _sceneTree = new ElementRBush();

    constructor(idOrElement: HTMLCanvasElement | string) {
        var isHTMLCanvas = typeof HTMLCanvasElement !== 'undefined' && idOrElement instanceof HTMLCanvasElement;
        if (!isHTMLCanvas) {
            if (!document.getElementById(idOrElement as string)) {
                throw 'Canvas with id ' + idOrElement + ' was not found';
            } else {
                this._userCanvas = document.getElementById(idOrElement as string) as HTMLCanvasElement;
            }
        } else {
            this._userCanvas = idOrElement as HTMLCanvasElement;
        }
        this._width = this._userCanvas.width;
        this._height = this._userCanvas.height;
        this._context = this._userCanvas.getContext('2d');

        this._sceneTree.insert

        requestAnimationFrame(this._requestRenderFrame.bind(this));
    }

    delete() {

    }

    addElement(element: Element) {
        element.bindSceneTree(this._sceneTree);
        this._addedElements.push(element);
        this._elements.push(element);
    }

    getElementByID(id: number): Element | undefined {
        return this._elements.find(element => { return element.ID == id; });
    }

    get elementsLength() {
        return this._elements.length;
    }

    get width() {
        return this._width;
    }

    get height() {
        return this._height;
    }

    enableDrawScene() {

    }

    disableDrawScene() {

    }

    private _requestRenderFrame() {
        let frameUpdated
            = !(this._changedElementsMap.size == 0 && this._addedElements.length == 0);
        if (frameUpdated) {
            let startTime = +new Date();
            for (let element of this._addedElements) {
                element.requestRender(this._context!);
            }
            this._addedElements = [];
            this._changedElementsMap.clear();
            let endTime = +new Date();
            console.log("元素数量：" + this._elements.length + " 耗时：" + (endTime - startTime) + "ms")
        }
        requestAnimationFrame(this._requestRenderFrame.bind(this));
    }
}