import { CanvasLoader } from "../../src/CanvasLoader";
import { Canvas } from "../../src/Canvas";

const { default: wasmURL } = await import('../InfiniteCanvas.wasm?url');
CanvasLoader.wasmPath = wasmURL;

CanvasLoader.init().then(async () => {
  let canvas = new Canvas("canvas1");
  let ctx = canvas.getContext("2d");
  ctx.lineWidth = 20;
  ctx.lineJoin = "round";
  ctx.beginPath();
  ctx.moveTo(20, 20);
  ctx.lineTo(190, 100);
  ctx.lineTo(280, 20);
  ctx.lineTo(280, 150);
  ctx.stroke();
  ctx.flush();
  canvas.delete();

  let canvas2 = new Canvas("canvas2");
  let ctx2 = canvas2.getContext("2d");
  ctx2.lineWidth = 10;

  ["round", "bevel", "miter"].forEach((join, i) => {
    ctx2.lineJoin = join;
    ctx2.beginPath();
    ctx2.moveTo(-5, 5 + i * 40);
    ctx2.lineTo(35, 45 + i * 40);
    ctx2.lineTo(75, 5 + i * 40);
    ctx2.lineTo(115, 45 + i * 40);
    ctx2.lineTo(155, 5 + i * 40);
    ctx2.stroke();
  });
  ctx2.flush();
  canvas2.delete();
});