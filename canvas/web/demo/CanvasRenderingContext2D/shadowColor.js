import { CanvasLoader } from "../../src/CanvasLoader";
import { Canvas } from "../../src/Canvas";

const { default: wasmURL } = await import('../InfiniteCanvas.wasm?url');
CanvasLoader.wasmPath = wasmURL;

CanvasLoader.init().then(async () => {
  let canvas = new Canvas("canvas1");
  let ctx = canvas.getContext("2d");
  // Shadow
  ctx.shadowColor = 'red';
  ctx.shadowOffsetX = 10;
  ctx.shadowOffsetY = 10;

  // Filled rectangle
  ctx.fillRect(20, 20, 100, 100);

  // Stroked rectangle
  ctx.lineWidth = 6;
  ctx.strokeRect(170, 20, 100, 100);
  canvas.flush();
  canvas.delete();
});