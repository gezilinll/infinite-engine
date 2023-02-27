import { CanvasLoader } from "../../src/CanvasLoader";
import { Canvas } from "../../src/Canvas";

const { default: wasmURL } = await import('../InfiniteCanvas.wasm?url');
CanvasLoader.wasmPath = wasmURL;

CanvasLoader.init().then(async () => {
  let canvas = new Canvas("canvas1");
  let ctx = canvas.getContext("2d");
  // Shadow
  ctx.shadowColor = "red";
  ctx.shadowBlur = 15;
  // Rectangle
  ctx.fillStyle = "blue";
  ctx.fillRect(20, 20, 150, 100);
  ctx.flush();
  canvas.delete();
});