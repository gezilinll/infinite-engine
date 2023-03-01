import { CanvasLoader } from "../../src/CanvasLoader";
import { Canvas } from "../../src/Canvas";

const { default: wasmURL } = await import('../InfiniteCanvas.wasm?url');
CanvasLoader.wasmPath = wasmURL;

CanvasLoader.init().then(async () => {
  let canvas = new Canvas("canvas1");
  let ctx = canvas.getContext("2d");
  ctx.beginPath();
  ctx.moveTo(20, 20);
  ctx.lineWidth = 15;
  ctx.lineCap = 'round';
  ctx.lineTo(100, 100);
  ctx.stroke();
  canvas.flush();
  canvas.delete();

  let canvas2 = new Canvas("canvas2");
  let ctx2 = canvas2.getContext("2d");
  // Draw guides
  ctx2.strokeStyle = '#09f';
  ctx2.beginPath();
  ctx2.moveTo(10, 10);
  ctx2.lineTo(140, 10);
  ctx2.moveTo(10, 140);
  ctx2.lineTo(140, 140);
  ctx2.stroke();
  // Draw lines
  ctx2.strokeStyle = 'black';
  ['butt', 'round', 'square'].forEach((lineCap, i) => {
    ctx2.lineWidth = 15;
    ctx2.lineCap = lineCap;
    ctx2.beginPath();
    ctx2.moveTo(25 + i * 50, 10);
    ctx2.lineTo(25 + i * 50, 140);
    ctx2.stroke();
  });
  canvas2.flush();
  canvas2.delete();
});