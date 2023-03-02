import RBush, { BBox } from 'rbush';
import { Element } from '../element/Element'

export class ElementRBush extends RBush<Element> {
    toBBox(item: Element): BBox {
        let dstRect = item.dstRect;
        return { minX: dstRect.left, minY: dstRect.top, maxX: dstRect.right, maxY: dstRect.bottom };
    }

    compareMinX(a: Element, b: Element): number {
        let aDst = a.dstRect;
        let bDst = b.dstRect;
        if (aDst.left < bDst.left) {
            return -1;
        } else if (aDst.left > bDst.left) {
            return 1;
        } else {
            return 0;
        }
    }

    compareMinY(a: Element, b: Element): number {
        let aDst = a.dstRect;
        let bDst = b.dstRect;
        if (aDst.top < bDst.top) {
            return -1;
        } else if (aDst.top > bDst.top) {
            return 1;
        } else {
            return 0;
        }
    }
}