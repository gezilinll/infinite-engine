export class Rect {
    private _x: number = 0;
    private _y: number = 0;
    private _width: number = 0;
    private _height: number = 0;

    constructor(x: number, y: number, width: number, height: number) {
        this._x = x;
        this._y = y;
        this._width = width;
        this._height = height;
    }

    static MakeEmpty() {
        return new Rect(0, 0, 0, 0);
    }

    static MakeXYWH(x: number, y: number, width: number, height: number) {
        return new Rect(x, y, width, height);
    }

    get left() {
        return this._x;
    }

    get top() {
        return this._y;
    }

    get right() {
        return this._x + this._width;
    }

    get bottom() {
        return this._y + this._height;
    }
}