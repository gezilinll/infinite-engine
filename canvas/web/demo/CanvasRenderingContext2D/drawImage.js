import { CanvasLoader } from "../../src/CanvasLoader";
import { Canvas } from "../../src/Canvas";
import { FontInfo } from "../../src/FontInfo";

const { default: wasmURL } = await import('../InfiniteCanvas.wasm?url');
CanvasLoader.wasmPath = wasmURL;

CanvasLoader.init().then(async () => {
  const response = await fetch("http://rqm1nmwwk.hn-bkt.clouddn.com/logo.jpg");
  const arrayBuffer = await response.arrayBuffer();
  // response.blob() is preferable when you don't need both a Blob *and* an ArrayBuffer.
  const blob = new Blob([arrayBuffer]);
  const imageBitmap = await createImageBitmap(blob);
  const canvasBitmapElement = document.createElement('canvas');
  canvasBitmapElement.width = imageBitmap.width;
  canvasBitmapElement.height = imageBitmap.height;
  const ctxBitmap = canvasBitmapElement.getContext('bitmaprenderer');
  ctxBitmap.transferFromImageBitmap(imageBitmap);

  let canvas = new Canvas("canvas1");
  let ctx = canvas.getContext("2d");
  ctx.drawImage(canvasBitmapElement, 33, 71, 104, 124, 21, 20, 87, 104);
  canvas.flush();
  canvas.delete();

  let canva2 = new Canvas("canvas2");
  let ctx2 = canva2.getContext("2d");
  ctx2.drawImage(canvasBitmapElement, 0, 0);
  canva2.flush();
  canva2.delete();
});