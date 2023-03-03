import { Element } from "./element/Element";
import { ElementRBush } from './base/ElementRBush'
import { Rect } from "./base/Rect";
import { Debuger, DebugFrame } from "./Debuger";

export class InfiniteEngine {
    private _userCanvas: HTMLCanvasElement | null = null;
    private _context: CanvasRenderingContext2D | null;
    private _width: number;
    private _height: number;

    private _sceneTree = new ElementRBush();
    private _elements: Array<Element> = new Array();
    private _addedElements: Array<Element> = new Array();
    private _addedElementsMap: Map<number, Element> = new Map();
    private _changedElementsMap: Map<number, Element> = new Map();

    private _debuger?: Debuger;

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
        element.registerStatusObserver(this._onElementStatusChanged.bind(this))
        this._addedElements.push(element);
        this._addedElementsMap.set(element.ID, element);
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

    set debuger(obj: Debuger) {
        this._debuger = obj;
    }

    private _onElementStatusChanged(element: Element) {
        if (this._addedElementsMap.get(element.ID)) {
            return;
        }
        this._changedElementsMap.set(element.ID, element);
    }

    private _requestRenderFrame() {
        let frameUpdated
            = !(this._changedElementsMap.size == 0 && this._addedElements.length == 0);
        if (frameUpdated) {
            this._debuger?.onRenderStart();
            let debugFrame = new DebugFrame();
            debugFrame.totalElementCount = this._elements.length;
            debugFrame.newElementCount = this._addedElements.length;
            debugFrame.changedElementCount = this._changedElementsMap.size;
            debugFrame.sceneTree = this._sceneTree;

            if (this._changedElementsMap.size > 0) {
                let rectsToClear: Array<Rect> = new Array();
                let rectsToDraw: Array<Rect> = new Array();
                for (let element of this._changedElementsMap) {
                    rectsToClear.push(element[1].rectToClear);
                    rectsToDraw.push(element[1].rectToDraw);
                }
                for (let rect of rectsToClear) {
                    this._context!.clearRect(rect.left, rect.top, rect.width, rect.height);
                    let hitElements = this._sceneTree.search({
                        minX: rect.left,
                        minY: rect.top,
                        maxX: rect.right,
                        maxY: rect.bottom
                    });
                    debugFrame.updatedElementCount += hitElements.length;
                    for (let element of hitElements) {
                        element.requestRenderDirty(this._context!, rect);
                    }
                }
                for (let rect of rectsToDraw) {
                    this._context!.clearRect(rect.left, rect.top, rect.width, rect.height);
                    let hitElements = this._sceneTree.search({
                        minX: rect.left,
                        minY: rect.top,
                        maxX: rect.right,
                        maxY: rect.bottom
                    });
                    debugFrame.updatedElementCount += hitElements.length;
                    for (let element of hitElements) {
                        element.requestRenderDirty(this._context!, rect);
                    }
                }
            }
            for (let element of this._addedElements) {
                element.requestRender(this._context!);
            }
            this._addedElements = [];
            this._addedElementsMap.clear();
            this._changedElementsMap.clear();
            this._debuger?.onRenderEnd(debugFrame);
        }
        requestAnimationFrame(this._requestRenderFrame.bind(this));
    }
}