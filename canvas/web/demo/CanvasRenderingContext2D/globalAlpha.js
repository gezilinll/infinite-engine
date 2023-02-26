import { CanvasLoader } from "../../src/CanvasLoader";
import { Canvas } from "../../src/Canvas";

const { default: wasmURL } = await import('../InfiniteCanvas.wasm?url');
CanvasLoader.wasmPath = wasmURL;

CanvasLoader.init().then(async () => {
  let canvas = new Canvas("isurface");
  let ctx = canvas.getContext("2d");

  // Draw background
  ctx.fillStyle = '#FD0';
  ctx.fillRect(0, 0, 75, 75);
  ctx.fillStyle = '#6C0';
  ctx.fillRect(75, 0, 75, 75);
  ctx.fillStyle = '#09F';
  ctx.fillRect(0, 75, 75, 75);
  ctx.fillStyle = '#F30';
  ctx.fillRect(75, 75, 75, 75);
  ctx.fillStyle = '#FFF';

  // Set transparency value
  ctx.globalAlpha = 0.2;

  // Draw transparent circles
  for (let i = 0; i < 7; i++) {
    ctx.beginPath();
    ctx.arc(75, 75, 10 + 10 * i, 0, Math.PI * 2, true);
    ctx.fill();
  }

  ctx.globalAlpha = 0.5;

  ctx.fillStyle = 'blue';
  ctx.fillRect(200, 10, 100, 100);

  ctx.fillStyle = 'red';
  ctx.fillRect(240, 50, 100, 100);

  canvas.delete();
});