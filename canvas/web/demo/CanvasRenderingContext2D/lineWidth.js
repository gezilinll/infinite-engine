import { CanvasLoader } from "../../src/CanvasLoader";
import { Canvas } from "../../src/Canvas";

const { default: wasmURL } = await import('../InfiniteCanvas.wasm?url');
CanvasLoader.wasmPath = wasmURL;

CanvasLoader.init().then(async () => {
  let canvas = new Canvas("canvas1");
  let ctx = canvas.getContext("2d");
  ctx.lineWidth = 15;
  ctx.beginPath();
  ctx.moveTo(20, 20);
  ctx.lineTo(130, 130);
  ctx.rect(40, 40, 70, 70);
  ctx.stroke();
  ctx.flush();
  canvas.delete();
});