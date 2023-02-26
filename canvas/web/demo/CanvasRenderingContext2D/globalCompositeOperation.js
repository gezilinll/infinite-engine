import { CanvasLoader } from "../../src/CanvasLoader";
import { Canvas } from "../../src/Canvas";

const { default: wasmURL } = await import('../InfiniteCanvas.wasm?url');
CanvasLoader.wasmPath = wasmURL;

CanvasLoader.init().then(async () => {
  let canvas = new Canvas("isurface");
  let ctx = canvas.getContext("2d");

  ctx.globalCompositeOperation = "xor";

  ctx.fillStyle = "blue";
  ctx.fillRect(10, 10, 100, 100);

  ctx.fillStyle = "red";
  ctx.fillRect(50, 50, 100, 100);

  canvas.delete();
});