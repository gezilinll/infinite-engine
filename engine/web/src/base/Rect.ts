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

    static Intersects(a: Rect, b: Rect) {
        if (a.intersects(b)) {
            let l = 0, t = 0, r = 0, bottom = 0;
            l = Math.max(a.left, b.left);
            t = Math.max(a.top, b.top);
            r = Math.min(a.right, b.right);
            bottom = Math.min(a.bottom, b.bottom);
            return Rect.MakeXYWH(l, t, r - l, bottom - t);
        }
        return Rect.MakeEmpty();
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

    get width() {
        return this._width;
    }

    get height() {
        return this._height;
    }

    intersects(b: Rect) {
        let L = Math.max(this.left, b.left);
        let R = Math.min(this.right, b.right);
        let T = Math.max(this.top, b.top);
        let B = Math.min(this.bottom, b.bottom);
        return L < R && T < B;
    }
}