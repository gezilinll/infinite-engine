import { CanvasLoader } from "../../src/CanvasLoader";
import { Canvas } from "../../src/Canvas";

const { default: wasmURL } = await import('../InfiniteCanvas.wasm?url');
CanvasLoader.wasmPath = wasmURL;

CanvasLoader.init().then(async () => {
  let canvas = new Canvas("canvas1");
  let ctx = canvas.getContext("2d");
  ctx.strokeStyle = '#09f';
  ctx.lineWidth = 2;
  ctx.strokeRect(-5, 50, 160, 50);
  ctx.strokeStyle = '#000';
  ctx.lineWidth = 10;
  ctx.miterLimit = 10;
  ctx.beginPath();
  ctx.moveTo(0, 100);
  for (var i = 0; i < 24; i++) {
    var dy = i % 2 == 0 ? 25 : -25;
    ctx.lineTo(Math.pow(i, 1.5) * 2, 75 + dy);
  }
  ctx.stroke();
  canvas.flush();
  canvas.delete();
});