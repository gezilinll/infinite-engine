import { CanvasLoader } from "../../src/CanvasLoader";
import { Canvas } from "../../src/Canvas";

const { default: wasmURL } = await import('../InfiniteCanvas.wasm?url');
CanvasLoader.wasmPath = wasmURL;

CanvasLoader.init().then(async () => {
  let canvas = new Canvas("canvas1");
  let ctx = canvas.getContext("2d");
  // Shadow
  ctx.shadowColor = "red";
  ctx.shadowOffsetY = 25;
  ctx.shadowBlur = 10;

  // Rectangle
  ctx.fillStyle = "blue";
  ctx.fillRect(20, 20, 150, 80);
  canvas.flush();
  canvas.delete();
});