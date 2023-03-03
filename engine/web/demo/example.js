import { InfiniteEngine } from "../src/InfiniteEngine";
import { ImageElement } from "../src/element/ImageElement";
import { Rect } from "../src/base/Rect";
import { Debuger } from "../src/Debuger";

async function downloadImage(path) {
    const response = await fetch(path);
    const arrayBuffer = await response.arrayBuffer();
    // response.blob() is preferable when you don't need both a Blob *and* an ArrayBuffer.
    const blob = new Blob([arrayBuffer]);
    const imageBitmap = await createImageBitmap(blob);
    const canvasBitmapElement = document.createElement('canvas');
    canvasBitmapElement.width = imageBitmap.width;
    canvasBitmapElement.height = imageBitmap.height;
    const ctxBitmap = canvasBitmapElement.getContext('bitmaprenderer');
    ctxBitmap.transferFromImageBitmap(imageBitmap);
    return canvasBitmapElement;

}

document.getElementById('message').textContent = "等待下载图片完成..."

const bitmapMap = new Map();
bitmapMap.set("0", await downloadImage("http://rqm1nmwwk.hn-bkt.clouddn.com/5.jpg"));
bitmapMap.set("1", await downloadImage("http://rqm1nmwwk.hn-bkt.clouddn.com/26.jpg"));
bitmapMap.set("2", await downloadImage("http://rqm1nmwwk.hn-bkt.clouddn.com/35.jpg"));
bitmapMap.set("3", await downloadImage("http://rqm1nmwwk.hn-bkt.clouddn.com/62.jpg"));
bitmapMap.set("4", await downloadImage("http://rqm1nmwwk.hn-bkt.clouddn.com/71.jpg"));

document.getElementById('message').textContent = "可正常使用"

let engine = new InfiniteEngine("isurface");
let debuger = new Debuger();
debuger.messageOutput = "message";
engine.debuger = debuger;

// let drawSceneEnabled = true;
// engine.enableDrawScene();
// document.getElementById("drawScene").onclick = async function () {
//     drawSceneEnabled = !drawSceneEnabled;
//     if (drawSceneEnabled) {
//         document.getElementById("drawScene").textContent = "场景线绘制已开启";
//         engine.enableDrawScene();
//     } else {
//         document.getElementById("drawScene").textContent = "场景线绘制已关闭";
//         engine.disableDrawScene();
//     }
// }

function getRandomInt(max) {
    return Math.floor(Math.random() * max);
}

function addImage() {
    let imageIndex = getRandomInt(5);
    let canvasBitmapElement = bitmapMap.get("" + imageIndex);
    let sx = 0;
    let sy = 0;
    let sw = canvasBitmapElement.width;
    let sh = canvasBitmapElement.height;
    let dx = getRandomInt(engine.width / 2);
    let dy = getRandomInt(engine.height / 2);
    let dw = Math.min(sw, getRandomInt(engine.width / 3) + 200);
    let dh = dw / (sw / sh);
    let imageElement = new ImageElement(engine.elementsLength);
    imageElement.source = canvasBitmapElement;
    imageElement.srcRect = Rect.MakeXYWH(sx, sy, sw, sh);
    imageElement.dstRect = Rect.MakeXYWH(dx, dy, dw, dh);
    engine.addElement(imageElement);
}

function moveImage() {
    let elementLength = engine.elementsLength;
    let elementID = getRandomInt(elementLength);
    let element = engine.getElementByID(elementID);
    let dx = getRandomInt(engine.width / 2);
    let dy = getRandomInt(engine.height / 2);
    let dw = Math.min(element.sourceWidth, getRandomInt(engine.width / 3) + 200);
    let dh = dw / (element.sourceWidth / element.sourceHeight);
    element.dstRect = Rect.MakeXYWH(dx, dy, dw, dh);
}

function startMadMode() {
    addImage();
    moveImage();
    requestAnimationFrame(startMadMode);
}

document.getElementById("addImage").onclick = async function () {
    addImage();

}

document.getElementById("moveImage").onclick = async function () {
    moveImage();
}

document.getElementById("madMode").onclick = async function () {
    startMadMode();
}