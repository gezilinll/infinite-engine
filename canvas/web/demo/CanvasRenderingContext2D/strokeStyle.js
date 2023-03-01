import { CanvasLoader } from "../../src/CanvasLoader";
import { Canvas } from "../../src/Canvas";

const { default: wasmURL } = await import('../InfiniteCanvas.wasm?url');
CanvasLoader.wasmPath = wasmURL;

CanvasLoader.init().then(async () => {
  let canvas = new Canvas("canvas1");
  let ctx = canvas.getContext("2d");
  ctx.strokeStyle = "blue";
  ctx.strokeRect(10, 10, 100, 100);
  canvas.flush();
  canvas.delete();

  let canvas2 = new Canvas("canvas2");
  let ctx2 = canvas2.getContext("2d");
  for (let i = 0; i < 6; i++) {
    for (let j = 0; j < 6; j++) {
      ctx2.strokeStyle = `rgb(
          0,
          ${Math.floor(255 - 42.5 * i)},
          ${Math.floor(255 - 42.5 * j)})`;
      ctx2.beginPath();
      ctx2.arc(12.5 + j * 25, 12.5 + i * 25, 10, 0, Math.PI * 2, true);
      ctx2.stroke();
    }
  }
  canvas2.flush();
  canvas2.delete();
});